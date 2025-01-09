#include "threads.h"
#include <string.h>

pthread_mutex_t med_mutex = PTHREAD_MUTEX_INITIALIZER;

float euclidean_distance(float* vec1, float* vec2, int comps) {
    float res = 0.0f;
    for (int i = 0; i < comps; i++) {
        float diff = vec1[i] - vec2[i];
        res += diff*diff;
    }
    res = sqrt(res);
    return res;
}

float squared_euclidean_distance(float* vec1, float* vec2, int comps) {
    float res = 0.0f;
    for (int i = 0; i < comps; i++) {
        float diff = vec1[i] - vec2[i];
        res += diff*diff;
    }
    
    return res;
}

void* calculate_distances(void* arg) {
    printf("hi from a thread!\n");
    ThreadData* data = (ThreadData*)arg;

    float* vec_p1 = (float*)malloc((data->comps-2) * sizeof(float));
    float* vec_p2 = (float*)malloc((data->comps-2) * sizeof(float));

    float min_sum = FLT_MAX;
    int min_p = -1;

    float dist;
    float dist_sum;

    for (int j= data->start; j< data->end; j++) {
        dist_sum = 0.0f;
        int count = 0;
        for (int i=2; i<data->comps; i++) {
            vec_p1[count] = data->dataset[i][j];
            count++;
        }

        for (int z = 0; z < data->vecs; z++) {
            count = 0;
            for (int i=2; i<data->comps; i++) {
                vec_p2[count] = data->dataset[i][z];
                count++;
            }

            // and add it to the total sum
            if (j != z)  {
                // compute the distance only if it has not been computed before
                if (data->dist_m[j][z] == 0) {
                    dist = squared_euclidean_distance(vec_p1, vec_p2, data->comps-2);
                    data->dist_m[j][z] = dist;
                    data->dist_m[z][j] = dist;
                }
                dist_sum = dist_sum + data->dist_m[j][z];
            }       
        }
        
        // then find which vector has the lowest total sum
        if (dist_sum <= min_sum) {
            min_sum = dist_sum;
            min_p = j;
        }
    }

    free(vec_p1);
    free(vec_p2);

    // Synchronize global medoid computation
    pthread_mutex_lock(&med_mutex);
    if (min_sum < data->med_sum) {
        data->med_sum = min_sum;
        data->med = min_p;
    }
    pthread_mutex_unlock(&med_mutex);

    pthread_exit(NULL);
}

void* calculate_distances_simple(void* arg) {
    printf("hi from a thread!\n");
    ThreadData* data = (ThreadData*)arg;

    float* vec_p1 = (float*)malloc((data->comps) * sizeof(float));
    float* vec_p2 = (float*)malloc((data->comps) * sizeof(float));

    float min_sum = FLT_MAX;
    int min_p = -1;

    float dist;
    float dist_sum;

    for (int j= data->start; j< data->end; j++) {
        dist_sum = 0.0f;
        for (int i=0; i<data->comps; i++) {
            vec_p1[i] = data->dataset[i][j];
        }

        for (int z = 0; z < data->vecs; z++) {
            for (int i=0; i<data->comps; i++) {
                vec_p2[i] = data->dataset[i][z];
            }

            // and add it to the total sum
            if (j != z)  {
                // compute the distance only if it has not been computed before
                if (data->dist_m[j][z] == 0) {
                    dist = squared_euclidean_distance(vec_p1, vec_p2, data->comps-2);
                    data->dist_m[j][z] = dist;
                    data->dist_m[z][j] = dist;
                }
                dist_sum = dist_sum + data->dist_m[j][z];
            }       
        }
        
        // then find which vector has the lowest total sum
        if (dist_sum <= min_sum) {
            min_sum = dist_sum;
            min_p = j;
        }
    }

    free(vec_p1);
    free(vec_p2);

    // Synchronize global medoid computation
    pthread_mutex_lock(&med_mutex);
    if (min_sum < data->med_sum) {
        data->med_sum = min_sum;
        data->med = min_p;
    }
    pthread_mutex_unlock(&med_mutex);

    pthread_exit(NULL);
}

void* calculate_distances_random(void* arg) {
    printf("hi from a thread!\n");
    ThreadData* data = (ThreadData*)arg;

    srand((unsigned int)time(NULL));

    float* vec_p1 = (float*)malloc((data->comps) * sizeof(float));
    float* vec_p2 = (float*)malloc((data->comps) * sizeof(float));

    float min_sum = FLT_MAX;
    int min_p = -1;

    float dist;
    float dist_sum;

    for (int j= data->start; j< data->end; j++) {
        dist_sum = 0.0f;
        for (int i=0; i<data->comps; i++) {
            vec_p1[i] = data->dataset[i][j];
        }

        for (int z = 0; z < data->vecs; z++) {
            for (int i=0; i<data->comps; i++) {
                vec_p2[i] = data->dataset[i][z];
            }

            // and add it to the total sum
            if (j != z)  {
                // compute the distance only if it has not been computed before
                if (data->dist_m[j][z] == 0) {
                    dist = squared_euclidean_distance(vec_p1, vec_p2, data->comps-2);
                    data->dist_m[j][z] = dist;
                    data->dist_m[z][j] = dist;
                }
                dist_sum = dist_sum + data->dist_m[j][z];
            }       
        }
        
        int x = rand() % (data->vecs - 1);
        if (x%2 == 0) {
            // if its an even number let it contribute to the medoid search
            // then find which vector has the lowest total sum
            if (dist_sum <= min_sum) {
                min_sum = dist_sum;
                min_p = j;
            }
        }
    }

    free(vec_p1);
    free(vec_p2);

    // Synchronize global medoid computation
    pthread_mutex_lock(&med_mutex);
    if (min_sum < data->med_sum) {
        data->med_sum = min_sum;
        data->med = min_p;
    }
    pthread_mutex_unlock(&med_mutex);

    pthread_exit(NULL);
}
