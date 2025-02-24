#include "Data_Structs.h"

//Implementaion of the List struct and functions

struct list{
	list_Node head;
	list_Node tail;
	pointer size;
};

struct List_Node{
	list_Node next_node;
	pointer data;
};

List create_list(){
	List l = malloc(sizeof(*l));
	if(l == NULL){
		return NULL;
	}

	l->size = 0;
	l->head = malloc(sizeof(*l->head));
	l->head->next_node = NULL;
	l->head->data = 0;
	l->tail = NULL;

    return l;
}

void l_add(List l, list_Node node, pointer value){
    if(node == NULL){
		node = l->head;
	}

	list_Node temp = malloc(sizeof(* temp));
    temp->data = value;
    
	temp->next_node = node->next_node;
	node->next_node = temp;

	if(l->tail == node){
		l->tail = temp;
	}
		
	l->size++;
}

void l_remove_next(List l ,list_Node node , pointer value){
	if(node == NULL)
		node = l->head;
	

	list_Node temp = node->next_node;
	node->next_node = temp->next_node;

	free(temp);
	l->size --;
	if(l->tail == temp)
		l->tail = node;
}

void list_destroy(List l){
	list_Node temp = l->head;
	while (temp != NULL){
		list_Node next = temp->next_node;
		free(temp);
		temp = next;
	}
	free(l);
}

// SET

set_Node S_node_create(int value) {
	set_Node node = malloc(sizeof(*node));
	node->left = NULL;
    node->right = NULL;
	node->value = value;
	return node;
}


set_Node S_node_insert(set_Node node,int * flag ,int value, int * old_value) {
	//If the tree is empty we create a node that is the root
	if (node == NULL) {
		* flag = 1;
		return S_node_create(value);
	}
	
	//The placement of the value is declared by its value 
	if (compare(value, node->value) == 0) {
		int old_value = node->value;
		node->value = value;
		return node;

	} else if (compare(value, node->value) < 0) {
		//If the value is bigger we go to the left
		node->left = S_node_insert(node->left,flag, value, old_value);

	} else {
		//If the value is smaller we got to the right
		node->right = S_node_insert(node->right,flag, value, old_value);
	}
	return node;
}

//A simple function that creates our set and save some space in our memory
Set set_Create(){
	Set set = malloc(sizeof(* set));
	set->root = NULL;
	set->size = 0;

	return set;
}

set_Node find_min(set_Node node){
	if(node != NULL && node->left != NULL)
		return find_min(node->left);
	else			
		return node;		
}

set_Node find_max(set_Node node) {
	if(node != NULL && node->right != NULL)
		return find_max(node->right);
	else
	 	return node;		
}

set_Node find_next(set_Node node , set_Node target){
	if (node == target){
		return find_min(node->right);
	}
	else if(compare(target->value , node->value) > 0){
		return find_next(node->right, target);
	}
	else if (compare(target->value , node->value) < 0){
		set_Node temp = find_next(node->left ,target);
		return temp != NULL ? temp : node;
	}
}

set_Node set_next(Set set, set_Node node){
	return find_next(set->root, node);
}

void set_insert(Set set, int value){
	int old_value;
	int flag = 0;
	set->root = S_node_insert(set->root,&flag,value,&old_value);
	if(flag == 1)
		set->size++;
}

void set_remove(Set set,int value){
	int old_value;
	int flag = 0;
	set->root = S_remove(set->root,value, &flag, &old_value);
	if(flag == 1)
		set->size--;
}

void set_destroy(Set set){
	S_destroy(set->root);
	free(set);
}

set_Node min_remove(set_Node node,set_Node * min){
    if(node->left == NULL){
        //In this case we have found the min and it is the node its self
        *min = node;
        return node->right; //So the new node is the right child
    }else{
        //Else, we have to left subtree and we have to continue searching in it for the min
        node->left=min_remove(node->left,min);
        return node;
    }
}


int compare(int value , int value1){
    return value - value1;
}

set_Node S_remove(set_Node node, int value, int * flag, int * old_value) {
    //A quick check to see if the subtree we have is empty or not
	if (node == NULL) {
		//printf("The subtree is empty returning");
		return NULL;
	}

	if (compare(value, node->value) == 0) {
        //In this case , we have found the value we have been searching for so now we have to remove the node
		*flag = 1;
		old_value = &(node->value);

		if (node->left == NULL) {
			//If there is no left child we can delete the node and keep the right child 
			set_Node right = node->right;
			free(node);
			return right;

		} else if (node->right == NULL) {
			//Similarly if we only have the left child we can erase our current node and the left child gets its place
			set_Node left = node->left;	
			free(node);
			return left;

		} else {
			//If our subtree has 2 children we have to find the minimum of its right child to replace its place
			set_Node temp;
			node->right = min_remove(node->right, &temp);

			// Σύνδεση του min_right στη θέση του node
			temp->left = node->left;
			temp->right = node->right;

			free(node);
			return temp;
		}
	}
	if (compare(value,node->value) < 0)
		node->left  = S_remove(node->left, value, flag, old_value);
	else
		node->right = S_remove(node->right, value, flag, old_value);

	return node;
}


void S_destroy(set_Node node){
	if(node == NULL){
		return;
	}
	S_destroy(node->left);
	S_destroy(node->right);

	free(node);
}

set_Node S_find_equal(set_Node node, int value) {
	if (node == NULL) return NULL;
	if (compare(value, node->value) == 0)
		return node;
	else if (compare(value, node->value) < 0)
		return S_find_equal(node->left, value);
	else 
		return S_find_equal(node->right, value);
}

// VECTOR
Vector vec_Create(int size ) {
	Vector vec = malloc(sizeof(*vec));

	vec->size = size;
	vec->capacity = size < 5 ? 5 : size;
	vec->array = calloc(vec->capacity, sizeof(*vec->array));

	return vec;
}

float vec_get_dist(Vector vec, int pos) {
	return vec->array[pos].dist;
}

int vec_get_at(Vector vec, int pos) {
	return vec->array[pos].value;
}

void vec_set_at(Vector vec, int pos, int value, float dist) {
	vec->array[pos].value = value;
	vec->array[pos].dist = dist;
}


void vec_insert(Vector vec, int value, float dist) {
	if (vec_find_node(vec, value) != VECTOR_EOF) return;

	if (vec->capacity == vec->size) {
		vec->capacity *= 2;
		vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
	}

	vec->array[vec->size].value = value;
	vec->array[vec->size].dist = dist;
	vec->size++;
}


void vec_remove(Vector vec) {
    if(vec->size == 0)
        return;

	
    //vec_set_destroy_value(vec, vec->array[vec->size - 1].value);
	vec->size--;

	if (vec->capacity > vec->size * 4 && vec->capacity > 10) {
		vec->capacity /= 2;
		vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
	}
}


VecNode vec_find_node(Vector vec, int value) {
	for (int i = 0; i < vec->size; i++)
		if (compare(vec->array[i].value, value) == 0)
			return &vec->array[i];		

	return VECTOR_EOF;
}

int vec_find_pos(Vector vec, int value) {
	for (int i = 0; i < vec->size; i++)
		if (compare(vec->array[i].value, value) == 0)
			return i;		

	return -1;
}

void vec_destroy(Vector vec) {
	free(vec->array);
	free(vec);
}


VecNode vec_first(Vector vec) {
	if (vec->size == 0)
		return VECTOR_BOF;
	else	
		return &vec->array[0];
}

VecNode vec_last(Vector vec) {
	if (vec->size == 0)
		return VECTOR_EOF;
	else
		return &vec->array[vec->size-1];
}


VecNode vec_next(Vector vec, VecNode node) {
	if (node == &vec->array[vec->size-1])
		return VECTOR_EOF;
	else
		return node + 1;
}


VecNode vec_previous(Vector vec, VecNode node) {
	if (node == &vec->array[0])
		return VECTOR_EOF;
	else
		return node - 1;
}


// PRIORITY QUEUE

void bubble_up(PQueue pqueue, int node) {
	// if you reach the root stop
	if (node == 1)
		return;

	// find the node's parent
	int parent = node/ 2;

	// if the parent value is smaller, swap and continue recursively to the top
	if (compare_dist(v_node_dist(pqueue, parent), v_node_dist(pqueue, node)) < 0.0f) {
		//printf("Swapping with parent: %d\n", parent);
		node_swap(pqueue, parent, node);
		bubble_up(pqueue, parent);
	}
}

void bubble_down(PQueue pqueue, int node) {
	// find the node's kids 
	int left = 2*node;
	int right = left + 1;
	int size = pqueue->vector->size;
	// if it has none return
	if (left > size) return;

	// find the max of the two children
	int max = left;
	if (right <= size && compare_dist(v_node_dist(pqueue, left), v_node_dist(pqueue, right)) < 0.0f) max = right;		

	// if the node is smaller than the max child, swap and continue recursively to the bottom
	if (compare_dist(v_node_dist(pqueue, node), v_node_dist(pqueue, max)) < 0.0f) {
		node_swap(pqueue, node, max);
		bubble_down(pqueue, max);
	}
}

float compare_dist(float dist1, float dist2) {
	return dist1-dist2;
}

int v_node_value(PQueue pqueue, int node) {
	return vec_get_at(pqueue->vector, node - 1);
}

float v_node_dist(PQueue pqueue, int node) {
	return vec_get_dist(pqueue->vector, node - 1);
}

void node_swap(PQueue pqueue, int node1, int node2) {
	int value1 = v_node_value(pqueue, node1);
	float dist1 = v_node_dist(pqueue, node1);
	int value2 = v_node_value(pqueue, node2);
	float dist2 = v_node_dist(pqueue, node2);

	vec_set_at(pqueue->vector, node1 - 1, value2, dist2);
	vec_set_at(pqueue->vector, node2 - 1, value1, dist1);
}

void naive_heapify(PQueue pqueue, Vector values) {
	// insert the vector's values one by one to the queue
	int size = values->size;
	for (int i = 0; i < size; i++)
		pqueue_insert(pqueue, vec_get_at(values, i), vec_get_dist(values, i));
}

PQueue pqueue_create(Vector values) {
	PQueue pqueue = malloc(sizeof(*pqueue));

	pqueue->vector = vec_Create(0);
	if (values != NULL ) naive_heapify(pqueue, values);

	return pqueue;
}

void pqueue_insert(PQueue pqueue, int value, float dist) {
	// first we insert the value at the very end of the vector if it does not already exist
	if (vec_find_node(pqueue->vector, value) != VECTOR_EOF) return;
	vec_insert(pqueue->vector, value, dist);

	// then we call bubble up to ensure the heap property
	bubble_up(pqueue, pqueue->vector->size);
}

void pqueue_remove(PQueue pqueue) {
	int last = pqueue->vector->size;
	if (last == 0) return;

	// swap the first and last node
	node_swap(pqueue, 1, last);
	// and remove the last
	vec_remove(pqueue->vector);

	// call bubble down to ensure the heap property
	bubble_down(pqueue, 1);
}

void pqueue_destroy(PQueue pqueue) {
	vec_destroy(pqueue->vector);
	free(pqueue);
}

// HASH MAP

int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

unsigned int hash_function(int filter, int min, int table_size) {
    return (filter-min) % table_size;
}

Map map_create(float min, float max) {
	Map map = malloc(sizeof(*map));
	int cap = 0;
	for (int i=0; i<25; i++) {
		if (prime_sizes[i] >= max-min+1) {
			cap = prime_sizes[i];
			break;
		}
	}
	map->capacity = cap;
	map->array = malloc(map->capacity * sizeof(struct map_node));

	// mark all nodes as empty
	for (int i = 0; i < map->capacity; i++) {
		map->array[i].state = EMPTY;
		map->array[i].values = NULL;
		map->array[i].key = 0;
	}

	map->size = 0;
	map->deleted = 0;

	map->max_f = max;
	map->min_f = min;

	return map;
}

static void rehash(Map map) {
	// store old data
	int old_capacity = map->capacity;
	MapNode old_array = map->array;

	// determine new capacity 
	int prime_no = sizeof(prime_sizes) / sizeof(int);
	for (int i = 0; i < prime_no; i++) {					
		if (prime_sizes[i] > old_capacity) {
			map->capacity = prime_sizes[i]; 
			break;
		}
	}

	// if there are no more firsts, double it
	if (map->capacity == old_capacity)					
		map->capacity *= 2;								

	// create a bigger hash table
	map->array = malloc(map->capacity * sizeof(struct map_node));
	for (int i = 0; i < map->capacity; i++)
		map->array[i].state = EMPTY;

	// only store the occupied entries
	map->size = 0;
	for (int i = 0; i < old_capacity; i++)
		if (old_array[i].state == OCCUPIED) {
			for (VecNode node = vec_first(old_array[i].values); node != VECTOR_EOF; node = vec_next(old_array[i].values, node))
				map_insert(map, old_array[i].key, node->value);
			vec_destroy(old_array[i].values);
		}
			
	free(old_array);
}

void map_insert(Map map, int key, int value) {
	MapNode node = NULL;

	// compute the position where the key "hashes" to
	int pos = hash_function(key, map->min_f, map->capacity);
	node = &map->array[pos];

	if (node->state == DELETED || node->state == EMPTY) {
		if (node->state == DELETED) map->deleted--;
		node->values = vec_Create(0);
		map->size++;
	}
	
	vec_insert(node->values, value, 0.0);
	node->state = OCCUPIED;
	node->key = key;

	float load_factor = (float)(map->size + map->deleted) / map->capacity;
	if (load_factor > MAX_LOAD_FACTOR)
		rehash(map);
}

MapNode map_find_node(Map map, int key) {
	// compute the position where the key "hashes" to
	int pos = hash_function(key, map->min_f, map->capacity);

	// if the position where the key hashes is occupied and the corresponding key matches, return the map node
	if (map->array[pos].state == OCCUPIED && compare(map->array[pos].key, key) == 0) {
		MapNode node = &map->array[pos];
		return node;
	}
	return MAP_EOF;
}

void map_remove(Map map, int key) {
	// find in which node the key is held
	MapNode node = map_find_node(map, key);
	if (node == MAP_EOF)
		return;

	// destroy the corresponding values set
	vec_destroy(node->values);

	// set the node to deleted
	node->state = DELETED;
	map->deleted++;
	map->size--;

	return;
}

Vector map_find_values(Map map, int key) {
	MapNode node = map_find_node(map, key);
	if (node != MAP_EOF)
		return node->values;
	else
		return NULL;
}

void map_destroy(Map map) {
	for (int i = 0; i < map->capacity; i++) {
		if (map->array[i].state == OCCUPIED) {
			vec_destroy(map->array[i].values);
		}
	}

	free(map->array);
	free(map);
}

MapNode map_first(Map map) {
	// start from the beginning of the array until you find the first occupied node
	for (int i = 0; i < map->capacity; i++)
		if (map->array[i].state == OCCUPIED)
			return &map->array[i];

	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
	for (int i = node - map->array + 1; i < map->capacity; i++)
		if (map->array[i].state == OCCUPIED)
			return &map->array[i];

	return MAP_EOF;
}
