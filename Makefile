
SRC_DIR = ./DataStructureAndAlgorithm/
BIN_DIR = ./bin/


SRC = $(wildcard $(SRC_DIR)*c)
BIN = $(patsubst %.c, $(BIN_DIR)%, $(notdir $(SRC)))

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm

.PHONY: all
all: $(BIN)
$(BIN_DIR)%: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(BIN)
