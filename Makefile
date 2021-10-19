CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
LIBS=
SRC=src/main.c src/recdir.c

dupfinder: $(SRC)
	$(CC) $(CFLAGS) -o dupfinder $(SRC) $(LIBS)
