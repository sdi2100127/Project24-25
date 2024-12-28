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
    Set filters;
    float** dataset = data_open(base_file, &num_vectors, d_base, &min_f, &max_f, &filters);
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

    printf("found Groundtruth\n");

    int t = 10, medoid, neigh = 5;
    Map med, filtered_data;
    Vector* per;
    Vector** G = StichedVamanaIndexing_randomG(dataset, min_f, max_f, filters, vecs, data_dim, L, R, a, &med, &medoid, t, neigh, &filtered_data, &per);
    //Vector** G = StichedVamanaIndexing(dataset, min_f, max_f, filters, vecs, data_dim, L, R, a, &med, &medoid, t, &filtered_data, &per);

    // printf("G: \n");
    // for (int f=0; f<filters->size; f++) {
    //     Vector G_f = G[f];
    //     int G_size = map_find_values(filtered_data, f)->size;

    //     for (int j = 0; j < G_size; j++) {
    //         Vector df = G[f][j];
    //         int vec = vec_get_at(per[f], j);
    //         printf("vector %d:", vec);
    //         for (VecNode s_node = vec_first(df); s_node != VECTOR_EOF; s_node = vec_next(df, s_node)) {
    //             int n = s_node->value;
    //             printf(" %d",  vec_get_at(per[f], n));   
    //         }
    //         printf("\n");
    //     }
    //     printf("done with filter %d\n", f);
    // }


    srand((unsigned int)time(NULL));

    // randomly select a query vector
    int fflag = 0, xq_pos;
    while (fflag == 0) {
        fflag = 1;
        xq_pos = rand() % (count - 1);
        if (posible_queries[1][xq_pos] != 0) fflag = 0;
    }
    // int xq_pos = rand() % (count - 1);
    int xq_size = queries_dim-4;
    float* xq = (float*)malloc(xq_size * sizeof(float));
    int z = 4, xq_f = (int)posible_queries[1][xq_pos];
    for (int i=0; i<xq_size; i++) {
        xq[i] = posible_queries[z][xq_pos];
        z++;
    }
    printf("query: %d with filter: %d\n", xq_pos, xq_f);

    // for (int j = 0; j < f_size; j++) {
    //     printf("vector %d -> %d: ", j, vec_get_at(per[xq_f], j));
    //     for (int i = 0; i < c; i++) {
    //         printf("%f ", data_f[i][j]);
    //     }
    //     printf("\n\n");
    // }

    // run the greedysearch algorithm to find its k nearest neighbours based on G
    Set V;
    PQueue knn;
    PQueue knn_q;
    // if the query has a filter, run greedysearch as normal
    if (xq_f != -1) {
        int c = data_dim-2;
        int f_size = map_find_values(filtered_data, xq_f)->size;
        float** data_f = (float**)malloc(c * sizeof(float*));
        for (int i = 0; i < c; i++) {
            data_f[i] = (float*)malloc(f_size * sizeof(float));
        }

        for (int j = 0; j < f_size; j++) {
            int count = 0;
            for (int i = 2; i < data_dim; i++) {
                //printf("%f \n", dataset[i][vec_get_at(per[f], j)]);
                data_f[count][j] = dataset[i][vec_get_at(per[xq_f], j)];
                count++;
            }
        }

        int f_med = vec_first(map_find_values(med, xq_f))->value;
        printf("f_med: %d\n", f_med);
        knn = greedySearch(G[xq_f], R, data_dim-2, f_size, data_f, f_med, xq, L, k, &V);

        printf("knn: ");
        for (VecNode vnode = vec_first(knn->vector); vnode != VECTOR_EOF; vnode = vec_next(knn->vector, vnode)) {
            vec_set_at(knn->vector, vec_find_pos(knn->vector, vnode->value), vec_get_at(per[xq_f], vnode->value), vnode->dist);
            printf("%d ", vnode->value);
        }
        printf("\n");

        free_matrix_fvecs(data_f, c);
    } else {
        // otherwise, run greedysearch to find its nearest neighbours from every filter
        knn_q = pqueue_create(0);
        PQueue knn_g;
        for (MapNode node = map_first(med); node != MAP_EOF; node = map_next(med, node)) {
            printf("filter %d: \n", node->key);
            int c = data_dim-2;
            int f_size = map_find_values(filtered_data, node->key)->size;
            float** data_f = (float**)malloc(c * sizeof(float*));
            for (int i = 0; i < c; i++) {
                data_f[i] = (float*)malloc(f_size * sizeof(float));
            }

            for (int j = 0; j < f_size; j++) {
                int count = 0;
                for (int i = 2; i < data_dim; i++) {
                    //printf("%f \n", dataset[i][vec_get_at(per[f], j)]);
                    data_f[count][j] = dataset[i][vec_get_at(per[node->key], j)];
                    count++;
                }
            }

            printf("found data_f\n");

            int f_med = vec_first(map_find_values(med, node->key))->value;
            printf("f_med: %d\n", f_med);

            knn_g = greedySearch(G[node->key], R, data_dim-2, f_size, data_f, f_med, xq, L, k, &V);
            
            for (VecNode vnode = vec_first(knn_g->vector); vnode != VECTOR_EOF; vnode = vec_next(knn_g->vector, vnode)) {
                pqueue_insert(knn_q, vnode->value, vnode->dist);    // add them to a priority queue
            }

            free_matrix_fvecs(data_f, c);
        }
        
        // and then keep only the k nearest of them
        if (knn_q->vector->size > k) {
            // remove the most distant neighbours of xq from knn_set until its equal to k
            while(knn_q->vector->size > k) {
                pqueue_remove(knn_q);
            }
        }

        knn = knn_q;
        
        printf("knn: ");
        for (VecNode vnode = vec_first(knn->vector); vnode != VECTOR_EOF; vnode = vec_next(knn->vector, vnode)) {
            vec_set_at(knn->vector, vec_find_pos(knn->vector, vnode->value), vec_get_at(per[xq_f], vnode->value), vnode->dist);
            printf("%d ", vnode->value);
        }
        printf("\n");

    }

    if (knn == NULL) {
        printf("there are no other vectors with filter %d in the dataset\n NO NEIGHBOURS WHERE FOUND\n", xq_f);
    }
    

    //Calculation of accuracy  
    printf("query groundtruth %d with filter %f: ", xq_pos, posible_queries[1][xq_pos]);
    for (VecNode node = vec_first(groundtruth[xq_pos]); node != VECTOR_EOF; node = vec_next(groundtruth[xq_pos], node)) {
        printf("%d -> %f  ", node->value , dataset[0][node->value]);
    }
    printf("\n");

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
    free_G_f(G, filters->size, filtered_data);
    map_destroy(filtered_data);
    free_G(per, filters->size);
    set_destroy(filters);

    set_destroy(V);
    pqueue_destroy(knn);
    free_G(groundtruth, count);
    free_matrix_fvecs(dataset, data_dim);
    free_matrix_fvecs(posible_queries, queries_dim);
    free(xq);
    map_destroy(med);

    end = time(NULL);
    printf("The programm has taken %.f seconds \n", difftime(end,start));

    return 0;

}