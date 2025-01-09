#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <float.h>
#include <math.h>

#include "../algorithms/algorithms.h"

extern pthread_mutex_t filtered_data_mutex;
extern pthread_mutex_t G_f_mutex;

// Shared data structure for passing to threads

typedef struct {
    float** dataset;
    Vector** G_f;
    Map filtered_data;
    Vector* per;
    int f_start;
    int f_end;
    int vecs;
    int comps;
    int R_stitched;
    int L;
    int a;
    const char* vmn;
} ThreadData_stitch;

void* build_vamana_index(void* arg);