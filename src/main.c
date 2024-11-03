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

  int vecs = 100;
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
  int groundtruth_vectors, k_n;
  int** groundtruth_G = ivecs_open(groundtruth_file, &groundtruth_vectors, &k_n);

  printf("\n");

  int med = medoid(vectors, vecs, d_base);
  printf("medoid: %d\n", med);

  // allocate memory for the graph G produced by the vamana algorithm
  int R = 60;
  Vector* G = (Vector*)malloc(vecs * sizeof(Vector));

  int a = 2, L = 25;
  G = Vamana(vectors, vecs, d_base, L, R, a);

  printf("neighbours\n");
  for (int j = 0; j < vecs; j++) {
    printf("vector %d:", j);
    for (int i = 0; i < R; i++) {
      printf(" %d",  vec_get_at(G[j], i));
        
    }
    printf("\n");
  }

  // randomly select a query vector
  int xq_pos = rand() % (query_vectors - 1);
  float* xq = (float*)malloc(d_queries * sizeof(float));
  for (int i=0; i<d_queries; i++) {
    xq[i] = posible_queries[i][xq_pos];
  }
  printf("%d\n", xq_pos);

  // run the greedysearch algorithm to find its k nearest neighbours based on G
  int k = 100;
  Set V;
  PQueue knn = greedySearch(G, R, d_base, vecs, vectors, med, xq, L, k, &V);

  // print the set knn
  printf("knn_set: ");
  Vector knn_vec = knn->vector;
  for (VecNode node = vec_first(knn_vec); node != VECTOR_EOF; node = vec_next(knn_vec, node)) {
    printf("%d ", node->value);
  }
  printf("\n");

  return 0;

}