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
        // for each vector of groundtruth, deserialize it 
        for (int i = 0; i < count; ++i) {
            groundtruth[i] = vec_Create(0);

            // first we have to read each vector's size
            fread(&(groundtruth[i]->size), sizeof(int), 1, file);

            // then we allocate memory for the array
            groundtruth[i]->array = (VecNode)malloc(groundtruth[i]->size * sizeof(VecNode));

            // and we fill it with the required elements
            fread(groundtruth[i]->array, sizeof(VecNode), groundtruth[i]->size, file);
        }

        fclose(file);
    } else {    // otherwise, compute it and store it
        groundtruth = Groundtruth(dataset, vecs, data_dim, posible_queries, count, queries_dim, k);

        file = fopen(path, "wb");
        if (file == NULL) {
            perror("Error opening file for writing");
            exit(EXIT_FAILURE);
        }
        
        // for each vector in the dataset, serialize it
        for (int i = 0; i < count; ++i) {
            // first we have to store each vector's size
            fwrite(&(groundtruth[i]->size), sizeof(int), 1, file);

            // then the elements of the actual array
            fwrite(groundtruth[i]->array, sizeof(VecNode), groundtruth[i]->size, file);
        }

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
    int fflag = 0, xq_pos;
    while (fflag == 0) {
        fflag = 1;
        xq_pos = rand() % (count - 1);
        if (posible_queries[1][xq_pos] != -1) fflag = 0;
    }
    // int xq_pos = rand() % (count - 1);
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
    printf("query groundtruth %d with filter %f: ", xq_pos, posible_queries[1][xq_pos]);
    for (VecNode node = vec_first(groundtruth[xq_pos]); node != VECTOR_EOF; node = vec_next(groundtruth[xq_pos], node)) {
        printf("%d -> %f  ", node->value , dataset[0][node->value]);
    }
    printf("\n");

    printf("medoid: %d with filter: %f\n", medoid, dataset[0][medoid]);

    int found = 0;
    for (int i=0; i<k; i++) {
        if (i >= groundtruth[xq_pos]->size) break;
        int n_point = vec_get_at(groundtruth[xq_pos], i);
        //printf("n_point: %d ", n_point);
        if (vec_find_node(knn->vector,n_point) != VECTOR_EOF) {
            //printf("exists");
            printf("n_point: %d -> %f exists\n", n_point, dataset[0][n_point]);
            found++;
        }
        //printf("\n");
    }
    float accuracy = found* 100 / groundtruth[xq_pos]->size;
    printf("Our program calculates the data with an accuracy of : %f \n",accuracy); 

    // frees
    set_destroy(V);
    pqueue_destroy(knn);
    free_G(groundtruth, count);
    free_matrix_fvecs(dataset, data_dim);
    free_matrix_fvecs(posible_queries, queries_dim);
    free_G(G, vecs);
    free(xq);
    map_destroy(med);

    end = time(NULL);
    printf("The programm has taken %.f seconds \n", difftime(end,start));

    return 0;

}