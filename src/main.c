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

  int vecs = 5000;
  float** vectors = (float**)malloc(d_base * sizeof(float*));
  for (int i = 0; i < d_base; i++) {
    vectors[i] = (float*)malloc(vecs * sizeof(float));
  }

  for (int j = 0; j<num_vectors && j < vecs; j++) {
    //printf("vector %d: ", j);
    for (int i = 0; i<d_base; i++) {
      vectors[i][j] = dataset[i][j];
      //printf("%f ", vectors[i][j]);
    }
  }
  printf("\n");

  // open query vectors file using ivecs_open
  const char* query_file = "siftsmall_query.fvecs";
  int query_vectors, d_queries;
  float** posible_queries = fvecs_open(query_file, &query_vectors, &d_queries);

  printf("\n");
  
  // open groundtruth file using ivecs_open
  const char* groundtruth_file = "siftsmall_groundtruth.ivecs";
  int groundtruth_vectors, k_n;
  int** groundtruth_G = ivecs_open(groundtruth_file, &groundtruth_vectors, &k_n);

  printf("\n");

  // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
  float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
  for (int i = 0; i < vecs; i++) {
      dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
  }

  for(int i=0; i<vecs; i++) {
    for(int j=0; j<vecs; j++) {
        dist_matrix[i][j] = 0;
    }
  }

  int med = medoid(vectors, vecs, d_base, &dist_matrix);
  printf("medoid: %d\n", med);
  return 0;
  //int med;

  // allocate memory for the graph G produced by the vamana algorithm
  int R = 60; // 1000: 6, 10000: 60

  int a = 1, L = 25; // 1000: 7, 10000: 25
  Vector* G = Vamana_main(vectors, vecs, d_base, L, R, a, &med);

  printf("G\n");
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
  int k = 10;
  Set V;
  PQueue knn = greedySearch(G, R, d_base, vecs, vectors, med, xq, L, k, &V);

  printf("GROUNDTRUTH knn_set under %d: ", vecs);
  int* test_knn = (int*)malloc(k * sizeof(int));
  int count = 0;
  for (int i=0; i<k_n; i++) {
    int n_point = groundtruth_G[i][xq_pos];
    if (n_point < vecs) {
      test_knn[count] = n_point;
      printf("%d ", test_knn[count]);
      count++;
    }
    if (count == k) break;
    
  }
  printf("\n");

  free_matrix_fvecs(dataset, d_base);
  free_matrix_fvecs(vectors, d_base);
  free_matrix_fvecs(posible_queries, d_queries);
  free_matrix_ivecs(groundtruth_G, k_n);
  free_matrix_fvecs(dist_matrix, vecs);
  free_G(G, vecs);
  free(xq);
  free(test_knn);
  pqueue_destroy(knn);
  set_destroy(V);

  return 0;

}