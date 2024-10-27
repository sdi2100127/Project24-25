#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../Data_Structs/Data_Structs.h"

float euclidean_distance(float* vec1, float* vec2, int comps);

// greedysearch is an algorithm that takes as input 
// G --> the search graph (a random R-regular directed graph)
// R --> the random number of neighbours correspondeing to each vector
// vecs --> the number of vectors in both vectors and G as they have the same indexing
// dim --> the number of components of each vector in vectors
// vectors --> the vectors corresponding to each of the nodes of G
// s --> a starting point in the graph
// xq --> the query vector
// k --> the result size/number of desired nearest neighbours
// L --> the maximum size of the result set
Set greedySearch(int** G, int R, int dim, int vecs, float** vectors, int s, float* xq, int L, int k, Set* V);
