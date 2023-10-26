CC=gcc
CFLAGS=-pedantic -Wall -Wextra -g
NAME=hinfosvc

all:
		$(CC) $(GFLAGS) hinfosvc.c -o $(NAME)
Clean:

		-rm -f *.o $(NAME)
