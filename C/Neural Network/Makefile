fast: build run clean
	
build: twice.c
	gcc -Wall -Wextra ./twice.c -lm -o twice

run:
	@echo
	@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	./twice
	@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@echo

clean:
	rm twice

test:
	gcc -Wall -Wextra ./test.c -lm -o test
	./test
	rm test

times2: 
	@for number in 1 2 ; do \
    gcc -Wall -Wextra ./twice.c -lm -o twice;\
	echo; echo run number $$number; echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	./twice;	echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	echo; rm twice; sleep 1;\
	done

times3: 
	@for number in 1 2 3 ; do \
    gcc -Wall -Wextra ./twice.c -lm -o twice;\
	echo; echo run number $$number; echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	./twice;	echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	echo; rm twice; sleep 1;\
	done
	