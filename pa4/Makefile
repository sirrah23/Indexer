SEARCH_C = main.c hash.c list.c
SEARCH_H = hash.h list.h
INDEX_C = main1.c index.c tokenizer.c sorted-list.c
INDEX_H = index.h tokenizer.h sorted-list.h

all: search

search: $(SEARCH_C) $(SEARCH_H)
	gcc $(SEARCH_C) -Wall -g -o search

all: index

index: $(INDEX_C) $(INDEX_H)
	gcc $(INDEX_C) -Wall -g -o index

clean:
	rm -f search
	rm -f index
