#include "open.h"

// mike path: /home/mike/Documents/Project24-25/open_functions/siftsmall
// nefeli path: /home/users/sdi2100127/Documents/Project24-25/open_functions/siftsmall

float** fvecs_open(const char* filename, int* num_vectors, int* d) {
    // first we have to open the file in binary read mode using the correct file path
    FILE *fp = NULL;
    char path[100];
    sprintf(path, "/home/mike/Documents/Project24-25/open_functions/siftsmall/%s", filename);
    fp = fopen(path, "rb");

    if (fp == NULL) {
        perror("error opening file");
        exit(EXIT_FAILURE);
    }

    // read the dimention off of the first 4 bytes of the file
    int dim;
    fread(&dim, sizeof(int), 1, fp);
    printf("dimention: %d ", dim);
    *d = dim;
    
    // the size of the vectors is given by: 4(sizeof(int))+dim*4(sizeof(float))
    int vec_size = sizeof(int) + dim * sizeof(float);
    printf("vector size: %d ", vec_size);
    
    // now we move the pointer to the end of the file
    fseek(fp, 0, SEEK_END);
    // then we retrieve its position to determine the total size of the file 
    long file_size = ftell(fp);
    fseek(fp, sizeof(int), SEEK_SET); // and we go back to the start

    // now that we know the size of the file we can calculate the number of vectors included
    int vecs = file_size / vec_size;
    printf("total num of vectors: %d ", vecs);
    *num_vectors = vecs;

    // allocate memory for the matrix of vectors and fill it
    // every column will represent a vector and the array will be of form dim x vecs

    // i = rows/components , j = columns/vectors !!!!!!!!
    // each column represents a different vector and each row a different component of each vector

    float** vectors = (float**)malloc(dim * sizeof(float*));
    for (int i = 0; i < dim; i++) {
        vectors[i] = (float*)malloc(vecs * sizeof(float));
    }

    for (int j = 0; j < vecs; j++) {
        // Read each vector and copy it
        float* vec = (float*)malloc(vec_size); 
        fread(vec, vec_size, 1, fp);
        
        for (int i = 0; i < dim; i++) {
            vectors[i][j] = vec[i + 1]; // vec[0] is the dimension, vec[1] to vec[d] are the vector components
        }
        
        free(vec);
    }

    fclose(fp);
    return vectors;
}

int** ivecs_open(const char* filename, int* num_vectors, int* d) {
    // first we have to open the file in binary read mode using the correct file path
    FILE *fp = NULL;
    char path[100];
    sprintf(path, "/home/mike/Documents/Project24-25/open_functions/siftsmall/%s", filename);
    fp = fopen(path, "rb");

    if (fp == NULL) {
        perror("error opening file");
        exit(EXIT_FAILURE);
    }

    // read the dimention off of the first 4 bytes of the file
    // int this case the dimention is the number k of the nearest neighbours
    int k;
    fread(&k, sizeof(int), 1, fp);
    printf("k: %d ", k);
    *d = k;
    
    // the size of the vectors is given by: 4(sizeof(int))+dim*4(sizeof(int))
    int vec_size = sizeof(int) + k * sizeof(int);
    printf("vector size: %d ", vec_size);
    
    // now we move the pointer to the end of the file
    fseek(fp, 0, SEEK_END);
    // then we retrieve its position to determine the total size of the file 
    long file_size = ftell(fp);
    fseek(fp, sizeof(int), SEEK_SET); // and we go back to the start

    // now that we know the size of the file we can calculate the number of query vectors included
    int vecs = file_size / vec_size;
    printf("total num of vectors: %d ", vecs);
    *num_vectors = vecs;

    // allocate memory for the matrix of vectors and fill it
    // every column will represent a query vector and 
    // each row represents the k nearest neighbours based on Euclidean distance of each  query vector

    // i = rows , j = columns !!!!!!!!

    int** vectors = (int**)malloc(k * sizeof(int*));
    for (int i = 0; i < k; i++) {
        vectors[i] = (int*)malloc(vecs * sizeof(int));
    }

    for (int j = 0; j < vecs; j++) {
        // Read each vector and copy it
        int* vec = (int*)malloc(vec_size); 
        fread(vec, vec_size, 1, fp);
        
        for (int i = 0; i < k; i++) {
            vectors[i][j] = vec[i + 1]; // vec[0] is the dimension, vec[1] to vec[d] are the vector components
        }
        
        free(vec);
    }


    fclose(fp);
    return vectors;
}

void free_matrix_fvecs(float** matrix, int d) {
    // Free the allocated memory for the matrix
    for (int i = 0; i < d; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void free_matrix_ivecs(int** matrix, int d) {
    // Free the allocated memory for the matrix
    for (int i = 0; i < d; i++) {
        free(matrix[i]);
    }
    free(matrix);
}