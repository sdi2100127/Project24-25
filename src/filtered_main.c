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
    int data_dim = d_base+2;

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
    int query_vectors, count, d_queries = 100;
    float** posible_queries = query_open(query_file, &query_vectors, d_queries, &count);
    int queries_dim = d_queries+4;


    printf("\n");

    // we create and store the groundtruth
    Vector* groundtruth = Groundtruth(dataset, vecs, data_dim, posible_queries, count, queries_dim, k);

    Map med;
    int neigh = 5, t = 10;
    Vector* G = FilteredVamanaIndexing(vectors, min_f, max_f, vecs, data_dim, L, R, neigh, a, &med, t);

    printf("G\n");
    for (int j = 0; j < vecs; j++) {
        printf("vector %d:", j);
        for (int i = 0; i < R; i++) {
        printf(" %d",  vec_get_at(G[j], i));
            
        }
        printf("\n");
    }

    // randomly select a query vector
    int xq_pos = rand() % (count - 1);
    float* xq = (float*)malloc(queries_dim * sizeof(float));
    for (int i=0; i<queries_dim; i++) {
        xq[i] = posible_queries[i][xq_pos];
    }
    printf("%d\n", xq_pos);

    // run the greedysearch algorithm to find its k nearest neighbours based on G
    Set V;
    PQueue knn = FilteredGreedySearch(G, R, data_dim, vecs, dataset, xq, xq[1], med, L, k, &V);

    //Calculation of accuracy  
    int* test_knn = (int*)malloc(k * sizeof(int));

    int found = 0;
    for (int i=0; i<k; i++) {
        int n_point = vec_get_at(groundtruth[xq_pos], i);
        if (vec_find_node(knn->vector,n_point) != VECTOR_EOF) {
        found++;
        }
        if (found == k) break;
    }
    float accuracy = found* 100 / k;
    printf("Our programm calculates the data with an accuracy of : %f \n",accuracy); 

    // frees
    set_destroy(V);
    pqueue_destroy(knn);
    free_G(groundtruth, query_vectors);
    free_matrix_fvecs(dataset, data_dim);
    free_matrix_fvecs(vectors, data_dim);
    free_matrix_fvecs(posible_queries, queries_dim);
    free_G(G, vecs);
    free(xq);

    end = time(NULL);
    printf("The programm has taken %.f seconds \n", difftime(end,start));

    return 0;

}