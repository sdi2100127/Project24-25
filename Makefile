# MAIN PROGRAM:

# Object files
open.o: open_functions/open.c open_functions/open.h
	gcc -c open_functions/open.c -o open.o

Data_Structs.o: Data_Structs/Data_Structs.c Data_Structs/Data_Structs.h
	gcc -c Data_Structs/Data_Structs.c -o Data_Structs.o

algorithms.o: algorithms/algorithms.c algorithms/algorithms.h 
	gcc -c algorithms/algorithms.c -o algorithms.o 

main.o: src/main.c open_functions/open.h algorithms/algorithms.h
	gcc -c src/main.c -o main.o 

# Executable
app: main.o open.o algorithms.o Data_Structs.o
	gcc main.o open.o algorithms.o Data_Structs.o -o app -lm -ggdb

# UNIT TESTS:

# Object files
testing.o: unit_tests/testing.c unit_tests/acutest.h open_functions/open.h algorithms/algorithms.h
	gcc -c unit_tests/testing.c -o testing.o
# Executable
test: testing.o open.o algorithms.o Data_Structs.o
	gcc testing.o open.o algorithms.o Data_Structs.o -o test -lm

# Clean target
clean:
	rm -f *.o app test

# Run the program
run: app
	./app

# Run the unit tests
run_test: test
	./test