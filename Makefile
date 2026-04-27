CC	:=	clang
CFLAGS	:=	-Wall -Wextra -std=gnu17
CPPFLAGS	:=	-I include

ifeq ($(ENV), dev)
	CFLAGS	+=	-g3
endif

SRC	:=	src/main.c \
		src/druid.c \
		src/panoramix.c \
		src/villagers.c

OBJ	:=	$(SRC:.c=.o)

BINARY	:=	panoramix

all:	$(BINARY)

$(BINARY):	$(OBJ)
	$(CC) -o $(BINARY) $(OBJ) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(BINARY)

re:	fclean all

.PHONY: all clean fclean re
