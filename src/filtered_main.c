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

    // open query vectors file using query_open
    const char* query_file = "dummy-queries.bin";
    int query_vectors, count, d_queries = 100;
    float** posible_queries = query_open(query_file, &query_vectors, d_queries, &count);
    int queries_dim = d_queries+4;


    printf("\n");

    // we create and store the groundtruth
    Vector* groundtruth;
    const char* grtrth_file = "groundtruth.dat";

    char path[100];
    sprintf(path, "groundtruth/%s", grtrth_file);
    FILE *file = fopen(path, "rb");
    // if it already exists load it from memory
    if (file) {
        groundtruth = (Vector*)malloc(count * sizeof(Vector));

        fread(groundtruth, sizeof(Vector), count, file);
        fclose(file);
    } else {    // otherwise, compute it and store it
        groundtruth = Groundtruth(dataset, vecs, data_dim, posible_queries, count, queries_dim, k);

        file = fopen(path, "wb");
        if (file == NULL) {
            perror("Error opening file for writing");
            exit(EXIT_FAILURE);
        }
        fwrite(groundtruth, sizeof(Vector), count, file);
        fclose(file);
    }

    printf("found Groundtruth\n");

    Map med;
    int neigh = 5, t = 10, medoid;
    Vector* G = FilteredVamanaIndexing(dataset, min_f, max_f, vecs, data_dim, L, R, neigh, a, &med, &medoid, t);

    printf("G\n");
    for (int j = 0; j < vecs; j++) {
        printf("vector %d:", j);
        for (int i = 0; i < G[j]->size; i++) {
        printf(" %d",  vec_get_at(G[j], i));
            
        }
        printf("\n");
    }

    srand((unsigned int)time(NULL));

    // randomly select a query vector
    int xq_pos = rand() % (count - 1);
    float* xq = (float*)malloc(queries_dim * sizeof(float));
    for (int i=0; i<queries_dim; i++) {
        xq[i] = posible_queries[i][xq_pos];
    }
    printf("query: %d with filter: %f\n", xq_pos, xq[1]);

    // run the greedysearch algorithm to find its k nearest neighbours based on G
    Set V;
    PQueue knn = FilteredGreedySearch(G, R, data_dim, vecs, dataset, xq, (int)xq[1], med, medoid, L, k, &V);
    
    if (knn == NULL) {
        printf("there are no other vectors with filter %f in the dataset\n NO NEIGHBOURS WHERE FOUND\n", xq[1]);
    }

    //Calculation of accuracy  
    for (int i=0; i<count; i++) {
        printf("vector: %d", i);
        for (VecNode node = vec_first(groundtruth[i]); node != VECTOR_EOF; node = vec_next(groundtruth[i], node)) {
            printf("%d ", node->value);
        }
        printf("\n");
    }

    int found = 0;
    for (int i=0; i<k; i++) {
        if (i >= groundtruth[xq_pos]->size) break;
        int n_point = vec_get_at(groundtruth[xq_pos], i);
        printf("n_point: %d\n", n_point);
        if (vec_find_node(knn->vector,n_point) != VECTOR_EOF) {
            printf("exists\n");
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
    free_matrix_fvecs(posible_queries, queries_dim);
    free_G(G, vecs);
    free(xq);
    map_destroy(med);

    end = time(NULL);
    printf("The programm has taken %.f seconds \n", difftime(end,start));

    return 0;

}