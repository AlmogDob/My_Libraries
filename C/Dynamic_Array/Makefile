test: build_test run_test clean_test  
	@echo test done
build_test: test.c
	gcc -g -Wall -Wextra ./test.c -o test

run_test:
	./test $(IN_DIR) $(MESH_DIR)

clean_test:
	rm test

##########################################################