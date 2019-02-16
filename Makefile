CFLAGS = -Wall -lm -I.
DEPS = shop.h
CC = gcc

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

individual: individual.c shop.c
	gcc -pthread -o individual individual.c shop.c $(CFLAGS)

clean:
	rm individual
