#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

float** fvecs_open(const char *filename, int* num_vectors, int* d);

int** ivecs_open(const char *filename, int* num_vectors, int* d);

void free_matrix_ivecs(int** matrix, int d);

void free_matrix_fvecs(float** matrix, int d);

float** data_open(const char* filename, int* num_vectors, int vec_num_d);