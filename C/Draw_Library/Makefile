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
