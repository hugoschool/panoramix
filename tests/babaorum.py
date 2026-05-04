import subprocess
import re
from random import randint

DRUID_PREFIX = "Druid: "
DRUID_START = DRUID_PREFIX + "I'm ready... but sleepy..."
DRUID_WORK = DRUID_PREFIX + "Ah! Yes, yes, I'm awake! Working on it! Beware I can only make ([0-9]*) more refills after this one\\.$"
DRUID_END = DRUID_PREFIX + "I'm out of viscum. I'm going back to... zZz"

VILLAGER_PREFIX = "Villager ([0-9]*): "
VILLAGER_START = VILLAGER_PREFIX + "Going into battle!"
VILLAGER_DRINK = VILLAGER_PREFIX + "I need a drink... I see ([0-9]*) servings left."
VILLAGER_POTION = VILLAGER_PREFIX + "Hey Pano wake up! We need more potion."
VILLAGER_FIGHT = VILLAGER_PREFIX + "Take that roman scum! Only ([0-9]*) left."
VILLAGER_END = VILLAGER_PREFIX + "I'm going to sleep now."

RANDOM_PARAMS = 25

class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    PURPLE = '\033[35m'
    RESET = '\033[0m'

def generateValidParams() -> (int, int, int, int):
    nbVillagers = randint(1, 50)
    nbFights = randint(1, 50)
    potSize = randint(1, 80)
    nbRefills = randint(1, 80)

    while potSize * nbRefills < nbVillagers * nbFights:
        potSize = randint(1, 80)
        nbRefills = randint(1, 80)

    return (nbVillagers, potSize, nbFights, nbRefills)

def generateInvalidParams() -> (int, int, int, int):
    nbVillagers = randint(1, 50)
    nbFights = randint(1, 50)
    potSize = randint(1, 80)
    nbRefills = randint(1, 80)

    while potSize * nbRefills >= nbVillagers * nbFights:
        potSize = randint(1, 80)
        nbRefills = randint(1, 80)

    return (nbVillagers, potSize, nbFights, nbRefills)

class PanoramixOutputTest:
    def __init__(self, nbVillagers, potSize, nbFights, nbRefills):
        self.nbVillagers = nbVillagers
        self.potSize = potSize
        self.nbFights = nbFights
        self.nbRefills = nbRefills

        self._statusCode = 0
        self.maxTimeout = 3

    def launch(self) -> None:
        self.pano = subprocess.Popen(
            [
                "timeout",
                f"{self.maxTimeout}s",
                "./panoramix",
                str(self.nbVillagers),
                str(self.potSize),
                str(self.nbFights),
                str(self.nbRefills)
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )

        self.stdout = None

        try:
            self.stdout = self.pano.communicate(timeout=3)[0].decode()
            self._statusCode = self.pano.returncode
        except Exception as e:
            self._statusCode = 84
            pass

        if self._statusCode != 0:
            raise Exception("Non zero return code")

    def statusCode(self) -> int:
        return self._statusCode

    def verifyDruid(self, druid) -> None:
        for i, line in enumerate(druid):
            if i == 0 and line != DRUID_START:
                raise Exception("Invalid druid start")

            if i == len(druid) - 1 and line != DRUID_END:
                raise Exception("Invalid druid end")

            if i == 0 or i == len(druid) - 1:
                continue

            druidMatches = re.findall(DRUID_WORK, line)

            if len(druidMatches) != 1:
                raise Exception("Incorrect druid instruction")

    def verifyVillager(self, villager, nb) -> None:
        i = 0
        for _ in range(len(villager)):
            i += 1
            if i >= len(villager):
                break
            line = villager[i]

            if i == 0 and len(re.findall(VILLAGER_START, line)) != 1:
                raise Exception("Invalid villager start")

            if i == len(villager) - 1 and len(re.findall(VILLAGER_END, line)) != 1:
                raise Exception("Invalid villager end")

            if i == 0 or i == len(villager) - 1:
                continue

            villagerDrink = re.findall(VILLAGER_DRINK, line)
            if len(villagerDrink) != 1 and len(villagerDrink[0]) != 2:
                raise Exception("Incorrect villager drink instruction")

            i += 1
            line = villager[i]

            servingsLeft = int(villagerDrink[0][1])
            if servingsLeft == 0:
                villagerPotion = re.findall(VILLAGER_POTION, line)
                if len(villagerPotion) != 1 and len(villagerPotion[0]) != 2:
                    raise Exception("Incorrect villager potion instruction")

                i += 1
                line = villager[i]

            villagerFight = re.findall(VILLAGER_FIGHT, line)
            if len(villagerFight) != 1 and len(villagerFight[0]) != 2:
                raise Exception("Incorrect villager fight instruction")

    # Returns nothing if it's valid
    def validate(self):
        lines = self.stdout.splitlines()

        villagers = {}
        druid = []

        for line in lines:
            villagerMatchess = re.findall(VILLAGER_PREFIX, line)
            if line.startswith(DRUID_PREFIX):
                druid.append(line)
            elif len(villagerMatchess) != 0:
                nb = int(villagerMatchess[0])

                if nb not in villagers:
                    villagers[nb] = []
                villagers[nb].append(line)
            else:
                raise Exception(f"Invalid line: {line}")

        self.verifyDruid(druid)
        for index in villagers:
            self.verifyVillager(villagers[index], index)

if __name__ == "__main__":
    print(f"{Colors.PURPLE}Babaorum - Panoramix Tester{Colors.RESET}")
    print()
    print("What the tester does:")
    print("- Verify status codes")
    print("- Verify the order of druid instructions")
    print("- Verify the order of each villager instructions")
    print()
    print("What the tester doesn't do:")
    print("- Correctly assert that the pot has enough size")
    print()

    tests = [
        (PanoramixOutputTest(3, 5, 3, 1), "Subject test", 0),
        (PanoramixOutputTest(3, 15, 3, 1), "Higher pot size than fights", 0),
        (PanoramixOutputTest(-1, 5, 3, 1), "Invalid nb_villagers", 84),
        (PanoramixOutputTest(3, -1, 3, 1), "Invalid pot_size", 84),
        (PanoramixOutputTest(3, 5, -1, 1), "Invalid nb_fights", 84),
        (PanoramixOutputTest(3, 5, 3, -1), "Invalid nb_refills", 84),
        (PanoramixOutputTest(0, 0, 0, 0), "All numbers to 0", 84),
        (PanoramixOutputTest("", "", "", ""), "No args", 84),
    ]

    for i in range(RANDOM_PARAMS):
        params = generateValidParams()
        tests.append((PanoramixOutputTest(params[0], params[1], params[2], params[3]), f"Random valid params {i}", 0))

    for i in range(RANDOM_PARAMS):
        params = generateInvalidParams()
        tests.append((PanoramixOutputTest(params[0], params[1], params[2], params[3]), f"Random invalid params {i}", 0))

    for (test, name, statusCode) in tests:
        try:
            print(f"Launching {Colors.PURPLE}\"{name}\"{Colors.RESET} with: {test.nbVillagers}, {test.potSize}, {test.nbFights}, {test.nbRefills}")
            test.launch()
        except Exception as e:
            if test.statusCode() == statusCode:
                print(f"{Colors.GREEN}Valid test!{Colors.RESET} Status code = {statusCode}")
            continue

        try:
            test.validate()
            print(f"{Colors.GREEN}Valid test!{Colors.RESET}")
        except Exception as e:
            print(f"{Colors.RED}Error: {e}{Colors.RESET}")
