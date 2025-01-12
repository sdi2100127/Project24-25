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
    int k , L , R , a = 1, threads, st_threads;
    char* filter = NULL;
    const char* rnd = NULL;
    const char* idx_file = NULL;
    const char* dtset = NULL;
    const char* vmn = NULL;
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
        if(strcmp(argv[i], "-vmn_threads") == 0){
        threads = atoi(argv[i+1]);
        }
        if(strcmp(argv[i], "-vamana") == 0){
        vmn = argv[i+1];
        }
        if(strcmp(argv[i], "-stitch_threads") == 0){
        st_threads = atoi(argv[i+1]);
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
    else if (strcmp(dtset, "20k") == 0){
        grtrth_file = "groundtruth_20k.dat";
    }
    else if (strcmp(dtset, "50k") == 0){
        grtrth_file = "groundtruth_50k.dat";
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

    int t = 10, medoid, neigh = 5;
    Map med, filtered_data;
    Vector* per;

    // we create and store the vamana index
    Vector** G;

    char G_path[100];
    sprintf(G_path, "filtered/%s.dat", idx_file);
    FILE *G_file = fopen(G_path, "rb");
    
    // if it already exists load it from memory
    if (G_file) {

        G = (Vector**)malloc(filters->size * sizeof(Vector*));
        for (int i = 0; i < filters->size; i++) {
            G[i] = (Vector*)malloc(vecs * sizeof(Vector));
            for (int j=0; j<vecs; j++) {
                G[i][j] = vec_Create(0); 
            }
        }

        // for each vector, deserialize it 
        for (int f=0; f<filters->size; f++) {
            // first we have to read each G's size
            int G_size;
            fread(&(G_size), sizeof(int), 1, G_file);

            for (int j = 0; j < G_size; j++) {

                // first we have to read each vector's size
                int vec_size;
                fread(&vec_size, sizeof(int), 1, G_file);
                G[f][j]->size = vec_size;

                // then we allocate memory for the array
                if (G[f][j]->size > 0) {
                    G[f][j]->array = (VecNode)realloc(G[f][j]->array, G[f][j]->size * sizeof(VecNode));
                } else {
                    G[f][j]->array = NULL;
                }

                // and we fill it with the required elements
                fread(G[f][j]->array, sizeof(VecNode), G[f][j]->size, G_file);
            }
        }
        fclose(G_file);

        // we also have to calculate the medoids map to be used by greedy search
        filtered_data = map_create(min_f, max_f);

        for (int j=0; j<vecs; j++) {
            map_insert(filtered_data, (int)dataset[0][j], j);
        }

        med = FindMedoid(dataset, vecs, min_f, max_f, filtered_data, t);
        printf("found starting points.\n\n");

        per = (Vector*)malloc(filters->size * sizeof(Vector));
        for (int i=0; i<filters->size; i++) {
            per[i] = vec_Create(0);
        }

        for (int f=0; f<filters->size; f++) {
            Vector f_values = map_find_values(filtered_data, f);

            for (VecNode s_node = vec_first(f_values); s_node != VECTOR_EOF; s_node = vec_next(f_values, s_node)) {
                vec_insert(per[f], s_node->value, s_node->dist);
            }
        }


    } else {    // otherwise, compute it and store it

        if (strcmp(rnd, "no") == 0) G = StichedVamanaIndexing(dataset, min_f, max_f, filters, vecs, data_dim, L, R, a, &med, &medoid, t, &filtered_data, &per, threads, vmn, st_threads);
        if (strcmp(rnd, "yes") == 0) G = StichedVamanaIndexing_randomG(dataset, min_f, max_f, filters, vecs, data_dim, L, R, a, &med, &medoid, t, neigh, &filtered_data, &per, threads, vmn, st_threads);

        G_file = fopen(G_path, "wb");
        if (G_file == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);         
        }
        
        // for each vector in the dataset, serialize it
        for (int f=0; f<filters->size; f++) {
            int G_size = map_find_values(filtered_data, f)->size;
            // first we have to store each G's size
            fwrite(&(G_size), sizeof(int), 1, G_file);
            for (int j = 0; j < G_size; j++) {
                Vector df = G[f][j];
                // first we have to store each vector's size
                fwrite(&(df->size), sizeof(int), 1, G_file);

                // then the elements of the actual array
                fwrite(df->array, sizeof(VecNode), df->size, G_file);
            }
        }

        fclose(G_file);
    }

    printf("Found Vamana index.\n\n");

    // printf("G: \n");
    // for (int f=0; f<filters->size; f++) {
    //     printf("filter: %d\n", f);
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
    // }


    srand((unsigned int)time(NULL));

    // randomly select a query vector
    int fflag = 0, xq_pos;
    while (fflag == 0) {
        fflag = 1;
        xq_pos = rand() % (count - 1);
        if (strcmp(filter, "no") == 0) if (posible_queries[1][xq_pos] != -1) fflag = 0;
        if (strcmp(filter, "yes") == 0) if (posible_queries[1][xq_pos] == -1) fflag = 0;
    }
    int xq_size = queries_dim-4;
    float* xq = (float*)malloc(xq_size * sizeof(float));
    int z = 4, xq_f = (int)posible_queries[1][xq_pos];
    for (int i=0; i<xq_size; i++) {
        xq[i] = posible_queries[z][xq_pos];
        z++;
    }
    printf("query: %d with filter: %d\n", xq_pos, xq_f);

    // run the greedysearch algorithm to find its k nearest neighbours based on G
    Set V;
    PQueue knn;
    PQueue knn_q;
    // if the query has a filter, run greedysearch as normal
    if (xq_f != -1) {

        printf("has filter\n");
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
        printf("f_med: %d, size: %d\n", f_med, G[xq_f][f_med]->size);
        // int sz = G[xq_f][f_med]->size;
        // while (G[xq_f][f_med]->size == 0) {
        //     fflag = 0;
        //     while (fflag == 0) {
        //         fflag = 1;
        //         xq_pos = rand() % (count - 1);
        //         if (strcmp(filter, "yes") == 0) if (posible_queries[1][xq_pos] == -1) fflag = 0;
        //     }
        //     xq_f = (int)posible_queries[1][xq_pos];
        //     printf("query: %d with filter: %d\n", xq_pos, xq_f);
        //     f_med = vec_first(map_find_values(med, xq_f))->value;
        //     sz = G[xq_f][f_med]->size;
        //     printf("f_med: %d, size: %d\n", f_med, sz);
        // }

        knn = greedySearch(G[xq_f], R, c, f_size, data_f, f_med, xq, L, k, &V);

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
        for (int f=0; f<filters->size; f++)  {
            //printf("filter %d: \n", f);
            int c = data_dim-2;
            int f_size = map_find_values(filtered_data, f)->size;
            float** data_f = (float**)malloc(c * sizeof(float*));
            for (int i = 0; i < c; i++) {
                data_f[i] = (float*)malloc(f_size * sizeof(float));
            }

            for (int j = 0; j < f_size; j++) {
                int count = 0;
                for (int i = 2; i < data_dim; i++) {
                    //printf("%f \n", dataset[i][vec_get_at(per[node->key], j)]);
                    data_f[count][j] = dataset[i][vec_get_at(per[f], j)];
                    count++;
                }
            }

            //printf("found data_f\n");

            int f_med = vec_first(map_find_values(med, f))->value;
            //printf("f_med: %d\n", f_med);

            knn_g = greedySearch(G[f], R, c, f_size, data_f, f_med, xq, L, k, &V);
            
            for (VecNode vnode = vec_first(knn_g->vector); vnode != VECTOR_EOF; vnode = vec_next(knn_g->vector, vnode)) {
                vec_set_at(knn_g->vector, vec_find_pos(knn_g->vector, vnode->value), vec_get_at(per[f], vnode->value), vnode->dist);
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
            printf("%d ", vnode->value);
        }
        printf("\n");

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