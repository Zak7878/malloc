CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -fPIC -g
SRC = malloc.c
OBJ = $(SRC:.c==.o)
LDFLAGS = -shared
LIB = libmalloc.so

all : $(LIB)
	$(CC) $(CFLAGS) -o malloc $(SRC) -L ./lib

libmalloc.so : malloc.o
	$(LINK.o) -o $@ $^ -L ./lib

check : 
	$(CC)  $(OBJ)
clean :	
	rm -f $(OBJ)

mrproper : clean
	rm -f malloc libmalloc.so
