CFLAGS = -Wall -lm -I.
DEPS = shop.h
CC = gcc

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shop: customer.c shop.c
	gcc -pthread -o shop customer.c shop.c $(CFLAGS)

clean:
	rm shop
