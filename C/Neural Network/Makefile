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

gates_times3: 
	@for number in 1 2 3 ; do \
    gcc -Wall -Wextra ./gates.c -lm -o gates;\
	echo; echo run number $$number; echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	./gates;	echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	echo; rm gates; sleep 1;\
	done
############################################################

nn: build_nn run_nn clean_nn
	@echo done nn

build_nn: nn.c
	gcc -Wall -Wextra ./nn.c -lm -o nn

run_nn:
	./nn

clean_nn:
	rm nn

nn_times3: 
	@for number in 1 2 3 ; do \
    gcc -Wall -Wextra ./nn.c -lm -o nn;\
	echo; echo run number $$number; echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	./nn;	echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	echo; rm nn; sleep 1;\
	done
############################################################

xor: build_xor run_xor clean_xor
	@echo done xor

build_xor: xor.c
	gcc -Wall -Wextra ./xor.c -lm -o xor

run_xor:
	./xor

clean_xor:
	rm xor

xor_times3: 
	@for number in 1 2 3 ; do \
    gcc -Wall -Wextra ./xor.c -lm -o xor;\
	echo; echo run number $$number; echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	./xor;	echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;\
	echo; rm xor; sleep 1;\
	done
############################################################

test:
	gcc -Wall -Wextra ./test.c -lm -o test
	./test
	rm test
	