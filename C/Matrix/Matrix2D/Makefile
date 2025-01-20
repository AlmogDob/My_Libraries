temp: build_temp run_temp clean_temp  
	@echo temp done

build_temp: temp.c
	@gcc -Wall -Wextra ./temp.c -lm -o temp

run_temp:
	@echo
	@./temp

clean_temp:
	@echo
	rm temp

debug_build_temp: temp.c
	@gcc -Wall -g -Wextra ./temp.c -lm -o temp