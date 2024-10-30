#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//#include "../Data_Structs/Data_Structs.h"
#include "../algorithms/algorithms.h"

// function to find the medoid of a dataset
int medoid(float** dataset, int vecs, int comps);

// Vamana algorithm
int** Vamana(float** dataset, int vecs, int comps, int L, int R, int a);