# MAIN PROGRAM:

# Object files
open.o: open_functions/open.c open_functions/open.h
	gcc -c open_functions/open.c -o open.o

Data_Structs.o: Data_Structs/Data_Structs.c Data_Structs/Data_Structs.h
	gcc -c Data_Structs/Data_Structs.c -o Data_Structs.o

threads.o: threads/threads.c threads/threads.h 
	gcc -c threads/threads.c -o threads.o 

threads_filt.o: threads_filtered/threads_filt.c threads_filtered/threads_filt.h 
	gcc -c threads_filtered/threads_filt.c -o threads_filt.o 

algorithms.o: algorithms/algorithms.c algorithms/algorithms.h 
	gcc -c algorithms/algorithms.c -o algorithms.o 

main.o: src/main.c open_functions/open.h algorithms/algorithms.h
	gcc -c src/main.c -o main.o 

# Executable
app: main.o open.o algorithms.o Data_Structs.o threads.o
	gcc main.o open.o algorithms.o Data_Structs.o threads.o -g -o app -lm -ggdb

# FILTERED MAIN PROGRAM:

# Object files
filtered_algorithms.o: algorithms/filtered_algorithms.c algorithms/filtered_algorithms.h 
	gcc -c algorithms/filtered_algorithms.c -o filtered_algorithms.o 

filtered_main.o: src/filtered_main.c open_functions/open.h algorithms/filtered_algorithms.h
	gcc -c src/filtered_main.c -o filtered_main.o 

# Executable
filtered_app: filtered_main.o open.o algorithms.o filtered_algorithms.o Data_Structs.o threads.o threads_filt.o
	gcc filtered_main.o open.o algorithms.o filtered_algorithms.o Data_Structs.o threads.o threads_filt.o -g -o filtered_app -lm -ggdb


# STITCHED MAIN PROGRAM:
stitched_main.o: src/stitched_main.c open_functions/open.h algorithms/filtered_algorithms.h
	gcc -c src/stitched_main.c -o stitched_main.o 

# Executable
stitched_app: stitched_main.o open.o algorithms.o filtered_algorithms.o Data_Structs.o threads.o threads_filt.o
	gcc stitched_main.o open.o algorithms.o filtered_algorithms.o Data_Structs.o threads.o threads_filt.o -g -o stitched_app -lm -ggdb


# UNIT TESTS:

# Object files
testing.o: unit_tests/testing.c unit_tests/acutest.h open_functions/open.h algorithms/algorithms.h algorithms/filtered_algorithms.h
	gcc -c unit_tests/testing.c -o testing.o
# Executable
test: testing.o open.o algorithms.o filtered_algorithms.o Data_Structs.o threads.o threads_filt.o
	gcc testing.o open.o algorithms.o filtered_algorithms.o Data_Structs.o threads.o threads_filt.o -g -o test -lm

# Clean target
clean:
	rm -f *.o app test

# Run the program
run_vamana_main: app
	./app -k 100 -L 125  -R 40 -vamana main -index_fname vamana_index -threads 5

run_vamana_random: app
	./app -k 100 -L 125  -R 40 -vamana random -index_fname vamana_index_r -threads 5

run_vamana_semi_random: app
	./app -k 100 -L 125  -R 40 -vamana semi_random -index_fname vamana_index_sr -threads 5

# Run the filtered program
run_filter_yes: filtered_app
	./filtered_app -k 100 -L 125  -R 40 -filtered yes -index_fname filtered_vamana_index_10k -random no -dataset 10k -threads 5

run_filter_no: filtered_app
	./filtered_app -k 100 -L 125  -R 40 -filtered no -index_fname filtered_vamana_index_10k -random no -dataset 10k -threads 5

run_filter_yes_random: filtered_app
	./filtered_app -k 100 -L 125  -R 40 -filtered yes -index_fname filtered_vamana_index_r_10k -random yes -dataset 10k -threads 5

run_filter_no_random: filtered_app
	./filtered_app -k 100 -L 125  -R 40 -filtered no -index_fname filtered_vamana_index_r_10k -random yes -dataset 10k -threads 5

# Run the stitched program
run_stitch: stitched_app
	./stitched_app -k 100 -L 125  -R 40 -filtered yes -index_fname stitched_vamana_index_10k -random no -dataset 10k -vmn_threads 0 -vamana main -stitch_threads 5

run_stitch_no_filter: stitched_app
	./stitched_app -k 100 -L 125  -R 40 -filtered no -index_fname stitched_vamana_index_10k -random no -dataset 10k -vmn_threads 0 -vamana main -stitch_threads 8

run_stitch_random: stitched_app
	./stitched_app -k 100 -L 125  -R 40 -filtered yes -index_fname stitched_vamana_index_r_10k -random yes -dataset 10k -vmn_threads 0 -vamana main -stitch_threads 8

run_stitch_no_filter_random: stitched_app
	./stitched_app -k 100 -L 125  -R 40 -filtered no -index_fname stitched_vamana_index_r_10k -random yes -dataset 10k -vmn_threads 0 -vamana main -stitch_threads 8

# Run the unit tests
run_test: test
	./test 