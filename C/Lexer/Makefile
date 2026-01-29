CCHECKS = -fsanitize=address,leak,undefined,bounds -fsanitize-address-use-after-scope
CWARNINGS = -W -Wall -Wextra -Wuninitialized -Wunreachable-code
CFLAGS = $(CWARNINGS) $(CCHECKS) -std=c99

#############################################################

BIN_DIR  := ./build
SRC_DIR  := .

.PHONY: temp
temp: SRC_LIST := $(SRC_DIR)/temp.c
temp: build run clean

.PHONY: tests
tests: SRC_LIST := $(SRC_DIR)/tests.c
tests: build run clean

.PHONY: all
all: SRC_LIST := $(SRC_DIR)/tests.c $(SRC_DIR)/temp.c
all: build run clean

#-----------------
.PHONY: build 
build: 
	@mkdir -p $(BIN_DIR)
	@$(foreach src,$(SRC_LIST), \
		echo "[INFO] building '$(src)'"; \
		gcc '$(src)' $(CFLAGS) -o '$(BIN_DIR)/$(notdir $(basename $(src)))'; \
	)

.PHONY: run 
run:
	@$(foreach src,$(SRC_LIST), \
		echo; echo [INFO] $(BIN_DIR)/$(notdir $(basename $(src))):;  $(BIN_DIR)/$(notdir $(basename $(src))); echo; \
	)

.PHONY: clean 
clean:
	@echo [INFO] removing build files
	@if [ -n "$(BIN_DIR)" ] && [ "$(BIN_DIR)" != "." ]; then \
		echo "[INFO] removing directory '$(BIN_DIR)'"; \
		rm -r "$(BIN_DIR)"; \
	else \
		$(foreach src,$(SRC_LIST), \
			rm $(BIN_DIR)/$(notdir $(basename $(src))); \
		) \
	fi

#############################################################

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

strip_comments_Almog_Lexer: ./Almog_Lexer.h
	gcc -fpreprocessed -dD -E -P ./Almog_Lexer.h > ./striped_Almog_Lexer.h


docs: doxygen.txt
	@if [$(wildcard docs)]; then\
		rm -r docs;\
	fi
	doxygen doxygen.txt
	cd docs/latex; make;
	cp docs/latex/refman.pdf .
	rm -r docs

