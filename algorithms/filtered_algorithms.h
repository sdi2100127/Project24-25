#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include "algorithms.h"
#include "../threads_filtered/threads_filt.h"

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
PQueue FilteredGreedySearch(Vector* G, int R, int dim, int vecs, float** vectors, float* xq, int fq, Map filter_med, int med, int L, int k, Set* V);

//robust prune is an algorithm that takes as input:
//G --> the graph we have to modify and return, that sets at most R new neighboors for p
//p --> the point we have to find the neighboors for
//V --> our canditcate set
//a --> the distance threshold that is necessarily bigger or equal to 1
//R --> the the degree bound for our selection
//neigh_count --> a count that we have to keep how many neighboors we have added tg p
void FilteredRobustPrune(Vector** G, int p ,Set* V, int a, int R , int dim , int vecs , float** vectors, float** dist_m);

// same as normal medoid, with the difference that it ignores the first two components of each vector
// seeing as they are not the actual vector
int FilteredMedoid(float** dataset, int vecs, int comps, float*** dist_m);

// function to find the medoid of a dataset
Map FindMedoid(float** dataset, int vecs, float min_f, float max_f, Map filtered_data, int t);

// We have created two implementation of the vamana algorithm, one for testing and one to use in the main program
// This was done because in oreder to test vamana we had to have predetermined data, such as the permutation of the
// vectors indexes and the neighbours in graph G, and that goes against the vamana's algorithm random initiallizations

// Vamana algorithm
Vector* FilteredVamanaIndexing(float** dataset, float min_f, float max_f, int vecs, int comps, int q_comps, int L, int R, int a, Map* med, int* medoid, int t, int thread_flag);

// Vamana algorithm with random graph
Vector* FilteredVamanaIndexing_randomG(float** dataset, float min_f, float max_f, int vecs, int comps, int q_comps, int L, int R, int neigh, int a, Map* med, int* medoid, int t, int thread_flag);

// Stiched Vamana algorithm
Vector** StichedVamanaIndexing(float** dataset, float min_f, float max_f, Set filters, int vecs, int comps, int L, int R, int a, Map* med, int* medoid, int t, Map* filt_data, Vector** permutation, int threads, const char* vmn, int threads_count);

// Stiched Vamana algorithm with random graph
Vector** StichedVamanaIndexing_randomG(float** dataset, float min_f, float max_f, Set filters, int vecs, int comps, int L, int R, int a, Map* med, int* medoid, int t, int neigh, Map* filt_data, Vector** permutation, int threads, const char* vmn, int threads_count);

// function to compute the groundtruth on which the results will be based 
Vector* Groundtruth(float** dataset, int vecs, int comps, float** queries, int vecs_q, int comps_q, int k);

void free_G_f(Vector** G_f, int num_filters, Map filtered_data);