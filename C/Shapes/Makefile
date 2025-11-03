CCHECKS = -fsanitize=address,leak,undefined,bounds -fsanitize-address-use-after-scope
CWARNINGS = -W -Wall -Wextra -Wuninitialized -Wunreachable-code
CFLAGS = $(CWARNINGS) -lm -lSDL2 -lSDL2_ttf 

#############################################################

examples: example1 example2 example3 example4 example5

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
	gprof ./build/temp gmon.out > profiler.txt
	gprof ./build/temp gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png profiler.txt 
	make clean_temp

profile_build_temp: ./src/temp.c
	@echo [INFO] building temp
	@gcc ./src/temp.c $(CFLAGS) -p -ggdb -o ./build/temp

# valgrind -s --leak-check=full ./temp
# cloc --exclude-lang=JSON,make .

#############################################################

example1: build_example1 run_example1 clean_example1  
	@echo ./build/example1 done
	@echo --------------------

build_example1: ./src/examples/example1.c 
	@echo [INFO] building example1
	@gcc ./src/examples/example1.c $(CFLAGS) -o ./build/example1

run_example1:
	@echo
	./build/example1
	@echo

clean_example1:
	@echo [INFO] removing all build files
	@rm ./build/example1

debug_example1: debug_build_example1
	gdb ./build/example1

debug_build_example1: ./src/examples/example1.c
	@echo [INFO] building example1
	@gcc ./src/examples/example1.c $(CFLAGS) -ggdb -o ./build/example1

profile_example1: profile_build_example1
	./build/example1
	@echo
	gprof ./build/example1 gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_example1

profile_build_example1: ./src/examples/example1.c
	@echo [INFO] building example1
	@gcc ./src/examples/example1.c $(CFLAGS) -p -ggdb -o ./build/example1

# valgrind -s --leak-check=full ./example1
# cloc --exclude-lang=JSON,make .

#############################################################
example2: build_example2 run_example2 clean_example2  
	@echo ./build/example2 done
	@echo --------------------

build_example2: ./src/examples/example2.c 
	@echo [INFO] building example2
	@gcc ./src/examples/example2.c $(CFLAGS) -o ./build/example2

run_example2:
	@echo
	./build/example2
	@echo

clean_example2:
	@echo [INFO] removing all build files
	@rm ./build/example2

debug_example2: debug_build_example2
	gdb ./build/example2

debug_build_example2: ./src/examples/example2.c
	@echo [INFO] building example2
	@gcc ./src/examples/example2.c $(CFLAGS) -ggdb -o ./build/example2

profile_example2: profile_build_example2
	./build/example2
	@echo
	gprof ./build/example2 gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_example2

profile_build_example2: ./src/examples/example2.c
	@echo [INFO] building example2
	@gcc ./src/examples/example2.c $(CFLAGS) -p -ggdb -o ./build/example2

# valgrind -s --leak-check=full ./example2
# cloc --exclude-lang=JSON,make .

#############################################################
example3: build_example3 run_example3 clean_example3  
	@echo ./build/example3 done
	@echo --------------------

build_example3: ./src/examples/example3.c 
	@echo [INFO] building example3
	@gcc ./src/examples/example3.c $(CFLAGS) -o ./build/example3

run_example3:
	@echo
	./build/example3
	@echo

clean_example3:
	@echo [INFO] removing all build files
	@rm ./build/example3

debug_example3: debug_build_example3
	gdb ./build/example3

debug_build_example3: ./src/examples/example3.c
	@echo [INFO] building example3
	@gcc ./src/examples/example3.c $(CFLAGS) -ggdb -o ./build/example3

profile_example3: profile_build_example3
	./build/example3
	@echo
	gprof ./build/example3 gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_example3

profile_build_example3: ./src/examples/example3.c
	@echo [INFO] building example3
	@gcc ./src/examples/example3.c $(CFLAGS) -p -ggdb -o ./build/example3

# valgrind -s --leak-check=full ./example3
# cloc --exclude-lang=JSON,make .

#############################################################
example4: build_example4 run_example4 clean_example4  
	@echo ./build/example4 done
	@echo --------------------

build_example4: ./src/examples/example4.c 
	@echo [INFO] building example4
	@gcc ./src/examples/example4.c $(CFLAGS) -o ./build/example4

run_example4:
	@echo
	./build/example4
	@echo

clean_example4:
	@echo [INFO] removing all build files
	@rm ./build/example4

debug_example4: debug_build_example4
	gdb ./build/example4

debug_build_example4: ./src/examples/example4.c
	@echo [INFO] building example4
	@gcc ./src/examples/example4.c $(CFLAGS) -ggdb -o ./build/example4

profile_example4: profile_build_example4
	./build/example4
	@echo
	gprof ./build/example4 gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_example4

profile_build_example4: ./src/examples/example4.c
	@echo [INFO] building example4
	@gcc ./src/examples/example4.c $(CFLAGS) -p -ggdb -o ./build/example4

# valgrind -s --leak-check=full ./example4
# cloc --exclude-lang=JSON,make .

#############################################################
example5: build_example5 run_example5 clean_example5  
	@echo ./build/example5 done
	@echo --------------------

build_example5: ./src/examples/example5.c 
	@echo [INFO] building example5
	@gcc ./src/examples/example5.c $(CFLAGS) -o ./build/example5

run_example5:
	@echo
	./build/example5
	@echo

clean_example5:
	@echo [INFO] removing all build files
	@rm ./build/example5

debug_example5: debug_build_example5
	gdb ./build/example5

debug_build_example5: ./src/examples/example5.c
	@echo [INFO] building example5
	@gcc ./src/examples/example5.c $(CFLAGS) -ggdb -o ./build/example5

profile_example5: profile_build_example5
	./build/example5
	@echo
	gprof ./build/example5 gmon.out | /home/almog/.local/bin/gprof2dot | dot -Tpng -Gdpi=200 -o output.png
	imview ./output.png
	# xdg-open ./output.png
	@echo
	rm ./gmon.out ./output.png 
	make clean_example5

profile_build_example5: ./src/examples/example5.c
	@echo [INFO] building example5
	@gcc ./src/examples/example5.c $(CFLAGS) -p -ggdb -o ./build/example5

# valgrind -s --leak-check=full ./example5
# cloc --exclude-lang=JSON,make .