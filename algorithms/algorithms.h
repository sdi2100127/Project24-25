#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../Data_Structs/Data_Structs.h"

float euclidean_distance(float* vec1, float* vec2, int comps);

// greedysearch is an algorithm that takes as input 
// G --> the search graph
// dim --> the number of components
// vecs --> the number of vectors
// vectors --> the vectors corresponding to each of the nodes of G
// s --> a starting point in the graph
// xq --> the query vector
// k --> the result size/number of desired nearest neighbours
Set greedySearch(int** G, int dim, int vecs, float** vectors, int s, float* xq, int k, Set V);
