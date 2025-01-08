#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//#include "../algorithms/algorithms.h"
#include "../algorithms/filtered_algorithms.h"
//#include "../open_functions/open.h"

int main(int argc, char ** argv) {
    srand((unsigned int)time(NULL));

    time_t start,end;
    start = time(NULL);
    int k , L , R , a = 1, threads;
    char* filter = NULL;
    const char* idx_file = NULL;
    const char* rnd = NULL;
    const char* dtset = NULL;
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
        if(strcmp(argv[i], "-filtered") == 0){
        filter = argv[i+1];
        }
        if(strcmp(argv[i], "-index_fname") == 0){
        idx_file = argv[i+1];
        }
        if(strcmp(argv[i], "-random") == 0){
        rnd = argv[i+1];
        }
        if(strcmp(argv[i], "-dataset") == 0){
        dtset = argv[i+1];
        }
        if(strcmp(argv[i], "-threads") == 0){
        threads = atoi(argv[i+1]);
        }
    }

    const char* base_file;
    const char* query_file;
    int vecs;
    if (strcmp(dtset, "10k") == 0) {
       base_file = "dummy-data.bin"; 
       query_file = "dummy-queries.bin";
       vecs = 10000;
    } else if (strcmp(dtset, "1m") == 0){
        base_file = "contest-data-release-1m.bin";
        query_file = "contest-queries-release-1m.bin";
        vecs = 1000000;
    }

    // open base vectors file using data_open   
    int num_vectors, d_base = 100;
    float min_f, max_f;
    Set filters;
    float** dataset = data_open(base_file, &num_vectors, d_base, &min_f, &max_f, &filters);
    int data_dim = d_base+2;

    // open query vectors file using query_open
    int query_vectors, count, d_queries = 100;
    float** posible_queries = query_open(query_file, &query_vectors, d_queries, &count);
    int queries_dim = d_queries+4;

    printf("\n");

    // we create and store the groundtruth
    Vector* groundtruth;
    const char* grtrth_file;
    if (strcmp(dtset, "10k") == 0) {
        grtrth_file = "groundtruth.dat";
    } else if (strcmp(dtset, "1m") == 0){
        grtrth_file = "groundtruth_1m.dat";
    }

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
            groundtruth[i]->array = (VecNode)realloc(groundtruth[i]->array, groundtruth[i]->size * sizeof(VecNode));

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

    printf("Found Groundtruth\n\n");

    // CODE FOR CROSS REFERENCING GROUNDTRUTH

    // int size_m = groundtruth[7]->size;
    // int* arr = (int*)malloc(sizeof(int) * size_m);
    // for (int i=0; i<size_m; i++) {
    //     arr[i] = vec_get_at(groundtruth[7], i);
    // }

    // for (int i = 0; i < size_m; i++) {
    //     for (int j = i + 1; j < size_m; j++) {
    //         if (arr[i] > arr[j]) {
    //             int temp = arr[i];
    //             arr[i] = arr[j];
    //             arr[j] = temp;
    //         }
    //     }
    // }

    // printf("grountruth for query 0: \n");
    // for (int i=0; i<size_m; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    // printf("queries: %d\n", count);
    // for (int j=0; j<count && j<20; j++) {
    //     printf("query %d with filter %f:", j, posible_queries[1][j]);
    //     for (VecNode node = vec_first(groundtruth[j]); node != VECTOR_EOF; node = vec_next(groundtruth[j], node)) {
    //         int idx = node->value;
    //         printf("%d - %f, ", idx, dataset[0][idx]);
    //     }
    //     printf("\n");
    // }

    // return 0;

    Map med;
    int neigh = 5, t = 10, medoid;

    // we create and store the vamana index
    Vector* G;

    char G_path[100];
    sprintf(G_path, "filtered/%s.dat", idx_file);
    FILE *G_file = fopen(G_path, "rb");
    
    // if it already exists load it from memory
    if (G_file) {

        G = (Vector*)malloc(vecs * sizeof(Vector));
        // for each vector, deserialize it 
        for (int i = 0; i < vecs; ++i) {
            G[i] = vec_Create(0);

            // first we have to read each vector's size
            fread(&(G[i]->size), sizeof(int), 1, G_file);

            // then we allocate memory for the array
            G[i]->array = (VecNode)realloc(G[i]->array, G[i]->size * sizeof(VecNode));

            // and we fill it with the required elements
            fread(G[i]->array, sizeof(VecNode), G[i]->size, G_file);
        }

        fclose(G_file);

        // we also have to calculate the medoids map to be used by greedy search
        Map filtered_data = map_create(min_f, max_f);

        for (int j=0; j<vecs; j++) {
            map_insert(filtered_data, (int)dataset[0][j], j);
        }

        med = FindMedoid(dataset, vecs, min_f, max_f, filtered_data, t);
        printf("Found starting points.\n\n");

    } else {    // otherwise, compute it and store it
        
        if (strcmp(rnd, "no") == 0){ G = FilteredVamanaIndexing(dataset, min_f, max_f, vecs, data_dim, queries_dim, L, R, a, &med, &medoid, t, threads);}
        if (strcmp(rnd, "yes") == 0){ G = FilteredVamanaIndexing_randomG(dataset, min_f, max_f, vecs, data_dim, queries_dim, L, R, neigh, a, &med, &medoid, t, threads);}

        G_file = fopen(G_path, "wb");
        if (G_file == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);         
        }
        
        // for each vector in the dataset, serialize it
        for (int i = 0; i < vecs; ++i) {
            // first we have to store each vector's size
            fwrite(&(G[i]->size), sizeof(int), 1, G_file);

            // then the elements of the actual array
            fwrite(G[i]->array, sizeof(VecNode), G[i]->size, G_file);
        }

        fclose(G_file);
    }

    printf("Found Vamana index\n");

    // printf("G\n");
    // for (int j = 0; j < vecs; j++) {
    //     printf("vector %d:", j);
    //     for (int i = 0; i < G[j]->size; i++) {
    //     printf(" %d",  vec_get_at(G[j], i));
            
    //     }
    //     printf("\n");
    // }

    srand((unsigned int)time(NULL));

    // randomly select a query vector
    int fflag = 0, xq_pos;
    while (fflag == 0) {
        fflag = 1;
        xq_pos = rand() % (count - 1);
        // make sure its filtered or unfiltered
        if (strcmp(filter, "no") == 0) if (posible_queries[1][xq_pos] != -1) fflag = 0;
        if (strcmp(filter, "yes") == 0) if (posible_queries[1][xq_pos] == -1) fflag = 0;
    }
    // int xq_pos = rand() % (count - 1);
    int xq_size = queries_dim-4;
    float* xq = (float*)malloc(xq_size * sizeof(float));
    int c = 4, xq_f = (int)posible_queries[1][xq_pos];
    for (int i=0; i<xq_size; i++) {
        xq[i] = posible_queries[c][xq_pos];
        c++;
    }
    printf("query: %d with filter: %d\n", xq_pos, xq_f);

    // run the greedysearch algorithm to find its k nearest neighbours based on G
    Set V;
    PQueue knn;
    PQueue knn_q;
    // if the query has a filter, run greedysearch as normal
    if (xq_f != -1) {
       knn = FilteredGreedySearch(G, R, data_dim, vecs, dataset, xq, xq_f, med, medoid, L, k, &V);
    } else {
        // otherwise, run greedysearch to find its nearest neighbours from every filter
        knn_q = pqueue_create(0);
        PQueue knn_g;
        for (MapNode node = map_first(med); node != MAP_EOF; node = map_next(med, node)) {
            knn_g = FilteredGreedySearch(G, R, data_dim, vecs, dataset, xq, node->key, med, medoid, L, k, &V);
            for (VecNode vnode = vec_first(knn_g->vector); vnode != VECTOR_EOF; vnode = vec_next(knn_g->vector, vnode)) {
                pqueue_insert(knn_q, vnode->value, vnode->dist);    // add them to a priority queue
            }
        }
        
        // and then keep only the k nearest of them
        if (knn_q->vector->size > k) {
            // remove the most distant neighbours of xq from knn_set until its equal to k
            while(knn_q->vector->size > k) {
                pqueue_remove(knn_q);
            }
        }

        knn = knn_q;
        
        // printf("knn: ");
        // for (VecNode vnode = vec_first(knn->vector); vnode != VECTOR_EOF; vnode = vec_next(knn->vector, vnode)) {
        //     printf("%d ", vnode->value);
        // }
        // printf("\n");

    }

    if (knn == NULL) {
        printf("there are no other vectors with filter %d in the dataset\n NO NEIGHBOURS WHERE FOUND\n", xq_f);
    }

    //Calculation of accuracy  
    // printf("query groundtruth %d with filter %f: ", xq_pos, posible_queries[1][xq_pos]);
    // for (VecNode node = vec_first(groundtruth[xq_pos]); node != VECTOR_EOF; node = vec_next(groundtruth[xq_pos], node)) {
    //     printf("%d -> %f  ", node->value , dataset[0][node->value]);
    // }
    // printf("\n");

    int found = 0;
    for (int i=0; i<k; i++) {
        if (i >= groundtruth[xq_pos]->size) break;
        int n_point = vec_get_at(groundtruth[xq_pos], i);
        if (vec_find_node(knn->vector,n_point) != VECTOR_EOF) {
            printf("n_point: %d -> %f exists\n", n_point, dataset[0][n_point]);
            found++;
        }
    }
    float accuracy = found* 100 / groundtruth[xq_pos]->size;
    printf("Our program calculates the data with an accuracy of : %f \n",accuracy); 

    // frees
    //pqueue_destroy(knn_q);
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