test: build_test run_test clean_test  
	@echo test done
build_test: test.c
	gcc -g -Wall -Wextra ./test.c -o test

run_test:
	./test $(IN_DIR) $(MESH_DIR)

clean_test:
	rm test

##########################################################

docs: doxygen.txt
	@if [$(wildcard docs)]; then\
		rm -r docs;\
	fi
	doxygen doxygen.txt
	cd docs/latex; make;
	cp docs/latex/refman.pdf .
	rm -r docs