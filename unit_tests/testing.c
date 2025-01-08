#include "acutest.h"
#include <time.h>
#include <float.h>

//#include "../open_functions/open.h"
//#include "../algorithms/algorithms.h"
#include "../algorithms/filtered_algorithms.h"


// https://github.com/mity/acutest/blob/master/README.md
// There is a macro TEST_ASSERT which is very similar to TEST_CHECK but, if it fails, it aborts execution of the current unit test instantly.
// Therefore it should be used only if you understand the costs connected with such a brutal abortion of the test.
// In general, TEST_CHECK should be preferred over TEST_ASSERT, unless you know exactly what you do and why you chose TEST_ASSERT in some particular situation.

// SET TESTS 

void test_set_Create(void) {
   Set set = set_Create();

   TEST_ASSERT(set->root == NULL);
   TEST_ASSERT(set->size == 0);

   set_destroy(set);
}

void test_S_node_create(void) {
   int x = 5;
   set_Node node = S_node_create(x);

   TEST_ASSERT(node->value == x);
   TEST_ASSERT(node->left == NULL);
   TEST_ASSERT(node->right == NULL);

   S_destroy(node);
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

   set_destroy(set);

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

   set_destroy(set);
}

void test_find_min(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_Node min_node = find_min(set->root);
   TEST_ASSERT(min_node->value == 3);

   set_destroy(set);
}

void test_find_max(void) {
   Set set = set_Create();
   int x = 5, y=3, z=8;
   set_insert(set, x);
   set_insert(set, y);
   set_insert(set, z);

   set_Node max_node = find_max(set->root);
   TEST_ASSERT(max_node->value == 8);

   set_destroy(set);
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

   set_destroy(set);
   S_destroy(node);
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

   set_destroy(set);
   S_destroy(node);
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

   int h = 6;
   set_insert(set, h);
   new_root = min_remove(set->root, &min);
   TEST_ASSERT(min->value == 5);
   TEST_ASSERT(new_root->value == 8);

   set_destroy(set);
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

   set_destroy(set);

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

   set_destroy(set);
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

   set_destroy(set);
   S_destroy(node);
}

//VECTOR FUNCTIONS TESTS

void test_vec_Create(void) {
   Vector vec = vec_Create(5);

   TEST_ASSERT(vec->size == 5);
   TEST_ASSERT(vec->capacity == 5);

   vec_destroy(vec);
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

   vec_destroy(vec);
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

   vec_destroy(vec);
}

void test_vec_get_dist(void) {
   Vector vec = vec_Create(0);
   vec_insert(vec, 10, 0.0);
   vec_insert(vec, 20, 2.3);
   vec_insert(vec, 310, 1.2);

   TEST_ASSERT(vec_get_dist(vec, 0) == 0.0f);
   TEST_ASSERT(vec_get_dist(vec, 1) == 2.3f);
   TEST_ASSERT(vec_get_dist(vec, 2) == 1.2f);

   vec_destroy(vec);
}

void test_vec_get_at(void){
   Vector vec = vec_Create(0);
   float dist = 0.0f;
   vec_insert(vec, 10, dist);
   vec_insert(vec, 20, dist);
   vec_insert(vec, 310, dist);
   vec_insert(vec, 510, dist);
   vec_insert(vec, 1210, dist);

   TEST_ASSERT(vec_get_at(vec, 2) == 310);
   TEST_ASSERT(vec_get_at(vec, 1) == 20);
   TEST_ASSERT(vec_get_at(vec, vec->size-1) == 1210);

   vec_destroy(vec);
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

   vec_destroy(vec);
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

   vec_destroy(vec);
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

   vec_destroy(vec);
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

   vec_destroy(vec);
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

   vec_destroy(vec);
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

   vec_destroy(vec);
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

   pqueue_destroy(pq);
   vec_destroy(vec);
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

   pqueue_destroy(pq);
   vec_destroy(vec);
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

   pqueue_destroy(pq);
   vec_destroy(vec);
}


// HASHMAP TESTS

void test_map_create() {
   Map map = map_create(0, 10);

   TEST_ASSERT(map != NULL);
   TEST_ASSERT(map->capacity == 53); 
   TEST_ASSERT(map->size == 0);
   TEST_ASSERT(map->deleted == 0);

   map_destroy(map);
}

void test_map_insert() {
   Map map = map_create(0, 100);
   TEST_ASSERT(map->capacity == 193); 

   map_insert(map, 10, 5);
   TEST_ASSERT(map->size == 1);
   TEST_ASSERT(map->array[10].key == 10);
   TEST_ASSERT(map->array[10].state == OCCUPIED);
   TEST_ASSERT(vec_find_node(map->array[10].values, 5) != VECTOR_EOF);

   map_insert(map, 20, 10);
   TEST_ASSERT(map->size == 2);
   TEST_ASSERT(map->array[20].key == 20);
   TEST_ASSERT(map->array[20].state == OCCUPIED);
   TEST_ASSERT(vec_find_node(map->array[20].values, 10) != VECTOR_EOF);

   map_insert(map, 10, 15); 
   TEST_ASSERT(map->size == 2);
   TEST_ASSERT(map->array[10].key == 10);
   TEST_ASSERT(map->array[10].state == OCCUPIED);
   TEST_ASSERT(vec_find_node(map->array[10].values, 15) != VECTOR_EOF);

   map_destroy(map);
}

void test_map_find_node() {
   Map map = map_create(0, 100);

   map_insert(map, 10, 5);
   map_insert(map, 20, 10);
   map_insert(map, 10, 15); 

   MapNode node = map_find_node(map, 10);
   TEST_ASSERT(node->key == 10);
   TEST_ASSERT(vec_find_node(map->array[10].values, 5) != VECTOR_EOF);
   TEST_ASSERT(vec_find_node(map->array[10].values, 15) != VECTOR_EOF);
   TEST_ASSERT(node->state == OCCUPIED);

   node = map_find_node(map, 15);
   TEST_ASSERT(node == MAP_EOF);

   map_destroy(map);
}

void test_map_remove() {
   Map map = map_create(0, 100);

   map_insert(map, 10, 5);
   map_insert(map, 20, 10);
   map_insert(map, 10, 15); 

   map_remove(map, 10);
   MapNode node = map_find_node(map, 10);
   TEST_ASSERT(node == MAP_EOF);
   TEST_ASSERT(map->deleted == 1);
   TEST_ASSERT(map->size == 1);

   map_remove(map, 20);
   node = map_find_node(map, 20);
   TEST_ASSERT(node == MAP_EOF);
   TEST_ASSERT(map->deleted == 2);
   TEST_ASSERT(map->size == 0);

   map_destroy(map);
}

void test_map_find_values() {
   Map map = map_create(0, 100);

   map_insert(map, 10, 5);
   map_insert(map, 20, 10);
   map_insert(map, 10, 15); 

   Vector vec = map_find_values(map, 10);
   TEST_ASSERT(vec->size == 2);
   TEST_ASSERT(vec_find_node(map->array[10].values, 15) != VECTOR_EOF);
   TEST_ASSERT(vec_find_node(map->array[10].values, 5) != VECTOR_EOF);

   vec = map_find_values(map, 20);
   TEST_ASSERT(vec->size == 1);

   vec = map_find_values(map, 15);
   TEST_ASSERT(vec == NULL);

   map_destroy(map);
}

void test_map_first() {
   Map map = map_create(0, 100);

   map_insert(map, 10, 5);
   map_insert(map, 20, 10);
   map_insert(map, 10, 15); 

   MapNode first = map_first(map);
   TEST_ASSERT(first->key == 10);

   map_remove(map, 10);
   first = map_first(map);
   TEST_ASSERT(first->key == 20);

   map_destroy(map);
}

void test_map_next() {
   Map map = map_create(0, 100);

   map_insert(map, 10, 5);
   map_insert(map, 20, 10);
   map_insert(map, 10, 15); 

   MapNode first = map_first(map);
   TEST_ASSERT(first->key == 10);

   MapNode next = map_next(map, first);
   TEST_ASSERT(next->key == 20);

   next = map_next(map, next);
   TEST_ASSERT(next == MAP_EOF);

   map_destroy(map);
}

void test_rehash() {

   Map map = map_create(0, 5); 
   TEST_ASSERT(map->capacity == 53);

   // Insert enough elements to exceed MAX_LOAD_FACTOR
   for (int i = 0; i < 50; i++) {
      map_insert(map, i, i * 10);
   }

   // Verify the map has rehashed
   TEST_ASSERT(map->capacity == 97);

   // Verify all elements are still accessible
   for (int i = 0; i < 50; i++) {
      Vector values = map_find_values(map, i);
      TEST_ASSERT(values != NULL);
      //TEST_ASSERT(set_contains(values, i * 10));
   }

   map_destroy(map);
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
   float* vec = (float*)malloc(vec_size);
   for (int j = 0; j<num_vectors && j < 20; j++) {
      fread(vec, vec_size, 1, fp);
      for (int i = 0; i<d && i < 5; i++) {
         TEST_ASSERT(vectors[i][j] == vec[i + 1]);
      }
   }
   free(vec);

   fclose(fp);

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
   int* vec = (int*)malloc(vec_size);
   for (int j = 0; j<num_vectors && j < 20; j++) { 
      fread(vec, vec_size, 1, fp);
      for (int i = 0; i<d && i < 5; i++) {
         TEST_ASSERT(vectors[i][j] == vec[i + 1]);
      }
   }
   free(vec);

   fclose(fp);

   // Free the allocated memory
   free_matrix_ivecs(vectors, d);
}

void test_data_open(void) {
   const char* filename = "dummy-data.bin"; // Specify your fvecs file
   // open the file
   FILE *fp = NULL;
   char path[100];
   sprintf(path, "dummies/%s", filename);
   fp = fopen(path, "rb");

   TEST_ASSERT(fp != NULL); // check that file was opened successfully

   // read the dimention off of the first 4 bytes of the file
   int vecs;
   fread(&vecs, sizeof(int), 1, fp);
   
   int vec_num_d = 100;
   int vec_size = (2 + vec_num_d) * sizeof(float);

   // call fvecs_open
   int num_vectors;
   float min, max;
   Set filters;
   float** vectors = data_open(filename, &num_vectors, vec_num_d, &min, &max, &filters);
   printf("we have %d different filters\n", filters->size);

   TEST_ASSERT(vectors != NULL); // check if the vectors matrix was created
   TEST_ASSERT(num_vectors == vecs); // check if the number of vectors is the same
   
   
   // for the first few vectors, check that they are infact the same
   float* vec = (float*)malloc(vec_size);
   float max_filter = FLT_MIN;
   float min_filter = FLT_MAX;
   for (int j = 0; j<num_vectors; j++) {
      fread(vec, vec_size, 1, fp);
      //printf("vector %d: ", j);
      for (int i = 0; i<vec_num_d+2; i++) {
         //printf("%f ", vectors[i][j]);
         if (i == 0) {
            if (vectors[i][j] <= min_filter) min_filter = vectors[i][j];
            if (vectors[i][j] >= max_filter) max_filter = vectors[i][j];
         }
         TEST_ASSERT(vectors[i][j] == vec[i]);
      }
      //printf("\n");
   }
   printf("min: %f , max: %f\n", min_filter, max_filter);
   TEST_ASSERT(min == min_filter);
   TEST_ASSERT(max == max_filter);
   free(vec);

   fclose(fp);

   // Free the allocated memory
   free_matrix_fvecs(vectors, vec_num_d+2);
   set_destroy(filters);
}

void test_query_open(void) {
   const char* filename = "dummy-queries.bin"; // Specify your fvecs file
   // open the file
   FILE *fp = NULL;
   char path[100];
   sprintf(path, "dummies/%s", filename);
   fp = fopen(path, "rb");

   TEST_ASSERT(fp != NULL); // check that file was opened successfully

   // read the query number off of the first 4 bytes of the file
   int queries;
   fread(&queries, sizeof(int), 1, fp);
   
   int vec_num_d = 100;
   int vec_size = (4 + vec_num_d) * sizeof(float);

   // call fvecs_open
   int num_q;
   int count;
   float** vectors = query_open(filename, &num_q, vec_num_d , &count);
   
   TEST_ASSERT(vectors != NULL); // check if the vectors matrix was created
   TEST_ASSERT(num_q == queries); // check if the number of vectors is the same
   
   
   // for the first few vectors, check that they are infact the same
   float* vec = (float*)malloc(vec_size);
   int count_j = 0;
   for (int j = 0; j<queries; j++) {
      fread(vec, vec_size, 1, fp);
      if(vec[0] == 0.0 || vec[0] == 1.0 ) {
         count_j++;
      }
   }
   TEST_ASSERT(count == count_j);
   printf("Usable count: %d \n", count);
   free(vec);

   fclose(fp);

   // Free the allocated memory
   free_matrix_fvecs(vectors, vec_num_d+4);
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

   free(vec1);
   free(vec2);
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
   set_Node node;
   for (node = find_min(V_test->root); node != SET_EOF; node = set_next(V_test, node)) {
      TEST_ASSERT(S_find_equal(V->root, node->value) != SET_EOF);
   }

   Set knn_test = set_Create();
   set_insert(knn_test, 0);
   set_insert(knn_test, 1);
   set_insert(knn_test, 3);
   for (node = find_min(knn_test->root); node != SET_EOF; node = set_next(knn_test, node)) {
      TEST_ASSERT(vec_find_node(knn->vector, node->value) != VECTOR_EOF);
   }

   free(xq);
   free_matrix_fvecs(vectors, dim);
   set_destroy(V);
   set_destroy(V_test);
   set_destroy(knn_test);
   pqueue_destroy(knn);
   free_G(G, vecs);

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

   for(int i=0; i<vecs; i++) {
      for(int j=0; j<vecs; j++) {
         dist_matrix[i][j] = 0;
      }
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

   free(xq);
   free_matrix_fvecs(vectors, dim);
   free_matrix_fvecs(dist_matrix, vecs);
   set_destroy(V);
   free(test_N_out);
   pqueue_destroy(knn);
   free_G(G, vecs);
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

   free_matrix_fvecs(vectors, dim);
   free_matrix_fvecs(dist_matrix, vecs);
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

   int L = 4, R = 3, a = 1, med;

   Vector* G = Vamana(vectors, vecs, dim, L, R, a);

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

   free_matrix_fvecs(vectors, dim);
   free_G(G, vecs);
   free_matrix_ivecs(G_test, R);

}

void test_FilteredMedoid(void) {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 5; // 0 --> C, 1--> T, 2-4--> vector components
   int vecs = 5;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   // randomly select C an T attributes
   for(int j=0; j<vecs; j++) {
      vectors[0][j] = (float)(rand() % 100);
      vectors[1][j] = (float)rand()/(float)(RAND_MAX/100);
   }
   

   vectors[2][0] = 4.0; vectors[3][0] = 6.0; vectors[4][0] = 9.0;
   vectors[2][1] = 4.0; vectors[3][1] = 5.0; vectors[4][1] = 4.0;
   vectors[2][2] = 2.0; vectors[3][2] = 8.0; vectors[4][2] = 4.0;
   vectors[2][3] = 1.0; vectors[3][3] = 5.0; vectors[4][3] = 8.0;
   vectors[2][4] = 2.0; vectors[3][4] = 5.0; vectors[4][4] = 0.0;

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

   int med = FilteredMedoid(vectors, vecs, dim, &dist_matrix);
   TEST_ASSERT(med == 1);

   free_matrix_fvecs(vectors, dim);
   free_matrix_fvecs(dist_matrix, vecs);
}

void test_FindMedoid() {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 5; // 0 --> C, 1--> T, 2-4--> vector components
   int vecs = 6;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   // randomly select T attribute
   for(int j=0; j<vecs; j++) {
      vectors[1][j] = (float)rand()/(float)(RAND_MAX/100);
   }
   
   vectors[0][0] = 1.0;
   vectors[0][1] = 1.0;
   vectors[0][2] = 4.0;
   vectors[0][3] = 2.0;
   vectors[0][4] = 2.0;
   vectors[0][5] = 2.0;

   vectors[2][0] = 4.0; vectors[3][0] = 6.0; vectors[4][0] = 9.0;
   vectors[2][1] = 4.0; vectors[3][1] = 5.0; vectors[4][1] = 4.0;
   vectors[2][2] = 2.0; vectors[3][2] = 8.0; vectors[4][2] = 4.0;
   vectors[2][3] = 1.0; vectors[3][3] = 5.0; vectors[4][3] = 8.0;
   vectors[2][4] = 2.0; vectors[3][4] = 5.0; vectors[4][4] = 0.0;
   vectors[2][5] = 3.0; vectors[3][5] = 5.0; vectors[4][5] = 1.0;

   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < dim; i++) {
         printf("%f ",  vectors[i][j]);
      }
      printf("\n");
   }

   // create the filtered data map
   float min_f = 1.0, max_f = 4.0;
   Map filtered_data = map_create(min_f, max_f);

   for (int j=0; j<vecs; j++) {
      map_insert(filtered_data, (int)vectors[0][j], j);
   }

   Vector values;
   for (MapNode node = map_first(filtered_data); node != MAP_EOF; node = map_next(filtered_data, node)) {
      printf("filter %d: ", node->key);
      values = node->values;
      for (VecNode node = vec_first(values); node != VECTOR_EOF; node = vec_next(values, node)) {
         printf("%d ", node->value);
      }
      printf("\n");
   }

   Map M = FindMedoid(vectors, vecs, min_f, max_f, filtered_data, 1);

   int test_M[3];
   test_M[0] = 0; test_M[1] = 3; test_M[2] = 2;

   int count = 0;
   for (MapNode node = map_first(M); node != MAP_EOF; node = map_next(M, node)) {
      printf("filter %d: ", node->key);
      values = node->values;
      for (VecNode vnode = vec_first(values); vnode != VECTOR_EOF; vnode = vec_next(values, vnode)) {
         printf("%d ", vnode->value);
         //TEST_ASSERT(node->value == test_M[count]);
         TEST_ASSERT(vec_find_node(map_find_values(filtered_data, node->key), vnode->value) != VECTOR_EOF);
      }
      count++; 
      printf("\n");
   }

   free_matrix_fvecs(vectors, dim);
   map_destroy(M);
   map_destroy(filtered_data);
}

void test_FilteredGreedySearch() {
   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 5; // 0 --> C, 1--> T, 2-4--> vector components
   int vecs = 6;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   // randomly select T attribute
   for(int j=0; j<vecs; j++) {
      vectors[1][j] = (float)rand()/(float)(RAND_MAX/100);
   }
   
   vectors[0][0] = 1.0;
   vectors[0][1] = 1.0;
   vectors[0][2] = 4.0;
   vectors[0][3] = 2.0;
   vectors[0][4] = 2.0;
   vectors[0][5] = 2.0;

   vectors[2][0] = 4.0; vectors[3][0] = 6.0; vectors[4][0] = 9.0;
   vectors[2][1] = 4.0; vectors[3][1] = 5.0; vectors[4][1] = 4.0;
   vectors[2][2] = 2.0; vectors[3][2] = 8.0; vectors[4][2] = 4.0;
   vectors[2][3] = 1.0; vectors[3][3] = 5.0; vectors[4][3] = 8.0;
   vectors[2][4] = 2.0; vectors[3][4] = 5.0; vectors[4][4] = 0.0;
   vectors[2][5] = 3.0; vectors[3][5] = 5.0; vectors[4][5] = 1.0;

   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < dim; i++) {
         printf("%f ",  vectors[i][j]);
      }
      printf("\n");
   }

   // create the filtered data map
   float min_f = 1.0, max_f = 4.0;
   Map filtered_data = map_create(min_f, max_f);

   for (int j=0; j<vecs; j++) {
      map_insert(filtered_data, (int)vectors[0][j], j);
   }

   Vector values;
   for (MapNode node = map_first(filtered_data); node != MAP_EOF; node = map_next(filtered_data, node)) {
      printf("filter %d: ", node->key);
      values = node->values;
      for (VecNode node = vec_first(values); node != VECTOR_EOF; node = vec_next(values, node)) {
         printf("%d ", node->value);
      }
      printf("\n");
   }

   Map M = FindMedoid(vectors, vecs, min_f, max_f, filtered_data, 1);
   int count = 0;
   for (MapNode node = map_first(M); node != MAP_EOF; node = map_next(M, node)) {
      printf("filter %d: ", node->key);
      values = node->values;
      for (VecNode node = vec_first(values); node != VECTOR_EOF; node = vec_next(values, node)) {
         printf("%d ", node->value);
      }
      count++;
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
   // vector5: 3 2 4

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

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][5];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j ==3 || j==2 || j==4) vec_insert(G[5], j, euclidean_distance(vec1, vec2, dim));
   }

   free(vec1);
   free(vec2);

   printf("neighbours\n");
   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < R; i++) {
         printf(" %d",  vec_get_at(G[j], i));
      }
      printf("\n");
   }

   float* xq = (float*)malloc(dim * sizeof(float*));
   xq[0] = 2.0; xq[1] = (float)rand()/(float)(RAND_MAX/100); xq[2] = 9.0; xq[3] = 4.0; xq[4] = 9.0;
   printf("vector xq:");
   for (int i = 0; i < dim; i++) {
      printf(" %f", xq[i]);
   }
   printf("\n");

   int L = 4, k = 1;
   // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
   float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
   for (int i = 0; i < vecs; i++) {
      dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
   }

   // and initiallize it with zeros
   for(int i=0; i<vecs; i++) {
      for(int j=0; j<vecs; j++) {
         dist_matrix[i][j] = 0;
      }
   }
   int medoid = FilteredMedoid(vectors, vecs, dim, &dist_matrix);
   Set V;
   PQueue knn = FilteredGreedySearch(G, R, dim, vecs, vectors, xq, (int)xq[0], M, medoid, L, k, &V);

   TEST_ASSERT(V->size == 2);
   TEST_ASSERT(knn->vector->size == 1);

   Set V_test = set_Create();
   set_insert(V_test, 3); set_insert(V_test, 4);
   set_Node node;
   for (node = find_min(V_test->root); node != SET_EOF; node = set_next(V_test, node)) {
      TEST_ASSERT(S_find_equal(V->root, node->value) != SET_EOF);
   }

   Set knn_test = set_Create();
   set_insert(knn_test, 3);
   for (node = find_min(knn_test->root); node != SET_EOF; node = set_next(knn_test, node)) {
      TEST_ASSERT(vec_find_node(knn->vector, node->value) != VECTOR_EOF);
   }

   free_matrix_fvecs(dist_matrix, vecs);
   free_matrix_fvecs(vectors, dim);
   free(xq);
   map_destroy(M);
   map_destroy(filtered_data);
   free_G(G, vecs);
   set_destroy(V);
   pqueue_destroy(knn);
   set_destroy(V_test);
   set_destroy(knn_test);
}

void test_FilteredRobustPrune() {
   srand((unsigned int)time(NULL));

   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 5; // 0 --> C, 1--> T, 2-4--> vector components
   int vecs = 6;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   // randomly select T attribute
   for(int j=0; j<vecs; j++) {
      vectors[1][j] = (float)rand()/(float)(RAND_MAX/100);
   }
   
   vectors[0][0] = 1.0;
   vectors[0][1] = 1.0;
   vectors[0][2] = 4.0;
   vectors[0][3] = 2.0;
   vectors[0][4] = 2.0;
   vectors[0][5] = 2.0;

   vectors[2][0] = 4.0; vectors[3][0] = 6.0; vectors[4][0] = 9.0;
   vectors[2][1] = 4.0; vectors[3][1] = 5.0; vectors[4][1] = 4.0;
   vectors[2][2] = 2.0; vectors[3][2] = 8.0; vectors[4][2] = 4.0;
   vectors[2][3] = 1.0; vectors[3][3] = 5.0; vectors[4][3] = 8.0;
   vectors[2][4] = 2.0; vectors[3][4] = 5.0; vectors[4][4] = 0.0;
   vectors[2][5] = 3.0; vectors[3][5] = 5.0; vectors[4][5] = 1.0;

   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < dim; i++) {
         printf("%f ",  vectors[i][j]);
      }
      printf("\n");
   }

   // create the filtered data map
   float min_f = 1.0, max_f = 4.0;
   Map filtered_data = map_create(min_f, max_f);

   for (int j=0; j<vecs; j++) {
      map_insert(filtered_data, (int)vectors[0][j], j);
   }

   Vector values;
   for (MapNode node = map_first(filtered_data); node != MAP_EOF; node = map_next(filtered_data, node)) {
      printf("filter %d: ", node->key);
      values = node->values;
      for (VecNode node = vec_first(values); node != VECTOR_EOF; node = vec_next(values, node)) {
         printf("%d ", node->value);
      }
      printf("\n");
   }

   Map M = FindMedoid(vectors, vecs, min_f, max_f, filtered_data, 1);
   int count = 0;
   for (MapNode node = map_first(M); node != MAP_EOF; node = map_next(M, node)) {
      printf("filter %d: ", node->key);
      values = node->values;
      for (VecNode node = vec_first(values); node != VECTOR_EOF; node = vec_next(values, node)) {
         printf("%d ", node->value);
      }
      count++;
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
   // vector5: 3 2 4

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

   for (int i = 0; i < dim; i++) {
      vec1[i] = vectors[i][5];
   }
   for(int j=0; j<vecs; j++) {
      for (int i = 0; i < dim; i++) {
         vec2[i] = vectors[i][j];
      }
      if (j ==3 || j==2 || j==4) vec_insert(G[5], j, euclidean_distance(vec1, vec2, dim));
   }

   free(vec1);
   free(vec2);

   printf("neighbours\n");
   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < R; i++) {
         printf(" %d",  vec_get_at(G[j], i));
      }
      printf("\n");
   }

   float* xq = (float*)malloc(dim * sizeof(float*));
   xq[0] = 2.0; xq[1] = (float)rand()/(float)(RAND_MAX/100); xq[2] = 9.0; xq[3] = 4.0; xq[4] = 9.0;
   printf("vector xq:");
   for (int i = 0; i < dim; i++) {
      printf(" %f", xq[i]);
   }
   printf("\n");

   int L = 4, k = 1, p = 0, a = 1;
   
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

   int med = FilteredMedoid(vectors, vecs, dim, &dist_matrix);

   Set V;
   PQueue knn = FilteredGreedySearch(G, R, dim, vecs, vectors, xq, (int)xq[0], M, med, L, k, &V);

   FilteredRobustPrune(&G, p , &V,  a,  R,  dim ,  vecs , vectors, dist_matrix);

   int* test_N_out = (int*)malloc(R * sizeof(int*));
   test_N_out[0] = 3; test_N_out[1] = 1; test_N_out[2] = 2;


   TEST_ASSERT(G[p]->size == 3);
   for (int i=0; i<G[p]->size; i++) {
      TEST_ASSERT(test_N_out[i] ==  vec_get_at(G[p], i));
   }

   free(xq);
   free_matrix_fvecs(vectors, dim);
   free_matrix_fvecs(dist_matrix, vecs);
   set_destroy(V);
   free(test_N_out);
   pqueue_destroy(knn);
   free_G(G, vecs);
   map_destroy(M);
   map_destroy(filtered_data);
}

void test_FilteredVamanaIndexing(void) {
   srand((unsigned int)time(NULL));

   // run the test for a vector matrix of 5 vectors with 3 components each
   int dim = 5; // 0 --> C, 1--> T, 2-4--> vector components
   int vecs = 6;
   float** vectors = (float**)malloc(dim * sizeof(float*));
   for (int i = 0; i < dim; i++) {
      vectors[i] = (float*)malloc(vecs * sizeof(float));
   }

   // randomly select T attribute
   for(int j=0; j<vecs; j++) {
      vectors[1][j] = (float)rand()/(float)(RAND_MAX/100);
   }
   
   vectors[0][0] = 1.0;
   vectors[0][1] = 1.0;
   vectors[0][2] = 4.0;
   vectors[0][3] = 2.0;
   vectors[0][4] = 2.0;
   vectors[0][5] = 2.0;

   vectors[2][0] = 4.0; vectors[3][0] = 6.0; vectors[4][0] = 9.0;
   vectors[2][1] = 4.0; vectors[3][1] = 5.0; vectors[4][1] = 4.0;
   vectors[2][2] = 2.0; vectors[3][2] = 8.0; vectors[4][2] = 4.0;
   vectors[2][3] = 1.0; vectors[3][3] = 5.0; vectors[4][3] = 8.0;
   vectors[2][4] = 2.0; vectors[3][4] = 5.0; vectors[4][4] = 0.0;
   vectors[2][5] = 3.0; vectors[3][5] = 5.0; vectors[4][5] = 1.0;

   for (int j = 0; j < vecs; j++) {
      printf("vector %d:", j);
      for (int i = 0; i < dim; i++) {
         printf("%f ",  vectors[i][j]);
      }
      printf("\n");
   }

   // create the filtered data map
   float min_f = 1.0, max_f = 4.0;

   int R = 3;

   float* xq = (float*)malloc((dim+2) * sizeof(float*));
   xq[0] = 1.0; xq[1] = 2.0; xq[2] = -1.0;  xq[3] = -1.0;xq[4] = 9.0; xq[5] = 4.0; xq[6] = 9.0;
   printf("vector xq:");
   for (int i = 0; i < dim; i++) {
      printf(" %f", xq[i]);
   }
   printf("\n");

   int L = 4, k = 1, p = 0, a = 1, neigh = 5, t = 1, medoid, threads = 0;
   Map med;
   Vector* G = FilteredVamanaIndexing(vectors, min_f, max_f, vecs, dim, dim+2, L, R, a, &med, &medoid, t, threads);

   int** G_test = (int**)malloc(R * sizeof(int*));
   for (int i = 0; i < R; i++) {
      G_test[i] = (int*)malloc(vecs * sizeof(int));
   }

   // G_test[0][0] = 1; G_test[1][0] = -1;  G_test[2][0] = -1;
   // G_test[0][1] = 2; G_test[1][1] = 4;  G_test[2][1] = 3;
   // G_test[0][2] = 1; G_test[1][2] = -1;  G_test[2][2] = -1;
   // G_test[0][3] = 0; G_test[1][3] = 1;  G_test[2][3] = -1;
   // G_test[0][4] = 1; G_test[1][4] = -1;  G_test[2][4] = -1;

   for (int j=0; j<vecs; j++) {
      printf("Nout(%d): ", j);
      for (int i=0; i<G[j]->size; i++) {
         printf("%d ", vec_get_at(G[j], i));
      }
      printf("\n");
   }

   // for (int j=0; j<vecs; j++) {
   //    for (int i=0; i<G[j]->size; i++) {
   //       TEST_ASSERT(vec_get_at(G[j], i) == G_test[i][j]);
   //    }
   // }

   map_destroy(med);
   free(xq);
   free_matrix_fvecs(vectors, dim);
   free_G(G, vecs);
   free_matrix_ivecs(G_test, R);
}

void test_Groundtruth() {
   // open base vectors file using data_open
   const char* base_file = "dummy-data.bin";
   int num_vectors, d_base = 100;
   float min_f, max_f;
   Set filters;
   float** dataset = data_open(base_file, &num_vectors, d_base, &min_f, &max_f, &filters);
   int data_dim = d_base+2;

   for (int j = 0; j < num_vectors; j++) {
      if (dataset[0][j] == 18) {printf("filter %f found in vec: %d\n", dataset[0][j], j);}
   }

   // open query vectors file using query_open
   const char* query_file = "dummy-queries.bin";
   int query_vectors, count, d_queries = 100;
   float** posible_queries = query_open(query_file, &query_vectors, d_queries, &count);
   int queries_dim = d_queries+4;

   int vecs = 100, k = 100;

   printf("query_vectors that will be used: %d\n", count);

   Vector* groundtruth = Groundtruth(dataset, vecs, data_dim, posible_queries, count, queries_dim, k);

   for (int j=0; j<count && j<20; j++) {
      // make sure the correct amount of neighbours has been found
      TEST_ASSERT(groundtruth[j]->size <= k);
      for (VecNode node = vec_first(groundtruth[j]); node != VECTOR_EOF; node = vec_next(groundtruth[j], node)) {
         int idx = node->value;
         // check that the filters of all the neighbours selected are the same
         if (posible_queries[1][j] != -1)
            TEST_ASSERT(dataset[0][idx] == posible_queries[1][j]);
      }
   }

   free_G(groundtruth, count);
   free_matrix_fvecs(posible_queries, queries_dim);
   free_matrix_fvecs(dataset, data_dim);
   set_destroy(filters);
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
   { "map_create", test_map_create },
   { "map_insert", test_map_insert },
   { "map_find_node", test_map_find_node },
   { "map_remove", test_map_remove },
   { "map_find_values", test_map_find_values },
   { "map_first", test_map_first },
   { "map_next", test_map_next },
   { "rehash", test_rehash },
   { "free_matrix_fvecs", test_free_matrix_fvecs },
   { "free_matrix_ivecs", test_free_matrix_ivecs },
   { "open_fvecs", test_open_fvecs },
   { "open_ivecs", test_open_ivecs },
   { "data_open", test_data_open },
   { "query_open", test_query_open },
   { "euclidean_distance", test_euclidean_distance },
   { "greedySearch", test_greedySearch },
   { "RobustPrune", test_RobustPrune },
   { "medoid", test_medoid },
   { "Vamana", test_Vamana },
   { "FilteredMedoid", test_FilteredMedoid },
   { "FindMedoid", test_FindMedoid },
   { "FilteredGreedySearch", test_FilteredGreedySearch },
   { "FilteredRobustPrune", test_FilteredRobustPrune },
   { "FilteredVamanaIndexing", test_FilteredVamanaIndexing },
   { "Groundtruth", test_Groundtruth },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};
