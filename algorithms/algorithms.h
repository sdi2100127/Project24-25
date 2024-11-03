#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../Data_Structs/Data_Structs.h"

float euclidean_distance(float* vec1, float* vec2, int comps);

float squared_euclidean_distance(float* vec1, float* vec2, int comps);

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

//robust prune is an algorithm that takes as input:
//G --> the graph we have to modify and return, that sets at most R new neighboors for p
//p --> the point we have to find the neighboors for
//V --> our canditcate set
//a --> the distance threshold that is necessarily bigger or equal to 1
//R --> the the degree bound for our selection
//neigh_count --> a count that we have to keep how many neighboors we have added tg p
void RobustPrune(int*** G, int p ,Set* V, int a, int R , int* neigh_count, int dim , int vecs , float** vectors);

// function to find the medoid of a dataset
int medoid(float** dataset, int vecs, int comps);

// Vamana algorithm
int** Vamana(float** dataset, int vecs, int comps, int L, int R, int a);