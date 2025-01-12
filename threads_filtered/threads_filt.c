#include "threads_filt.h"
#include <string.h>

pthread_mutex_t filtered_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t G_f_mutex = PTHREAD_MUTEX_INITIALIZER;

void* build_vamana_index(void* arg) {
    printf("hi from a thread!\n");
    ThreadData_stitch* data = (ThreadData_stitch*)arg;

    //printf("f_start: %d, f_end: %d\n", data->f_start, data->f_end);

    for (int f=data->f_start; f<data->f_end; f++) {
        //printf("filter: %d\n", f);
        
        // keep the current filter, the values associated with it, and how many there are
        pthread_mutex_lock(&filtered_data_mutex);
        int f_size = map_find_values(data->filtered_data, f)->size;
        if (f_size == 1) {pthread_mutex_unlock(&filtered_data_mutex); continue;}

        Vector f_values = map_find_values(data->filtered_data, f);
        pthread_mutex_unlock(&filtered_data_mutex);

        for (VecNode s_node = vec_first(f_values); s_node != VECTOR_EOF; s_node = vec_next(f_values, s_node)) {
            vec_insert(data->per[f], s_node->value, s_node->dist);
        }

        // printf("per: ");
        // for (VecNode s_node = vec_first(data->per[f]); s_node != VECTOR_EOF; s_node = vec_next(data->per[f], s_node)) {
        //     printf("%d ", s_node->value);
        // }
        // printf("\n");

        //printf("random stitched permutation: \n");
        for (int j=0; j<f_size; j++) {
            //printf("%d ", vec_get_at(per[f], j));
        }
        //printf("\n");

        // and store them in the new dataset
        int c = data->comps-2;
        float** data_f = (float**)malloc(c * sizeof(float*));
        for (int i = 0; i < c; i++) {
            data_f[i] = (float*)malloc(f_size * sizeof(float));
        }

        for (int j = 0; j < f_size; j++) {
            int count = 0;
            for (int i = 2; i < data->comps; i++) {
                //printf("%f \n", dataset[i][vec_get_at(per[f], j)]);
                data_f[count][j] = data->dataset[i][vec_get_at(data->per[f], j)];
                count++;
            }
        }

        // for (int j = 0; j < f_size; j++) {
        //     printf("vector %d -> %d: ", j, vec_get_at(data->per[f], j));
        //     for (int i = 0; i < c; i++) {
        //         printf("%f ", data_f[i][j]);
        //     }
        //     printf("\n\n");
        // }

        int Vmedoid;

        if (f_size == 2) {
            int pos = 1;
            for (int j = 0; j < f_size; j++) {
                float* vec_x = (float*)malloc(c * sizeof(float));
                float* vec_j = (float*)malloc(c * sizeof(float));
                for (int i=0; i<c; i++) {
                    vec_x[i] = data_f[i][j];
                    vec_j[i] = data_f[i][pos];
                }
                
                pthread_mutex_lock(&G_f_mutex);
                vec_insert(data->G_f[f][j], vec_get_at(data->per[f], j), euclidean_distance(vec_x, vec_j, c));
                pthread_mutex_unlock(&G_f_mutex);

                pos--;
                free(vec_x);
                free(vec_j);
            }
        } else {
            int threads = 0;
            pthread_mutex_lock(&G_f_mutex);
            if (data->R_stitched >= f_size) {
                //printf("R: %d\n", f_size);
                //printf("vmn, threads: %s, %d\n", vmn, threads);
                if (strcmp(data->vmn, "main") == 0) data->G_f[f] = Vamana_main(data_f, f_size, data->comps-2, data->L, f_size-1, data->a, &Vmedoid, threads);
                if (strcmp(data->vmn, "random")==0) data->G_f[f] = Vamana_random_medoid(data_f, f_size, data->comps-2, data->L, f_size-1, data->a, &Vmedoid, threads);
                if (strcmp(data->vmn, "semirandom")==0) data->G_f[f] = Vamana_semirandom_medoid(data_f, f_size, data->comps-2, data->L, f_size-1, data->a, &Vmedoid, threads);
            } else {
                //printf("R: %d\n", R_stitched);
                //printf("vmn, threads: %s, %d\n", vmn, threads);
                if (strcmp(data->vmn, "main")==0) data->G_f[f] = Vamana_main(data_f, f_size, data->comps-2, data->L, data->R_stitched, data->a, &Vmedoid, threads);
                if (strcmp(data->vmn, "random")==0) data->G_f[f] = Vamana_random_medoid(data_f, f_size, data->comps-2, data->L, data->R_stitched, data->a, &Vmedoid, threads);
                if (strcmp(data->vmn, "semirandom")==0) data->G_f[f] = Vamana_semirandom_medoid(data_f, f_size, data->comps-2, data->L, data->R_stitched, data->a, &Vmedoid, threads);
            }
            pthread_mutex_unlock(&G_f_mutex);
        }

        free_matrix_fvecs(data_f, data->comps-2);

    }

    pthread_exit(NULL);
}

void* build_vamana_index_random(void* arg) {
    printf("hi from a thread!\n");
    ThreadData_stitch* data = (ThreadData_stitch*)arg;

    //printf("f_start: %d, f_end: %d\n", data->f_start, data->f_end);

    for (int f=data->f_start; f<data->f_end; f++) {
        //printf("filter: %d\n", f);
        
        // keep the current filter, the values associated with it, and how many there are
        pthread_mutex_lock(&filtered_data_mutex);
        int f_size = map_find_values(data->filtered_data, f)->size;
        if (f_size == 1) {pthread_mutex_unlock(&filtered_data_mutex); continue;}

        Vector f_values = map_find_values(data->filtered_data, f);
        pthread_mutex_unlock(&filtered_data_mutex);

        for (VecNode s_node = vec_first(f_values); s_node != VECTOR_EOF; s_node = vec_next(f_values, s_node)) {
            vec_insert(data->per[f], s_node->value, s_node->dist);
        }

        // printf("per: ");
        // for (VecNode s_node = vec_first(data->per[f]); s_node != VECTOR_EOF; s_node = vec_next(data->per[f], s_node)) {
        //     printf("%d ", s_node->value);
        // }
        // printf("\n");

        // and store them in the new dataset
        int c = data->comps-2;
        float** data_f = (float**)malloc(c * sizeof(float*));
        for (int i = 0; i < c; i++) {
            data_f[i] = (float*)malloc(f_size * sizeof(float));
        }

        for (int j = 0; j < f_size; j++) {
            int count = 0;
            for (int i = 2; i < data->comps; i++) {
                //printf("%f \n", dataset[i][vec_get_at(per[f], j)]);
                data_f[count][j] = data->dataset[i][vec_get_at(data->per[f], j)];
                count++;
            }
        }

        // for (int j = 0; j < f_size; j++) {
        //     printf("vector %d -> %d: ", j, vec_get_at(data->per[f], j));
        //     for (int i = 0; i < c; i++) {
        //         printf("%f ", data_f[i][j]);
        //     }
        //     printf("\n\n");
        // }

        printf("Creating random starting graph.\n\n");

        int x;
        float* vec_x = (float*)malloc((data->comps-2) * sizeof(float));
        float* vec_j = (float*)malloc((data->comps-2) * sizeof(float));

        // for every vector in the dataset
        for (int j = 0; j < f_size; j++) {
            for (int i=0; i<c; i++) {
                vec_j[i] = data_f[i][j];
            }
            //printf("vector %d:", j);

            // find its filter
            int filter = data_f[0][j];
            //Vector same_f_values = map_find_values(filtered_data, filter);
        
            // for every one of its neigh neighbours, or until all the same filter points have been checked
            int count_n = 0;
            VecNode v_node = vec_first(f_values);
            while (count_n < data->neigh && count_n < f_values->size) {
                // insert them in the vector of out going neighbours of vector j
                x = v_node->value;
                if (x != j) {
                    // temporarilly store the vectors j and x to compute their distance
                    for (int i=0; i<c; i++) {
                        vec_x[i] = data_f[i][x];
                    }
                    float dist = euclidean_distance(vec_j, vec_x, c);
                    pthread_mutex_lock(&G_f_mutex);
                    vec_insert(data->G_f[f][j], v_node->value, dist);
                    pthread_mutex_unlock(&G_f_mutex);
                    //printf(" %d, %f ", v_node->value, dist);
                    count_n++;
                }
                v_node = vec_next(f_values, v_node);

                if (v_node == VECTOR_EOF) break;
                
            }
            //printf("\n");
        }
        free(vec_x);
        free(vec_j);

        printf("Found random starting graph.\n\n");

        int Vmedoid;

        if (f_size == 2) {
            int pos = 1;
            for (int j = 0; j < f_size; j++) {
                float* vec_x = (float*)malloc(c * sizeof(float));
                float* vec_j = (float*)malloc(c * sizeof(float));
                for (int i=0; i<c; i++) {
                    vec_x[i] = data_f[i][j];
                    vec_j[i] = data_f[i][pos];
                }
                
                pthread_mutex_lock(&G_f_mutex);
                vec_insert(data->G_f[f][j], vec_get_at(data->per[f], j), euclidean_distance(vec_x, vec_j, c));
                pthread_mutex_unlock(&G_f_mutex);

                pos--;
                free(vec_x);
                free(vec_j);
            }
        } else {
            int threads = 0;
            pthread_mutex_lock(&G_f_mutex);
            if (data->R_stitched >= f_size) {
                //printf("R: %d\n", f_size);
                //printf("vmn, threads: %s, %d\n", vmn, threads);
                if (strcmp(data->vmn, "main") == 0) data->G_f[f] = Vamana_main(data_f, f_size, data->comps-2, data->L, f_size-1, data->a, &Vmedoid, threads);
                if (strcmp(data->vmn, "random")==0) data->G_f[f] = Vamana_random_medoid(data_f, f_size, data->comps-2, data->L, f_size-1, data->a, &Vmedoid, threads);
                if (strcmp(data->vmn, "semirandom")==0) data->G_f[f] = Vamana_semirandom_medoid(data_f, f_size, data->comps-2, data->L, f_size-1, data->a, &Vmedoid, threads);
            } else {
                //printf("R: %d\n", R_stitched);
                //printf("vmn, threads: %s, %d\n", vmn, threads);
                if (strcmp(data->vmn, "main")==0) data->G_f[f] = Vamana_main(data_f, f_size, data->comps-2, data->L, data->R_stitched, data->a, &Vmedoid, threads);
                if (strcmp(data->vmn, "random")==0) data->G_f[f] = Vamana_random_medoid(data_f, f_size, data->comps-2, data->L, data->R_stitched, data->a, &Vmedoid, threads);
                if (strcmp(data->vmn, "semirandom")==0) data->G_f[f] = Vamana_semirandom_medoid(data_f, f_size, data->comps-2, data->L, data->R_stitched, data->a, &Vmedoid, threads);
            }
            pthread_mutex_unlock(&G_f_mutex);
        }

        free_matrix_fvecs(data_f, data->comps-2);

    }

    pthread_exit(NULL);
}