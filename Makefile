# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude

# Source and object files
SRC = src
OBJ = build
INCLUDE = include
BIN = build/cpu_simulator

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

# Default target: Build the simulator
all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ)/*.o $(BIN)

# Test target (optional)
test:
	$(CC) $(CFLAGS) -o $(OBJ)/test_runner test/*.c $(SRCS)
	$(OBJ)/test_runner

# Debugging build
debug: CFLAGS += -DDEBUG
debug: all

# Run target for convenience
run: $(BIN)
	$(BIN) programs/bin/sample_program.bin
