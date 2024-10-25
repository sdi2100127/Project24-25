#include "acutest.h"
#include <time.h>

#include "../open_functions/open.h"
#include "../algorithms/algorithms.h"

// compile example: cc testing.c ../open_functions/open.c -o testing
// run: ./testing

// https://github.com/mity/acutest/blob/master/README.md
// There is a macro TEST_ASSERT which is very similar to TEST_CHECK but, if it fails, it aborts execution of the current unit test instantly.
// Therefore it should be used only if you understand the costs connected with such a brutal abortion of the test.
// In general, TEST_CHECK should be preferred over TEST_ASSERT, unless you know exactly what you do and why you chose TEST_ASSERT in some particular situation.

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
   for (int i = 0; i < dim; i++) {
   //    vec1[i] = ((float)rand()/(float)(RAND_MAX)) * a;
   //    vec2[i] = ((float)rand()/(float)(RAND_MAX)) * a;
       printf(" %f    %f\n", vec1[i], vec2[i]);
   }
   // printf("\n");

   float dist = euclidean_distance(vec1, vec2, dim);
   printf("distance %f", dist);
   //TEST_ASSERT(dist == 8.544003);

}

void test_greedySearch(void) {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 3;
   int vecs = 5;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   srand((unsigned int)time(NULL));

   float a = 10.0;
   printf("\n");
   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < dim; i++) {
         vectors[i][j] = ((float)rand()/(float)(RAND_MAX)) * a;
         printf(" %f", vectors[i][j]);
      }
      printf("\n");
   }

   int R = 3;
   int** random = (int**)malloc(R * sizeof(int*));
   for (int i = 0; i < R; i++) {
      random[i] = (int*)malloc(vecs * sizeof(int));
   }

   printf("neighbours\n");
   int x;
   int r = 4;
   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < R; i++) {

         int stop = 1;
         while (stop == 1) {
            x = rand() % r;
            stop = 0;
            for (int z = 0; z < i; z++) {
               if (x == random[z][j]) {
                  stop = 1;
                  break;
               }
            }    
         }
         random[i][j] = x;
         printf(" %d", random[i][j]);
         
      }
      printf("\n");
   }



}

TEST_LIST = {
   { "free_matrix_fvecs", test_free_matrix_fvecs },
   { "free_matrix_ivecs", test_free_matrix_ivecs },
   { "open_fvecs", test_open_fvecs },
   { "open_ivecs", test_open_ivecs },
   { "euclidean_distance", test_euclidean_distance },
   { "greedySearch", test_greedySearch },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};