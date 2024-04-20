fast: build run clean
	
build: main.c
	gcc -Wall -Wextra ./main.c -lm -o main

run:
	@echo -+-+-+-+-+-+-+-+-+-+-+-+-+-+
	./main
	@echo -+-+-+-+-+-+-+-+-+-+-+-+-+-+

clean:
	rm main

test:
	gcc -Wall -Wextra ./test.c -lm -o test
	./test
	rm test