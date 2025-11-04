CCHECKS = -fsanitize=address
CWARNINGS = -Wall -Wextra -Wuninitialized 
CFLAGS = $(CWARNINGS) -lm -lSDL2 -lSDL2_ttf $(CCHECKS)

#############################################################
temp: build_temp run_temp clean_temp  
	@echo ./build/temp done

build_temp: ./src/temp.c 
	@echo [INFO] building temp
	@gcc ./src/temp.c $(CFLAGS) -o ./build/temp

run_temp:
	@echo
	./build/temp
	@echo

clean_temp:
	@echo [INFO] removing all build files
	@rm ./build/temp

debug_temp: debug_build_temp
	gdb ./build/temp

debug_build_temp: ./src/temp.c
	@echo [INFO] building temp
	@gcc ./src/temp.c $(CFLAGS) -ggdb -o ./build/temp

profile_temp: profile_build_temp
	./build/temp
	@echo
	gprof ./build/temp gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_temp

profile_build_temp: ./src/temp.c
	@echo [INFO] building temp
	@gcc ./src/temp.c $(CFLAGS) -p -ggdb -o ./build/temp

# valgrind -s --leak-check=full ./temp
# cloc --exclude-lang=JSON,make .

#############################################################
grid_example: build_grid_example run_grid_example clean_grid_example  
	@echo ./build/grid_example done

build_grid_example: ./src/grid_example.c 
	@echo [INFO] building grid_example
	@gcc ./src/grid_example.c $(CFLAGS) -o ./build/grid_example

run_grid_example:
	@echo
	./build/grid_example
	@echo

clean_grid_example:
	@echo [INFO] removing all build files
	@rm ./build/grid_example

debug_grid_example: debug_build_grid_example
	gdb ./build/grid_example

debug_build_grid_example: ./src/grid_example.c
	@echo [INFO] building grid_example
	@gcc ./src/grid_example.c $(CFLAGS) -ggdb -o ./build/grid_example

profile_grid_example: profile_build_grid_example
	./build/grid_example
	@echo
	gprof ./build/grid_example gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_grid_example

profile_build_grid_example: ./src/grid_example.c
	@echo [INFO] building grid_example
	@gcc ./src/grid_example.c $(CFLAGS) -p -ggdb -o ./build/grid_example

# valgrind -s --leak-check=full ./grid_example
# cloc --exclude-lang=JSON,make .

#############################################################
teapot_example: build_teapot_example run_teapot_example clean_teapot_example  
	@echo ./build/teapot_example done

build_teapot_example: ./src/teapot_example.c 
	@echo [INFO] building teapot_example
	@gcc ./src/teapot_example.c $(CFLAGS) -o ./build/teapot_example

run_teapot_example:
	@echo
	./build/teapot_example
	@echo

clean_teapot_example:
	@echo [INFO] removing all build files
	@rm ./build/teapot_example

debug_teapot_example: debug_build_teapot_example
	gdb ./build/teapot_example

debug_build_teapot_example: ./src/teapot_example.c
	@echo [INFO] building teapot_example
	@gcc ./src/teapot_example.c $(CFLAGS) -ggdb -o ./build/teapot_example

profile_teapot_example: profile_build_teapot_example
	./build/teapot_example
	@echo
	gprof ./build/teapot_example gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_teapot_example

profile_build_teapot_example: ./src/teapot_example.c
	@echo [INFO] building teapot_example
	@gcc ./src/teapot_example.c $(CFLAGS) -p -ggdb -o ./build/teapot_example

# valgrind -s --leak-check=full ./teapot_example
# cloc --exclude-lang=JSON,make .

################################################################

strip_comments_Engine: src/include/Almog_Engine.h
	gcc -fpreprocessed -dD -E -P src/include/Almog_Engine.h > striped_Almog_Engine.h


docs: doxygen.txt
	@if [$(wildcard docs)]; then\
		rm -r docs;\
	fi
	doxygen doxygen.txt
	cd docs/latex; make;
	cp docs/latex/refman.pdf .
	rm -r docs
