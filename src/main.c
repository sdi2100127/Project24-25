#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../algorithms/algorithms.h"
#include "../open_functions/open.h"

int main() {
  srand((unsigned int)time(NULL));

  // open base vectors file using ivecs_open
  const char* base_file = "siftsmall_base.fvecs";
  int num_vectors, d_base;
  float** dataset = fvecs_open(base_file, &num_vectors, &d_base);

  int vecs = 1000;
  float** vectors = (float**)malloc(d_base * sizeof(float*));
  for (int i = 0; i < d_base; i++) {
    vectors[i] = (float*)malloc(vecs * sizeof(float));
  }

  for (int j = 0; j<num_vectors && j < vecs; j++) {
    printf("vector %d: ", j);
    for (int i = 0; i<d_base && i < 5; i++) {
      vectors[i][j] = dataset[i][j];
      printf("%f ", vectors[i][j]);
    }
  }
  printf("\n");

  // open query vectors file using ivecs_open
  const char* query_file = "siftsmall_query.fvecs";
  int query_vectors, d_queries;
  float** posible_queries = fvecs_open(query_file, &query_vectors, &d_queries);
  
  // open groundtruth file using ivecs_open
  const char* groundtruth_file = "siftsmall_groundtruth.ivecs";
  int groundtruth_vectors, k;
  int** groundtruth_G = ivecs_open(groundtruth_file, &groundtruth_vectors, &k);

  printf("\n");

  int med = medoid(vectors, vecs, d_base);
  printf("medoid: %d\n", med);

  int R = 60;
  int** G = (int**)malloc(R * sizeof(int*));
  for (int i = 0; i < R; i++) {
    G[i] = (int*)malloc(vecs * sizeof(int));
  }

  int a = 2, L = 25;
  G = Vamana(vectors, vecs, d_base, L, R, a);

  printf("neighbours\n");
  for (int j = 0; j < num_vectors; j++) {
    printf("vector %d:", j);
    for (int i = 0; i < R; i++) {
        printf(" %d",  G[i][j]);
        
    }
    printf("\n");
  }

  // used for output
  // Set V;
  // Set knn_set = greedySearch(G, d, num_vectors, s, xq, k, L, V);

  // printf("k-nearest neighbours of %d according to the groundtruth\n", xq);
  // for (int i=0; i<k || i<20; i++) {
  //   printf("%d ", G[i][xq]);
  // }
  // printf("\n");

  // printf("k-nearest neighbours of %d according to greedy search\n", xq);
  // int count = 0;
  // for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) {
  //   printf("%d ", node->value);
  //   count++;
  //   if (count >= 20) break;
  // }
  // printf("\n");

  return 0;

}