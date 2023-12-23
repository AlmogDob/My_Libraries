CFLAGS = -Wall -Wextra -lm
COPTIONS =
DUNPING = ***

build: test

test: test.o
	@echo "$(DUNPING)Linking and producing the test applitestion$(DUNPING)"
	gcc $(CFLAGS) test.o -o test $(COPTIONS)

test.o: test.c
	@echo "$(DUNPING)Compilings test.c file$(DUNPING)"
	gcc $(CFLAGS) -c test.c $(COPTIONS)

clean: 
	@echo "$(DUNPING)Removing everthing but the source files$(DUNPING)"
	rm test.o test

run: 
	./test