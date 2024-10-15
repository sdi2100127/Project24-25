#include "open.h"

float** fvecs_open(const char* filename, int* num_vectors) {
    // first we have to open the file in binary read mode using the correct file path
    FILE *fp = NULL;
    char path[100];
    sprintf(path, "/home/users/sdi2100127/Documents/Project24-25/open_functions/siftsmall/%s", filename);
    fp = fopen(path, "rb");

    if (fp == NULL) {
        perror("error opening file");
        exit(EXIT_FAILURE);
    }

    // read the dimention off of the first 4 bytes of the file
    int dim;
    fread(&dim, sizeof(int), 1, fp);
    printf("dimention: %dim\n", dim);
    
    // the size of the vectors is given by: 4(sizeof(int))+dim*4(sizeof(float))
    int vec_size = sizeof(int) + dim * sizeof(float);
    printf("vector size: %d\n", vec_size);
    
    // now we move the pointer to the end of the file
    fseek(fp, 0, SEEK_END);
    // then we retrieve its position to determine the total size of the file 
    long file_size = ftell(fp);
    fseek(fp, sizeof(int), SEEK_SET); // and we go back to the start

    // now that we know the size of the file we can calculate the number of vectors included
    int vecs = (file_size - sizeof(int)) / vec_size;
    printf("total num of vectors: %d\n", vecs);
    *num_vectors = vecs;

    // allocate memory for the matrix of vectors and fill it
    // every column will represent a vector and the array will be of form dim x vecs

    // i = columns , j = rows !!!!!!!!

    float** vectors = (float**)malloc(vecs * sizeof(float*));
    for (int i = 0; i < vecs; i++) {
        vectors[i] = (float*)malloc(dim * sizeof(float));
    }

    for (int i = 0; i < vecs; i++) {
        // Read each vector and copy it
        float* vec = (float*)malloc(vec_size); 
        fread(vec, vec_size, 1, fp);
        
        for (int j = 0; j < dim; j++) {
            vectors[i][j] = vec[j + 1]; // vec[0] is the dimension, vec[1] to vec[d] are the vector components
        }
        
        free(vec);
    }

    fclose(fp);
    return vectors;
}

void free_matrix(float** matrix, int num_vectors) {
    // Free the allocated memory for the matrix
    for (int i = 0; i < num_vectors; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    const char* filename = "siftsmall_base.fvecs"; // Specify your fvecs file
    int num_vectors;
    float** vectors = fvecs_open(filename, &num_vectors);
    
    if (vectors == NULL) {
        return 1; // Exit if the file reading failed
    }
    
    // Print the number of vectors and the first few for verification
    for (int i = 0; i<num_vectors && i < 20; i++) {
        printf("Vector %d: ", i);
        for (int j = 0; j < sizeof(vectors[i])/sizeof(vectors[i][0]); j++) {
            printf("%f ", vectors[i][j]);
        }
        printf("\n");
    }

    // Free the allocated memory
    free_matrix(vectors, num_vectors);
    return 0;
}
