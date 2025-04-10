CFLAGS = -Wall -Wextra -lm -lSDL2 -lSDL2_ttf

example: build run clean
	@echo example done
build: example.c
	gcc ./example.c $(CFLAGS) -o example 

run:
	./example

clean:
	rm example

temp:
	gcc ./temp.c $(CFLAGS) -o temp
	./temp
	rm temp

debug_build: example.c
	gcc ./example.c $(CFLAGS) -g -o example 
