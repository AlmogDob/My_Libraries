CCHECKS = -fsanitize=address,leak,undefined,bounds -fsanitize-address-use-after-scope
CWARNINGS = -W -Wall -Wextra -Wuninitialized -Wunreachable-code
CFLAGS = $(CWARNINGS) $(CCHECKS) -std=c99 -lm
# CFLAGS = $(CWARNINGS) -lm -O3

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
