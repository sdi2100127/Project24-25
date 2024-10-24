#include <stdio.h>
#include <stdlib.h>

#include "../Data_Structs/Data_Structs.h"

// greedysearch is an algorithm that takes as input 
// G --> the groundtruth graph
// nn --> number of nearest neighbours / number of rows
// vecs --> number of query vectors / number of columns
// s --> a starting point in the graph
// xq --> the query vector
// k --> the result size
// L --> search list size
Set greedySearch(int** G, int nn, int vecs, int s, int xq, int k, int L, Set V);