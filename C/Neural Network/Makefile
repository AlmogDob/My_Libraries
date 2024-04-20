fast: build run clean
	
build: main.c
	gcc -Wall -Wextra ./main.c -lm -o main

run:
	@echo
	@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	./main
	@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@echo

clean:
	rm main

test:
	gcc -Wall -Wextra ./test.c -lm -o test
	./test
	rm test

times2: 
	@for number in 1 2 ; do \
    gcc -Wall -Wextra ./main.c -lm -o main;\
	echo; echo run number $$number; echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	./main;	echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	echo; rm main; sleep 1;\
	done

times3: 
	@for number in 1 2 3 ; do \
    gcc -Wall -Wextra ./main.c -lm -o main;\
	echo; echo run number $$number; echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	./main;	echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	echo; rm main; sleep 1;\
	done
	