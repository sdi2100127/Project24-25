#include "acutest.h"
#include <time.h>

#include "../open_functions/open.h"
#include "../algorithms/algorithms.h"

//#include "../Vamana/Vamana.h"

// compile example: cc testing.c ../open_functions/open.c ../Data_Structs/Data_Structs.c ../algorithms/algorithms.c -o testing -lm
// run: ./testing

// https://github.com/mity/acutest/blob/master/README.md
// There is a macro TEST_ASSERT which is very similar to TEST_CHECK but, if it fails, it aborts execution of the current unit test instantly.
// Therefore it should be used only if you understand the costs connected with such a brutal abortion of the test.
// In general, TEST_CHECK should be preferred over TEST_ASSERT, unless you know exactly what you do and why you chose TEST_ASSERT in some particular situation.

// SET TESTS 

void test_set_Create(void) {
   Set set = set_Create();

   TEST_ASSERT(set->root == NULL);
   TEST_ASSERT(set->size == 0);
}

void test_S_node_create(void) {
   int x = 5;
   set_Node node = S_node_create(x);

   TEST_ASSERT(node->value == x);
   TEST_ASSERT(node->left == NULL);
   TEST_ASSERT(node->right == NULL);
}

void test_S_node_insert(void) {
   Set set = set_Create();

   int x = 5, y=3, z=8, old_value;

   int flag = 0;
   set->root = S_node_insert(set->root, &flag, x, &old_value);
   if (flag == 1) set->size++;
   TEST_ASSERT(flag == 1);

   flag = 0;
   set->root = S_node_insert(set->root, &flag, y, &old_value);
   if (flag == 1) set->size++;
   TEST_ASSERT(flag == 1);

   flag = 0;
   set->root = S_node_insert(set->root, &flag, z, &old_value);
   if (flag == 1) set->size++;
   TEST_ASSERT(flag == 1);

   flag = 0;
   set->root = S_node_insert(set->root, &flag, z, &old_value);
   TEST_ASSERT(flag == 0);

   TEST_ASSERT(set->root->value == x);
   TEST_ASSERT(set->root->left->value == y);
   TEST_ASSERT(set->root->right->value == z);

}

void test_set_insert(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   TEST_ASSERT(set->root->value == x);
   TEST_ASSERT(set->root->left->value == y);
   TEST_ASSERT(set->root->right->value == z);
   TEST_ASSERT(set->size == 3);

   set_insert(set, y);
   TEST_ASSERT(set->size == 3);
}

void test_find_min(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_Node min_node = find_min(set->root);
   TEST_ASSERT(min_node->value == 3);
}

void test_find_max(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_Node max_node = find_max(set->root);
   TEST_ASSERT(max_node->value == 8);
}

void test_find_next(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_Node node = find_next(set->root, set->root);
   TEST_ASSERT(node->value == z);

   node = find_next(set->root, set->root->left);
   TEST_ASSERT(node->value == x);

   node = find_next(set->root, set->root->right);
   TEST_ASSERT(node == SET_EOF);
}

void test_set_next(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_Node node = set_next(set, set->root);
   TEST_ASSERT(node->value == z);

   node = set_next(set, set->root->left);
   TEST_ASSERT(node->value == x);

   node = set_next(set, set->root->right);
   TEST_ASSERT(node == SET_EOF);
}

void test_min_remove(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_Node min;
   set_Node new_root = min_remove(set->root, &min);
   TEST_ASSERT(min->value == y);
   TEST_ASSERT(new_root->value == set->root->value);

   set_insert(set, 6);
   new_root = min_remove(set->root, &min);
   TEST_ASSERT(min->value == 5);
   TEST_ASSERT(new_root->value == 8);
}

void test_S_remove(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   int old_value;
   int flag = 0;
   set->root = S_remove(set->root, x, &flag, &old_value);
   TEST_ASSERT(flag == 1);
   TEST_ASSERT(set->root->value == 8);

   flag = 0;
   set->root= S_remove(set->root, x, &flag, &old_value);
   TEST_ASSERT(flag == 0);
}

void test_set_remove(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_remove(set, x);
   TEST_ASSERT(set->size == 2);
   TEST_ASSERT(set->root->value == 8);

   set_remove(set, x);
   TEST_ASSERT(set->size == 2);
}

void test_S_find_equal(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_Node node = S_find_equal(set->root, x);
   TEST_ASSERT(node->value == x);

   node = S_find_equal(set->root, 6);
   TEST_ASSERT(node == SET_EOF);
}

//VECTOR FUNCTIONS TESTS

void test_vec_Create(void) {
   Vector vec = vec_Create(5);

   TEST_ASSERT(vec->size == 5);
   TEST_ASSERT(vec->capacity == 5);
}

void test_vec_insert(void) {
	Vector vec = vec_Create(0); 
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);
   vec_insert(vec, 410, dist);
   vec_insert(vec, 5610, dist);
   vec_insert(vec, 610, dist);
   vec_insert(vec, 610, dist);

   for (VecNode node = vec_first(vec); node != VECTOR_EOF; node = vec_next(vec, node)) {
      printf("%d ", node->value);
   }

   TEST_ASSERT(vec->size == 8);
   TEST_ASSERT(vec->array[(vec->size)-1].value == 610);
   TEST_ASSERT(vec->array[(vec->size)-1].dist == 0.0f);
}

void test_vec_remove(void){
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   vec_remove(vec);
   TEST_ASSERT(vec->size == 4);
   TEST_ASSERT(vec->array[vec->size-1].value == 510);

   vec_remove(vec);
   TEST_ASSERT(vec->size == 3);
   TEST_ASSERT(vec->array[vec->size-1].value == 310);
}

void test_vec_get_dist(void) {
   Vector vec = vec_Create(0);
   vec_insert(vec, 10, 0.0);
   vec_insert(vec, 20, 2.3);
   vec_insert(vec, 310, 1.2);

   TEST_ASSERT(vec_get_dist(vec, 0) == 0.0f);
   TEST_ASSERT(vec_get_dist(vec, 1) == 2.3f);
   TEST_ASSERT(vec_get_dist(vec, 2) == 1.2f);
}

void test_vec_get_at(void){
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   TEST_ASSERT(vec->array[2].value == 310);
   TEST_ASSERT(vec->array[1].value == 20);
   TEST_ASSERT(vec->array[vec->size-1].value == 1210);
}

void test_vec_set_at(void) {
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   vec_set_at(vec, 2, 250, 1.0);
   TEST_ASSERT(vec->array[2].value == 250);
   TEST_ASSERT(vec->array[2].dist == 1.0);
   vec_set_at(vec, vec->size-1, 35, 1.0);
   TEST_ASSERT(vec->array[vec->size-1].value == 35);
   TEST_ASSERT(vec->array[vec->size-1].dist == 1.0);
}

void test_vec_find_node(void) {
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   TEST_ASSERT(vec_find_node(vec, 510)->value == 510);
   TEST_ASSERT(vec_find_node(vec, 1210)->value == 1210);
   TEST_ASSERT(vec_find_node(vec, 3) == VECTOR_EOF);
}

void test_vec_find_pos(void) {
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   TEST_ASSERT(vec_find_pos(vec, 510) == 3);
   TEST_ASSERT(vec_find_pos(vec, 1210) == 4);
   TEST_ASSERT(vec_find_pos(vec, 3) == -1);
}

void test_vec_first(void) {
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   TEST_ASSERT(vec_first(vec)->value == 10);
}

void test_vec_last(void) {
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   TEST_ASSERT(vec_last(vec)->value == 1210);
}

void test_vec_next(void) {
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   TEST_ASSERT(vec_next(vec, &vec->array[0])->value == 20);
   TEST_ASSERT(vec_next(vec, &vec->array[1])->value == 310);
}

// PRIORITY QUEUE TESTS

void test_pqueue_create(void) {
   Vector vec = vec_Create(0);
   vec_insert(vec, 10, 0.0);
   vec_insert(vec, 20, 1.0);
   vec_insert(vec, 310, 2.0);
   vec_insert(vec, 510, 3.0);
   vec_insert(vec, 1210, 4.0);

   PQueue pq = pqueue_create(vec);
   TEST_ASSERT(pq != NULL);
   TEST_ASSERT(pq->vector != NULL);
   TEST_ASSERT(pq->vector->size == 5);
   TEST_ASSERT(v_node_value(pq, 1) == 1210);
}

void test_pqueue_insert(void) {
   Vector vec = vec_Create(0);

   vec_insert(vec, 10, 0.0);
   vec_insert(vec, 20, 1.0);
   vec_insert(vec, 310, 2.0);
   vec_insert(vec, 510, 3.0);
   vec_insert(vec, 1210, 4.0);
   PQueue pq = pqueue_create(vec);

   TEST_ASSERT(pq->vector->size == 5);
   TEST_ASSERT(v_node_value(pq, 1) == 1210);

   pqueue_insert(pq, 45, 1.5);
   
   TEST_ASSERT(v_node_value(pq, 1) == 1210);
   TEST_ASSERT(pq->vector->size == 6);

   pqueue_insert(pq, 45, 1.5);
   TEST_ASSERT(pq->vector->size == 6);
}

void test_pqueue_remove(void) {
   Vector vec = vec_Create(0);
   vec_insert(vec, 10, 0.0);
   vec_insert(vec, 20, 1.0);
   vec_insert(vec, 310, 2.0);
   vec_insert(vec, 510, 3.0);
   vec_insert(vec, 1210, 4.0);

   PQueue pq = pqueue_create(vec);

   TEST_ASSERT(pq->vector->size == 5);
   TEST_ASSERT(v_node_value(pq, 1) == 1210);

   pqueue_remove(pq);

   TEST_ASSERT(v_node_value(pq, 1) == 510);
   TEST_ASSERT(pq->vector->size == 4);
}

// OPEN FUNCTION TESTS
// mike path: /home/mike/Documents/Project24-25/open_functions/siftsmall


void test_free_matrix_fvecs(void) {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 3;
   int vecs = 5;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   srand((unsigned int)time(NULL));

   float a = 10.0;
   for (int j = 0; j < vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vectors[i][j] = ((float)rand()/(float)(RAND_MAX)) * a;
      }
   }

   free_matrix_fvecs(vectors, dim);
   // we cannot really test further because after free the pointers just point to free memory
   // and trying to access them would just lead to undefined behavior

}

void test_free_matrix_ivecs(void) {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 3;
   int vecs = 5;
   int** vectors = (int**)malloc(dim * sizeof(int*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (int*)malloc(vecs * sizeof(int));
   }

   srand((unsigned int)time(NULL));

   int a = 20;
   for (int j = 0; j < vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vectors[i][j] = rand() % a;
      }
   }

   free_matrix_ivecs(vectors, dim);
   // we cannot really test further because after free the pointers just point to free memory
   // and trying to access them would just lead to undefined behavior

}

void test_open_fvecs(void) {
   const char* filename = "siftsmall_base.fvecs"; // Specify your fvecs file
   // open the file
   FILE *fp = NULL;
   char path[100];
   sprintf(path, "open_functions/siftsmall/%s", filename);
   fp = fopen(path, "rb");

   TEST_ASSERT(fp != NULL); // check that file was opened successfully

   // read the dimention off of the first 4 bytes of the file
   int dim;
   fread(&dim, sizeof(int), 1, fp);
   
   int vec_size = sizeof(int) + dim * sizeof(float);
   // now we move the pointer to the end of the file
   fseek(fp, 0, SEEK_END);
   // then we retrieve its position to determine the total size of the file 
   long file_size = ftell(fp);
   fseek(fp, sizeof(int), SEEK_SET); // and we go back to the start
   int vecs = file_size / vec_size;

   // call fvecs_open
   int num_vectors, d;
   float** vectors = fvecs_open(filename, &num_vectors, &d);
   
   TEST_ASSERT(vectors != NULL); // check if the vectors matrix was created
   TEST_ASSERT(d == dim); // check if dimention is the same
   TEST_ASSERT(num_vectors == vecs); // check if the number of vectors is the same
   
   
   // for the first few vectors, check that they are infact the same
   for (int j = 0; j<num_vectors && j < 20; j++) {
      float* vec = (float*)malloc(vec_size); 
      fread(vec, vec_size, 1, fp);
      for (int i = 0; i<d && i < 5; i++) {
         TEST_ASSERT(vectors[i][j] == vec[i + 1]);
      }
   }

   // Free the allocated memory
   free_matrix_fvecs(vectors, d);
}

void test_open_ivecs(void) {
   const char* filename = "siftsmall_groundtruth.ivecs"; // Specify your fvecs file
   // open the file
   FILE *fp = NULL;
   char path[100];
   sprintf(path, "open_functions/siftsmall/%s", filename);
   fp = fopen(path, "rb");

   TEST_ASSERT(fp != NULL); // check that file was opened successfully

   // read the dimention off of the first 4 bytes of the file
   int dim;
   fread(&dim, sizeof(int), 1, fp);
   
   int vec_size = sizeof(int) + dim * sizeof(float);
   // now we move the pointer to the end of the file
   fseek(fp, 0, SEEK_END);
   // then we retrieve its position to determine the total size of the file 
   long file_size = ftell(fp);
   fseek(fp, sizeof(int), SEEK_SET); // and we go back to the start
   int vecs = file_size / vec_size;

   // call fvecs_open
   int num_vectors, d;
   int** vectors = ivecs_open(filename, &num_vectors, &d);
   
   TEST_ASSERT(vectors != NULL); // check if the vectors matrix was created
   TEST_ASSERT(d == dim); // check if dimention is the same
   TEST_ASSERT(num_vectors == vecs); // check if the number of vectors is the same
   
   
   // for the first few vectors, check that they are infact the same
   for (int j = 0; j<num_vectors && j < 20; j++) {
      int* vec = (int*)malloc(vec_size); 
      fread(vec, vec_size, 1, fp);
      for (int i = 0; i<d && i < 5; i++) {
         TEST_ASSERT(vectors[i][j] == vec[i + 1]);
      }
   }

   // Free the allocated memory
   free_matrix_ivecs(vectors, d);
}

void test_euclidean_distance(void) {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 3;
   float* vec1 = (float*)malloc(dim * sizeof(float));
   float* vec2 = (float*)malloc(dim * sizeof(float));

   vec1[0] = 5.0; vec1[1] = 3.0; vec1[2] = 8.0;
   vec2[0] = 4.0; vec2[1] = 9.0; vec2[2] = 2.0;

   // Define a small tolerance value
   float tolerance = 0.000001;

   float dist = euclidean_distance(vec1, vec2, dim);
   TEST_ASSERT(fabs(dist - 8.544003) < tolerance);

}

void test_greedySearch(void) {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 3;
   int vecs = 5;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   float* xq = (float*)malloc(dim * sizeof(float*));

   vectors[0][0] = 4.0; vectors[1][0] = 6.0; vectors[2][0] = 9.0;
   vectors[0][1] = 4.0; vectors[1][1] = 5.0; vectors[2][1] = 4.0;
   vectors[0][2] = 2.0; vectors[1][2] = 8.0; vectors[2][2] = 4.0;
   vectors[0][3] = 1.0; vectors[1][3] = 5.0; vectors[2][3] = 8.0;
   vectors[0][4] = 2.0; vectors[1][4] = 5.0; vectors[2][4] = 0.0;
   xq[0] = 9.0; xq[1] = 4.0; xq[2] = 9.0;

   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < dim; i++) {
         printf(" %f", vectors[i][j]);
      }
      printf("\n");
   }

   printf("vector xq:");
   for (int i = 0; i < dim; i++) {
      printf(" %f", xq[i]);
   }
   printf("\n");

   int R = 3;

   Vector* G = (Vector*)malloc(vecs * sizeof(Vector));
   for (int i=0; i<vecs; i++) {
      G[i] = vec_Create(0);
   }

   // we check the vamana for G:
   // vector0: 1 2 3 
   // vector1: 3 2 0
   // vector2: 1 3 4
   // vector3: 2 4 1
   // vector4: 2 3 0

   float* vec1 = (float*)malloc(dim * sizeof(float));
   float* vec2 = (float*)malloc(dim * sizeof(float));

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][0];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j == 1 || j==2 || j==3) vec_insert(G[0], j, euclidean_distance(vec1, vec2, dim));
   }
   printf("vec1: ");
   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][1];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j ==2 || j==3 || j==0) vec_insert(G[1], j, euclidean_distance(vec1, vec2, dim));
   }
   

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][2];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j ==3 || j==4 || j==1) vec_insert(G[2], j, euclidean_distance(vec1, vec2, dim));
   }

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][3];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j == 1 || j==4 || j==2) vec_insert(G[3], j, euclidean_distance(vec1, vec2, dim));
   }

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][4];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j ==2 || j==3 || j==0) vec_insert(G[4], j, euclidean_distance(vec1, vec2, dim));
   }

   free(vec1);
   free(vec2);
   
   printf("neighbours\n");
   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < R; i++) {
         printf(" %d, %f ",  vec_get_at(G[j], i), vec_get_dist(G[j], i));
      }
      printf("\n");
   }

   int s = rand() % (vecs-1), L = 4, k = 3;
   printf("s: %d\n", s);
   Set V;
   PQueue knn = greedySearch(G, R, dim, vecs, vectors, s, xq, L, k, &V);

   TEST_ASSERT(V->size == 4);
   TEST_ASSERT(knn->vector->size == 3);

   Set V_test = set_Create();
   for (int i=0; i<4; i++) set_insert(V_test, i);
   for (set_Node node = find_min(V_test->root); node != SET_EOF; node = set_next(V_test, node)) {
      TEST_ASSERT(S_find_equal(V->root, node->value) != SET_EOF);
   }

   Set knn_test = set_Create();
   set_insert(knn_test, 0);
   set_insert(knn_test, 1);
   set_insert(knn_test, 3);
   for (set_Node node = find_min(knn_test->root); node != SET_EOF; node = set_next(knn_test, node)) {
      TEST_ASSERT(vec_find_node(knn->vector, node->value) != VECTOR_EOF);
   }

}

void test_RobustPrune(void) {
   srand((unsigned int)time(NULL));

   // run the test for a vector matrix of 7 vectors with 3 components each
   int dim = 3;
   int vecs = 5;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   vectors[0][0] = 4.0; vectors[1][0] = 6.0; vectors[2][0] = 9.0;
   vectors[0][1] = 4.0; vectors[1][1] = 5.0; vectors[2][1] = 4.0;
   vectors[0][2] = 2.0; vectors[1][2] = 8.0; vectors[2][2] = 4.0;
   vectors[0][3] = 1.0; vectors[1][3] = 5.0; vectors[2][3] = 8.0;
   vectors[0][4] = 2.0; vectors[1][4] = 5.0; vectors[2][4] = 0.0;

   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < dim; i++) {
         printf(" %f", vectors[i][j]);
      }
      printf("\n");
   }

   int R = 3;

   Vector* G = (Vector*)malloc(vecs * sizeof(Vector));
   for (int i=0; i<vecs; i++) {
      G[i] = vec_Create(0);
   }

   // we check the vamana for G:
   // vector0: 1 2 3 
   // vector1: 3 2 0
   // vector2: 1 3 4
   // vector3: 2 4 1
   // vector4: 2 3 0

   float* vec1 = (float*)malloc(dim * sizeof(float));
   float* vec2 = (float*)malloc(dim * sizeof(float));

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][0];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j == 1 || j==2 || j==3) vec_insert(G[0], j, euclidean_distance(vec1, vec2, dim));
   }
   printf("vec1: ");
   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][1];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j ==2 || j==3 || j==0) vec_insert(G[1], j, euclidean_distance(vec1, vec2, dim));
   }
   

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][2];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j ==3 || j==4 || j==1) vec_insert(G[2], j, euclidean_distance(vec1, vec2, dim));
   }

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][3];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j == 1 || j==4 || j==2) vec_insert(G[3], j, euclidean_distance(vec1, vec2, dim));
   }

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][4];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j ==2 || j==3 || j==0) vec_insert(G[4], j, euclidean_distance(vec1, vec2, dim));
   }

   free(vec1);
   free(vec2);
   
   printf("neighbours\n");
   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < R; i++) {
         printf(" %d, %f ",  vec_get_at(G[j], i), vec_get_dist(G[j], i));
      }
      printf("\n");
   }

   int p = 0;
   int s = 1, L = 4, k = 3;
   printf("s: %d\n", s);
   printf("p: %d\n", p);
   Set V;
   int a = 1;

   float* xq = (float*)malloc(dim * sizeof(float*));
   for(int i = 0; i < dim; i++ ){
      xq[i] = vectors[i][p];
   }
   printf("vector xq:");
   for (int i = 0; i < dim; i++) {
      printf(" %f", xq[i]);
   }
   printf("\n");

   // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
   float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
   for (int i = 0; i < vecs; i++) {
      dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
   }

   int med = medoid(vectors, vecs, dim, &dist_matrix);

   PQueue knn = greedySearch(G ,R ,dim ,vecs ,vectors ,s ,xq ,L ,k ,&V);

   RobustPrune(&G, p , &V,  a,  R,  dim ,  vecs , vectors, dist_matrix);
   TEST_ASSERT(V->size == 0);

   int* test_N_out = (int*)malloc(R * sizeof(int*));
   test_N_out[0] = 3; test_N_out[1] = -1; test_N_out[2] = -1;


   TEST_ASSERT(G[p]->size == 1);
   for (int i=0; i<G[p]->size; i++) {
      TEST_ASSERT(test_N_out[i] ==  vec_get_at(G[p], i));
   }
}

void test_medoid(void) {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 3;
   int vecs = 5;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   vectors[0][0] = 4.0; vectors[1][0] = 6.0; vectors[2][0] = 9.0;
   vectors[0][1] = 4.0; vectors[1][1] = 5.0; vectors[2][1] = 4.0;
   vectors[0][2] = 2.0; vectors[1][2] = 8.0; vectors[2][2] = 4.0;
   vectors[0][3] = 1.0; vectors[1][3] = 5.0; vectors[2][3] = 8.0;
   vectors[0][4] = 2.0; vectors[1][4] = 5.0; vectors[2][4] = 0.0;

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

   int med = medoid(vectors, vecs, dim, &dist_matrix);
   TEST_ASSERT(med == 1);
}

void test_Vamana(void) {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 3;
   int vecs = 5;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   vectors[0][0] = 4.0; vectors[1][0] = 6.0; vectors[2][0] = 9.0;
   vectors[0][1] = 4.0; vectors[1][1] = 5.0; vectors[2][1] = 4.0;
   vectors[0][2] = 2.0; vectors[1][2] = 8.0; vectors[2][2] = 4.0;
   vectors[0][3] = 1.0; vectors[1][3] = 5.0; vectors[2][3] = 8.0;
   vectors[0][4] = 2.0; vectors[1][4] = 5.0; vectors[2][4] = 0.0;

   int L = 4, R = 3, a = 1;

   Vector* G = (Vector*)malloc(vecs * sizeof(Vector));

   G = Vamana(vectors, vecs, dim, L, R, a);

   int** G_test = (int**)malloc(R * sizeof(int*));
   for (int i = 0; i < R; i++) {
      G_test[i] = (int*)malloc(vecs * sizeof(int));
   }

   G_test[0][0] = 3; G_test[1][0] = -1;  G_test[2][0] = -1;
   G_test[0][1] = 2; G_test[1][1] = 4;  G_test[2][1] = 3;
   G_test[0][2] = 1; G_test[1][2] = -1;  G_test[2][2] = -1;
   G_test[0][3] = 0; G_test[1][3] = 1;  G_test[2][3] = -1;
   G_test[0][4] = 1; G_test[1][4] = -1;  G_test[2][4] = -1;

   for (int j=0; j<vecs; j++) {
      printf("Nout(%d): ", j);
      for (int i=0; i<G[j]->size; i++) {
         printf("%d ", vec_get_at(G[j], i));
      }
      printf("\n");
   }

   for (int j=0; j<vecs; j++) {
      for (int i=0; i<G[j]->size; i++) {
         TEST_ASSERT(vec_get_at(G[j], i) == G_test[i][j]);
      }
   }

}

TEST_LIST = {
   { "set_Create", test_set_Create },
   { "S_node_create", test_S_node_create },
   { "S_node_insert", test_S_node_insert },
   { "set_insert", test_set_insert },
   { "find_min", test_find_min },
   { "find_max", test_find_max },
   { "find_next", test_find_next },
   { "set_next", test_set_next },
   { "min_remove", test_min_remove },
   { "S_remove", test_S_remove },
   { "set_remove", test_set_remove },
   { "S_find_equal", test_S_find_equal },
   { "vec_Create", test_vec_Create },
   { "vec_insert", test_vec_insert },
   { "vec_remove", test_vec_remove },
   { "vec_get_dist", test_vec_get_dist },
   { "vec_get_at", test_vec_get_at },
   { "vec_set_at", test_vec_set_at },
   { "vec_find_node", test_vec_find_node },
   { "vec_find_pos", test_vec_find_pos },
   { "vec_first", test_vec_first },
   { "vec_last", test_vec_last },
   { "vec_next", test_vec_next },
   { "pqueue_create", test_pqueue_create },
   { "pqueue_insert", test_pqueue_insert },
   { "pqueue_remove", test_pqueue_remove },
   { "free_matrix_fvecs", test_free_matrix_fvecs },
   { "free_matrix_ivecs", test_free_matrix_ivecs },
   { "open_fvecs", test_open_fvecs },
   { "open_ivecs", test_open_ivecs },
   { "euclidean_distance", test_euclidean_distance },
   { "greedySearch", test_greedySearch },
   { "RobustPrune", test_RobustPrune },
   { "medoid", test_medoid },
   { "Vamana", test_Vamana },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};
