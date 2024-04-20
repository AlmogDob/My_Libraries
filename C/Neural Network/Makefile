twice: build_twice run_twice clean_twice
	@echo done twice

build_twice: twice.c
	gcc -Wall -Wextra ./twice.c -lm -o twice

run_twice:
	@echo
	@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	./twice
	@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@echo

clean_twice:
	rm twice

############################################################
gates: build_gates run_gates clean_gates
	@echo done gates

build_gates: gates.c
	gcc -Wall -Wextra ./gates.c -lm -o gates

run_gates:
	./gates

clean_gates:
	rm gates

############################################################

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

gates_times3: 
	@for number in 1 2 3 ; do \
    gcc -Wall -Wextra ./gates.c -lm -o gates;\
	echo; echo run number $$number; echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	./gates;	echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	echo; rm gates; sleep 1;\
	done
	