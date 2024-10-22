#include <stdio.h>
#include <stdlib.h>

#include "algorithms/algorithms.h"
#include "open_functions/open.h"

int main() {
    // open groundtruth file using ivecs_open
    const char* filename = "siftsmall_groundtruth.ivecs";
    int num_vectors, d;
    int** G = ivecs_open(filename, &num_vectors, &d);

    // needed for input
    int s, xq;  // starting point, query vector 
    // used for output
    int k, L; // result size, search list size
    Set V;
    Set knn_set = greedySearch(G, d, num_vectors, s, xq, k, L, V);
    
    for (set_Node node = find_min(set->root); node != SET_EOF; node = set_next(set, node)) {
		printf("%d ", node->value);
	}
}