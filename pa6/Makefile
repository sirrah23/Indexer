ORDER_C = main.c my_malloc_prevs.c
ORDER_H = malloc2.h

all: test

test: $(ORDER_C) $(ORDER_H)
	gcc $(ORDER_C) -Wall -g -o test

clean:
	rm -f test
