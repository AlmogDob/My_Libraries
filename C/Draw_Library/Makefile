CCHECKS = -fsanitize=address
CWARNINGS = -Wall -Wextra -Wuninitialized 
CFLAGS = $(CWARNINGS) -lm -lSDL2 -lSDL2_ttf

temp: build_temp run_temp clean_temp  
	@echo [INFO] done

build_temp: temp.c
	@echo [INFO] building
	@gcc ./temp.c $(CFLAGS) -o temp

run_temp:
	@echo
	./temp

clean_temp:
	@echo
	rm temp

debug_temp: debug_build_temp
	gdb ./temp

debug_build_temp: ./temp.c
	@echo [INFO] building temp
	gcc ./temp.c $(CFLAGS) -ggdb -o ./temp

# valgrind -s --leak-check=full ./temp
# cloc --exclude-lang=JSON,make .

############################################################
example1: build_example1 run_example1 clean_example1  
	@echo [INFO] done

build_example1: example1.c
	@echo [INFO] building
	@gcc ./example1.c $(CFLAGS) -o example1

run_example1:
	@echo
	./example1

clean_example1:
	@echo
	rm example1

debug_example1: debug_build_example1
	gdb ./example1

debug_build_example1: ./example1.c
	@echo [INFO] building example1
	gcc ./example1.c $(CFLAGS) -ggdb -o ./example1

########################################
strip_comments_Almog_Draw_Library.h: ./Almog_Draw_Library.h
	gcc -fpreprocessed -dD -E -P ./Almog_Draw_Library.h > striped_Almog_Draw_Library.h