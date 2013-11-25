ORDER_C = main.c database.c queue.c
ORDER_H = database.h queue.h

all: order

order: $(ORDER_C) $(ORDER_H)
	gcc $(ORDER_C) -Wall -g -o order

clean:
	rm -f order
