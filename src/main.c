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

  // allocate memory for the graph G produced by the vamana algorithm
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

  // randomly select a query vector
  int xq_pos = rand() % (query_vectors - 1);
  float* xq = (float*)malloc(d_queries * sizeof(float));
  for (int i=0; i<d_queries; i++) {
    xq[i] = posible_queries[i][xq_pos];
  }

  // run the greedysearch algorithm to find its k nearest neighbours based on G
  int k = 25;
  Set V;
  Set knn = greedySearch(G, R, d_base, num_vectors, vectors, med, xq, L, k, &V);

  // print the set knn
  printf("knn_set: ");
  for (set_Node node = find_min(knn->root); node != SET_EOF; node = set_next(knn, node)) { 
      printf("%d ", node->value);
  }
  printf("\n");

  return 0;

}