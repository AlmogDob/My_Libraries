CCHECKS = -fsanitize=address
CWARNINGS = -Wall -Wextra -Wuninitialized 
CFLAGS = $(CWARNINGS) -lm -lSDL2 -lSDL2_ttf -O3

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
sphere_circle_example: build_sphere_circle_example run_sphere_circle_example clean_sphere_circle_example  
	@echo ./build/sphere_circle_example done

build_sphere_circle_example: ./src/sphere_circle_example.c 
	@echo [INFO] building sphere_circle_example
	@gcc ./src/sphere_circle_example.c $(CFLAGS) -o ./build/sphere_circle_example

run_sphere_circle_example:
	@echo
	./build/sphere_circle_example
	@echo

clean_sphere_circle_example:
	@echo [INFO] removing all build files
	@rm ./build/sphere_circle_example

debug_sphere_circle_example: debug_build_sphere_circle_example
	gdb ./build/sphere_circle_example

debug_build_sphere_circle_example: ./src/sphere_circle_example.c
	@echo [INFO] building sphere_circle_example
	@gcc ./src/sphere_circle_example.c $(CFLAGS) -ggdb -o ./build/sphere_circle_example

profile_sphere_circle_example: profile_build_sphere_circle_example
	./build/sphere_circle_example
	@echo
	gprof ./build/sphere_circle_example gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_sphere_circle_example

profile_build_sphere_circle_example: ./src/sphere_circle_example.c
	@echo [INFO] building sphere_circle_example
	@gcc ./src/sphere_circle_example.c $(CFLAGS) -p -ggdb -o ./build/sphere_circle_example

# valgrind -s --leak-check=full ./sphere_circle_example
# cloc --exclude-lang=JSON,make .
