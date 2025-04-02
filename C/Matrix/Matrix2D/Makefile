CFLAGS = -Wall -Wextra -lm 
CCHECKS = -fsanitize=address

temp: build_temp run_temp clean_temp  
	@echo temp done

build_temp: temp.c
	@gcc ./temp.c $(CFLAGS) -o temp

run_temp:
	@echo
	@./temp

clean_temp:
	@echo
	rm temp

debug_build_temp: temp.c
	@gcc ./temp.c $(CFLAGS) -g -o temp
# valgrind -s --leak-check=full ./temp
