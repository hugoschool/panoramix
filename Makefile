CC	:=	clang
CFLAGS	:=	-Wall -Wextra -std=gnu17
CPPFLAGS	:=	-I include

SRC	:=	src/main.c

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
