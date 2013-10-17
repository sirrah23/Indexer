CFILES = main.c index.c tokenizer.c sorted-list.c
HFILES = index.h tokenizer.h sorted-list.h

all: index

index: $(CFILES) $(HFILES)
	gcc $(CFILES) -Wall -g -o index

clean:
	rm -f index
