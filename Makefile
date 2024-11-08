BIN_DIR = bin
BUILD_DIR = build
INCLUDE_DIR = include
SRC_DIR = lib

all: main

main: utils.o
	gcc -std=c11 -Wall -Wextra -Werror -lm -I $(INCLUDE_DIR) -c main.c -o $(BUILD_DIR)/main.o
	gcc -std=c11 -Wall -Wextra -Werror -lm -I $(INCLUDE_DIR) -o $(BIN_DIR)/main $(BUILD_DIR)/*

utils.o:
	gcc -std=c11 -Wall -Wextra -Werror -I $(INCLUDE_DIR) -c $(SRC_DIR)/utils.c -o $(BUILD_DIR)/utils.o
