#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//#include "../algorithms/algorithms.h"
#include "../algorithms/filtered_algorithms.h"
#include "../open_functions/open.h"

int main(int argc, char ** argv) {
    srand((unsigned int)time(NULL));

    time_t start,end;
    start = time(NULL);
    int k , L , R , a = 1;
    for (int i = 0; i< argc; i++){
        if(strcmp(argv[i], "-k") == 0){
        k = atoi(argv[i+1]);
        }
        if(strcmp(argv[i], "-L") == 0){
        L = atoi(argv[i+1]);
        }
        if(strcmp(argv[i], "-R") == 0){
        R = atoi(argv[i+1]);
        }
    }

    // open base vectors file using data_open
    const char* base_file = "dummy-data.bin";
    int num_vectors, d_base = 100;
    float min_f, max_f;
    float** dataset = data_open(base_file, &num_vectors, d_base, &min_f, &max_f);

    int vecs = 10000;
    float** vectors = (float**)malloc(d_base * sizeof(float*));
    for (int i = 0; i < d_base; i++) {
        vectors[i] = (float*)malloc(vecs * sizeof(float));
    }

    for (int j = 0; j<num_vectors && j < vecs; j++) {
        //printf("vector %d: ", j);
        for (int i = 0; i<d_base; i++) {
        vectors[i][j] = dataset[i][j];
        //printf("%f ", vectors[i][j]);
        }
    }
    printf("\n");

    // open query vectors file using query_open
    const char* query_file = "dummy-queries.bin";
    int query_vectors, d_queries = 100;
    float** posible_queries = query_open(query_file, &query_vectors, d_queries);

    printf("\n");

    
    
    // WE HAVE TO CREATE AND STORE OUR OWN GROUNDTRUTH FILE ONCE!!

    // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
    float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
    for (int i = 0; i < vecs; i++) {
        dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
    }

    for(int i=0; i<vecs; i++) {
        for(int j=0; j<vecs; j++) {
            dist_matrix[i][j] = 0;
        }
    }

    int med;
    int neigh = 5;
    Vector* G = FilteredVamanaIndexing(vectors, min_f, max_f, vecs, d_base, L, R, neigh, a, &med);

    // printf("G\n");
    // for (int j = 0; j < vecs; j++) {
    //     printf("vector %d:", j);
    //     for (int i = 0; i < R; i++) {
    //     printf(" %d",  vec_get_at(G[j], i));
            
    //     }
    //     printf("\n");
    // }

    // randomly select a query vector
    int xq_pos = rand() % (query_vectors - 1);
    float* xq = (float*)malloc(d_queries * sizeof(float));
    for (int i=0; i<d_queries; i++) {
        xq[i] = posible_queries[i][xq_pos];
    }
    printf("%d\n", xq_pos);


    // run the greedysearch algorithm to find its k nearest neighbours based on G
    

    //Calculation of accuracy  

    // frees
    free_matrix_fvecs(dataset, d_base);
    free_matrix_fvecs(vectors, d_base);
    free_matrix_fvecs(posible_queries, d_queries);
    free_matrix_fvecs(dist_matrix, vecs);
    free_G(G, vecs);
    free(xq);

    end = time(NULL);
    printf("The programm has taken %.f seconds \n", difftime(end,start));

    return 0;

}