CCHECKS = -fsanitize=address,leak,undefined,bounds -fsanitize-address-use-after-scope
CWARNINGS = -W -Wall -Wextra -Wuninitialized -Wunreachable-code
CFLAGS = $(CWARNINGS) $(CCHECKS) -std=c99 -lm
# CFLAGS = $(CWARNINGS) -lm -O3

#############################################################

examples: example1

#############################################################

temp: build_temp run_temp clean_temp  
	@echo ./temp done

build_temp: ./temp.c 
	@echo [INFO] building temp
	@gcc ./temp.c $(CFLAGS) -o ./temp

run_temp:
	@echo
	./temp
	@echo

clean_temp:
	@echo [INFO] removing all build files
	@rm ./temp

debug_temp: debug_build_temp
	gdb ./temp

debug_build_temp: ./temp.c
	@echo [INFO] building temp
	@gcc ./temp.c $(CFLAGS) -ggdb -o ./temp

profile_temp: profile_build_temp
	./temp
	@echo
	gprof ./temp gmon.out > profiler.txt
	gprof ./temp gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png profiler.txt 
	make clean_temp

profile_build_temp: ./temp.c
	@echo [INFO] building temp
	@gcc ./temp.c $(CFLAGS) -p -ggdb -o ./temp

# valgrind -s --leak-check=full ./temp
# cloc --exclude-lang=JSON,make .

#############################################################

example1: build_example1 run_example1 clean_example1  
	@echo ./examples/example1 done

build_example1: ./examples/example1.c 
	@echo [INFO] building ./examples/example1
	@gcc ./examples/example1.c $(CFLAGS) -o ./examples/example1

run_example1:
	@echo
	./examples/example1
	@echo

clean_example1:
	@echo [INFO] removing all build files
	@rm ./examples/example1

debug_example1: debug_build_example1
	gdb ./examples/example1

debug_build_example1: ./example1.c
	@echo [INFO] building ./examples/example1
	@gcc ./examples/example1.c $(CFLAGS) -ggdb -o ./examples/example1

profile_example1: profile_build_example1
	./examples/example1
	@echo
	gprof ./examples/example1 gmon.out > profiler.txt
	gprof ./examples/example1 gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png profiler.txt 
	make clean_example1

profile_build_example1: ./examples/example1.c
	@echo [INFO] building example1
	@gcc ./examples/example1.c $(CFLAGS) -p -ggdb -o ./examples/example1

#############################################################

strip_comments_Matrix2D: ./Matrix2D.h
	gcc -fpreprocessed -dD -E -P ./Matrix2D.h > ./striped_Matrix2D.h


docs: doxygen.txt
	@if [$(wildcard docs)]; then\
		rm -r docs;\
	fi
	doxygen doxygen.txt
	cd docs/latex; make;
	cp docs/latex/refman.pdf .
	rm -r docs

#############################################################
# By AI
# Matrix2D tests

TEST_BIN = ./test_matrix2d
TEST_SRC = ./test_matrix2d.c

test_matrix2d: build_test_matrix2d run_test_matrix2d clean_test_matrix2d
	@echo ./test_matrix2d done

build_test_matrix2d: $(TEST_SRC) ./Matrix2D.h
	@echo [INFO] building Matrix2D tests
	@gcc -I. $(TEST_SRC) $(CFLAGS) -ggdb -o $(TEST_BIN)

run_test_matrix2d:
	@echo
	@$(TEST_BIN)
	@echo

clean_test_matrix2d:
	@echo [INFO] removing Matrix2D test binary
	@rm -f $(TEST_BIN)


#############################################################
# By AI
# Matrix2D coverage (gcov/gcovr)
#
# Notes:
#  - Coverage and sanitizers can be used together, but it’s often cleaner to
#    disable sanitizers for coverage builds to avoid noisy/slow runs.
#  - Requires: gcovr (pip install gcovr / apt install gcovr)

COV_DIR     = ./coverage
TEST_COV_BIN = ./test_matrix2d_cov
TEST_COV_OBJ = $(COV_DIR)/test_matrix2d_cov.o

# Coverage build flags (intentionally no sanitizers here)
COVFLAGS = -O0 -g --coverage
COV_CFLAGS = $(CWARNINGS) $(COVFLAGS) -std=c99 -lm

.PHONY: coverage_matrix2d build_cov_matrix2d run_cov_matrix2d report_cov_matrix2d clean_cov_matrix2d

coverage_matrix2d: clean_cov_matrix2d build_cov_matrix2d run_cov_matrix2d report_cov_matrix2d
	@echo $(TEST_COV_BIN) coverage done
	xdg-open coverage/index.html
	make clean_cov_matrix2d

build_cov_matrix2d: $(TEST_SRC) ./Matrix2D.h
	@echo [INFO] building Matrix2D coverage binary
	@mkdir -p $(COV_DIR)
	@gcc -I. $(TEST_SRC) $(COV_CFLAGS) -c -o $(TEST_COV_OBJ)
	@gcc $(TEST_COV_OBJ) $(COV_CFLAGS) -o $(TEST_COV_BIN)

run_cov_matrix2d:
	@echo
	@$(TEST_COV_BIN)
	@echo

report_cov_matrix2d:
	@echo [INFO] generating coverage report into $(COV_DIR)
	@mkdir -p $(COV_DIR)
	@gcovr -r . --html --html-details -o $(COV_DIR)/index.html
	@gcovr -r . --txt -o $(COV_DIR)/summary.txt
	@echo [INFO] generating function summary via gcov
	@gcov -b -c -f -o $(COV_DIR) $(TEST_COV_OBJ) > $(COV_DIR)/gcov_functions_raw.txt
	@grep -E '^function ' $(COV_DIR)/gcov_functions_raw.txt \
		> $(COV_DIR)/functions.txt || true
	@echo [INFO] function summary: $(COV_DIR)/functions.txt
	@echo [INFO] report: $(COV_DIR)/index.html

clean_cov_matrix2d:
	@echo [INFO] cleaning coverage artifacts
	@rm -f $(TEST_COV_BIN)
	@rm -f ./*.gcda ./*.gcno ./*.gcov
	@rm -rf $(COV_DIR)