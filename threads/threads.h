#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <float.h>
#include <math.h>

#include "../open_functions/open.h"

float euclidean_distance(float* vec1, float* vec2, int comps);

float squared_euclidean_distance(float* vec1, float* vec2, int comps);

// Shared data structure for passing to threads
typedef struct {
    float** dataset;
    //double* query;
    float** dist_m;
    int med;
    float med_sum;
    int start;
    int end;
    int vecs;
    int comps;
} ThreadData;

extern pthread_mutex_t med_mutex;

void* calculate_distances(void* arg);

void* calculate_distances_simple(void* arg);

void* calculate_distances_random(void* arg);