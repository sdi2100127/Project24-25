#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Data_Structs/Data_Structs.h"

float** fvecs_open(const char *filename, int* num_vectors, int* d);

int** ivecs_open(const char *filename, int* num_vectors, int* d);

void free_matrix_ivecs(int** matrix, int d);

void free_matrix_fvecs(float** matrix, int d);

float** data_open(const char* filename, int* num_vectors, int vec_num_d, float* min_f, float* max_f, Set* filters);

float** query_open(const char* filename, int* num_q, int vec_num_d , int * count);