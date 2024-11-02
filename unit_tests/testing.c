#include "acutest.h"
#include <time.h>

#include "../open_functions/open.h"
#include "../algorithms/algorithms.h"
//#include "../Vamana/Vamana.h"

// compile example: cc testing.c ../open_functions/open.c ../Data_Structs/Data_Structs.c ../algorithms/algorithms.c ../Vamana/Vamana.c -o testing -lm
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
   sprintf(path, "/home/mike/Documents/Project24-25/open_functions/siftsmall/%s", filename);
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
   sprintf(path, "/home/mike/Documents/Project24-25/open_functions/siftsmall/%s", filename);
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

   //srand((unsigned int)time(NULL));

   // float a = 10.0;
   // printf("\n");
   // for (int i = 0; i < dim; i++) {
   // //    vec1[i] = ((float)rand()/(float)(RAND_MAX)) * a;
   // //    vec2[i] = ((float)rand()/(float)(RAND_MAX)) * a;
   //     printf(" %f    %f\n", vec1[i], vec2[i]);
   // }
   // printf("\n");

   // Define a small tolerance value
   float tolerance = 0.000001;

   float dist = euclidean_distance(vec1, vec2, dim);
   // printf("distance %f", dist);
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
   int** G = (int**)malloc(R * sizeof(int*));
   for (int i = 0; i < R; i++) {
      G[i] = (int*)malloc(vecs * sizeof(int));
   }

   G[0][0] = 1; G[1][0] = 2;  G[2][0] = 3;
   G[0][1] = 2; G[1][1] = 3;  G[2][1] = 0;
   G[0][2] = 3; G[1][2] = 4;  G[2][2] = 1;
   G[0][3] = 1; G[1][3] = 4;  G[2][3] = 2;
   G[0][4] = 2; G[1][4] = 3;  G[2][4] = 0;


   printf("neighbours\n");
   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < R; i++) {
         printf(" %d",  G[i][j]);
         
      }
      printf("\n");
   }

   int s = rand() % (vecs-1), L = 4, k = 3;
   printf("s: %d\n", s);
   Set V;
   Set knn = greedySearch  (G, R, dim, vecs, vectors, s, xq, L, k, &V);

   TEST_ASSERT(V->size == 4);
   TEST_ASSERT(knn->size == 3);

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
      TEST_ASSERT(S_find_equal(knn->root, node->value) != SET_EOF);
   }

   // set_destroy()

}

void test_RobustPrune(void){
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
   int** G = (int**)malloc(R * sizeof(int*));
   for (int i = 0; i < R; i++) {
      G[i] = (int*)malloc(vecs * sizeof(int));
   }

   G[0][0] = 1;   G[1][0] = 2;  G[2][0] = 3;
   G[0][1] = 2;   G[1][1] = 3;  G[2][1] = 0;
   G[0][2] = 3;   G[1][2] = 4;  G[2][2] = 1;
   G[0][3] = 1;   G[1][3] = 4;  G[2][3] = 2;
   G[0][4] = 2;   G[1][4] = 3;  G[2][4] = 0;


   printf("neighbours\n");
   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < R; i++) {
         printf(" %d",  G[i][j]);
         
      }
      printf("\n");
   }

   // rand() % (vecs-1)
   int p = 3;
   int s = rand() % (vecs-1),L = 4, k = 3;
   printf("s: %d\n", s);
   printf("p: %d\n", p);
   Set V;
   int neigh_count;
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

   Set knn = greedySearch(G ,R ,dim ,vecs ,vectors ,s ,xq ,L ,k ,&V);

   RobustPrune(&G, p , &V,  a,  R , &neigh_count,  dim ,  vecs , vectors);
   TEST_ASSERT(V->size == 0);


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

   int med = medoid(vectors, vecs, dim);
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

   int** G = (int**)malloc(R * sizeof(int*));
   for (int i = 0; i < R; i++) {
      G[i] = (int*)malloc(vecs * sizeof(int));
   }

   G = Vamana(vectors, vecs, dim, L, R, a);

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
