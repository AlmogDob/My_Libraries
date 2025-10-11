CCHECKS = -fsanitize=address
CWARNINGS = -Wall -Wextra -Wuninitialized 
CFLAGS = $(CWARNINGS) -lm -lSDL2 -lSDL2_ttf

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
example1: build_example1 run_example1 clean_example1  
	@echo ./build/example1 done

build_example1: ./src/example1.c 
	@echo [INFO] building example1
	@gcc ./src/example1.c $(CFLAGS) -o ./build/example1

run_example1:
	@echo
	./build/example1
	@echo

clean_example1:
	@echo [INFO] removing all build files
	@rm ./build/example1

debug_example1: debug_build_example1
	gdb ./build/example1

debug_build_example1: ./src/example1.c
	@echo [INFO] building example1
	@gcc ./src/example1.c $(CFLAGS) -ggdb -o ./build/example1

profile_example1: profile_build_example1
	./build/example1
	@echo
	gprof ./build/example1 gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_example1

profile_build_example1: ./src/example1.c
	@echo [INFO] building example1
	@gcc ./src/example1.c $(CFLAGS) -p -ggdb -o ./build/example1

# valgrind -s --leak-check=full ./example1
# cloc --exclude-lang=JSON,make .

#############################################################
example2: build_example2 run_example2 clean_example2  
	@echo ./build/example2 done

build_example2: ./src/example2.c 
	@echo [INFO] building example2
	@gcc ./src/example2.c $(CFLAGS) -o ./build/example2

run_example2:
	@echo
	./build/example2
	@echo

clean_example2:
	@echo [INFO] removing all build files
	@rm ./build/example2

debug_example2: debug_build_example2
	gdb ./build/example2

debug_build_example2: ./src/example2.c
	@echo [INFO] building example2
	@gcc ./src/example2.c $(CFLAGS) -ggdb -o ./build/example2

profile_example2: profile_build_example2
	./build/example2
	@echo
	gprof ./build/example2 gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_example2

profile_build_example2: ./src/example2.c
	@echo [INFO] building example2
	@gcc ./src/example2.c $(CFLAGS) -p -ggdb -o ./build/example2

# valgrind -s --leak-check=full ./example2
# cloc --exclude-lang=JSON,make .