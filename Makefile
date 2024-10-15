SRC_DIR = open_functions

# Object files
open.o: $(SRC_DIR)/open.c $(SRC_DIR)/open.h
	gcc -c $(SRC_DIR)/open.c -o open.o

# Executable
open: open.o
	gcc open.o -o open