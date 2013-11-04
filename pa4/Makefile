CFILES = main.c hash.c list.c
HFILES = hash.h list.h

all: search

search: $(CFILES) $(HFILES)
	gcc $(CFILES) -Wall -g -o search

clean:
	rm -f search
