#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../algorithms/algorithms.h"
#include "../open_functions/open.h"

int main() {
  srand((unsigned int)time(NULL));
  
  // open groundtruth file using ivecs_open
  const char* filename = "siftsmall_groundtruth.ivecs";
  int num_vectors, d;
  int** G = ivecs_open(filename, &num_vectors, &d);

  // needed for input
  int x = 100;
  int s = rand() % x, xq = rand() % x;  // starting point, query vector
  printf("starting point: %d, query point: %d\n", s, xq);
  int k = 100, L = 150; // result size, search list size

  // used for output
  Set V;
  Set knn_set = greedySearch(G, d, num_vectors, s, xq, k, L, V);

  printf("k-nearest neighbours of %d according to the groundtruth\n", xq);
  for (int i=0; i<k || i<20; i++) {
    printf("%d ", G[i][xq]);
  }
  printf("\n");

  printf("k-nearest neighbours of %d according to greedy search\n", xq);
  int count = 0;
  for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) {
    printf("%d ", node->value);
    count++;
    if (count >= 20) break;
  }
  printf("\n");

  return 0;

}