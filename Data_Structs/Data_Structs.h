#include <stdio.h>
#include <stdlib.h>

//Decleration of a pointer to store our abstract values in our data structures
typedef void * pointer;

//Decleration of struct List and its funtions

//Decleration of a pointer to the List
typedef struct list * List;

//Decleration of a pointer to the List nodes
typedef struct List_Node * list_Node;

//Creation of the list
List  create_list();

//Addition of a list node to the list
void l_add(List, list_Node, pointer);

//Removal of a list node from the list
void l_remove_next(List, list_Node, pointer);



/* KANE TA IDIA KAI STO SETTTTTTT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

//Finding a node by its value
list_Node find_node (List, pointer);

//Returns the value of value
pointer node_value(list_Node);

//Returns the next node from the given 
list_Node list_next(list_Node);

// WS EDW..................

void destroy(List);


//Decleration of struct Set and its funtions

#define SET_BOF (set_Node)0
#define SET_EOF (set_Node)0

//Decleration of a pointer to the set nodes
typedef struct set_node * set_Node;

struct set_node{
    set_Node left,right;
    int value;
};

struct set{
	set_Node root;				
	int size;
};

//Decleration of a pointer to the Set
typedef struct set * Set;     


//Creation of the set
Set set_Create();

//Finds the next set node
set_Node set_next(Set , set_Node);

set_Node find_next(set_Node , set_Node);

//Insertion to the set
void set_insert(Set, int);

//Removal from the set
void set_remove(Set ,int );

//Destroy of the set
void set_destroy(Set);

//Creation of a set node
set_Node S_node_create(int);      

//Insertion of a set node
set_Node S_node_insert(set_Node,int * , int , int *);      

//Removal of the min node of the tree
set_Node min_remove(set_Node ,set_Node * );

//Removal of a node from the set
set_Node S_remove(set_Node , int, int* , int* );

//Total deletion of a set node
void S_destroy(set_Node);

//Other functions that will be very helpfull

set_Node find_min(set_Node);

set_Node find_max(set_Node);

set_Node S_find_equal(set_Node, int);


//A function that compares 2 numbers that returns -1 if the second one is bigger 1 if the first is bigger and 0 if the numbers are equal
int compare (int , int );

// VECTOR
typedef struct vector* Vector; 
typedef struct vector_node* VecNode;

#define VECTOR_BOF (VecNode)0
#define VECTOR_EOF (VecNode)0

struct vector_node {
	int value;
	float dist;
};

struct vector {
	VecNode array;			
	int size;					
	int capacity;
};

//Creation of a vector. Needs an int for the size and returns a pointer to a vector sturct.
Vector vec_Create(int );

//Inserts a new value to the end of the vector and the size expands by 1
void vec_insert(Vector , int, float);

//Removes the last value in the vector
void vec_remove(Vector );

//Returns the dist attribute of the node in position pos
float vec_get_dist(Vector vec, int pos);

//Takes you to the value of the posiotion you given it
int vec_get_at(Vector , int );

//Changes the position of a value 
void vec_set_at(Vector vec, int pos, int value, float dist);

//Finds the node of a value
VecNode vec_find_node(Vector , int );

//Finds the position of a value
int vec_find_pos(Vector , int );

// free vector's vec memory
void vec_destroy(Vector);



VecNode vec_first(Vector vec);
VecNode vec_last(Vector vec);
VecNode vec_next(Vector vec, VecNode node);
VecNode vec_previous(Vector vec, VecNode node);

// PRIORITY QUEUE 

struct priority_queue
{
	Vector vector;
};

typedef struct priority_queue* PQueue;

float compare_dist(float dist1, float dist2);

int v_node_value(PQueue pqueue, int node);

float v_node_dist(PQueue pqueue, int node);

void node_swap(PQueue pqueue, int node1, int node2);

void bubble_up(PQueue pqueue, int node);

void bubble_down(PQueue pqueue, int node);

void naive_heapify(PQueue pqueue, Vector values);

PQueue pqueue_create(Vector values);

void pqueue_insert(PQueue pqueue, int value, float dist);

void pqueue_remove(PQueue pqueue);

void pqueue_destroy(PQueue pqueue);

void heapify(PQueue pq, int node);

void build_heap(PQueue pq);

// HASH MAP

#define MAP_EOF (MapNode)0

extern int prime_sizes[];

typedef enum {
	EMPTY, OCCUPIED, DELETED
} State;

#define MAX_LOAD_FACTOR 0.7

struct map_node{
	int key;		// hash key
	//Set values;  	// set of values corresponding to said key
	Vector values;
	State state;	// state of each node
};

typedef struct map_node* MapNode;

struct map {
	MapNode array;
	int capacity;
	int size;
	int deleted;
	float max_f;
	float min_f;
};

typedef struct map* Map;

Map map_create(float min, float max);

static void rehash(Map map);

void map_insert(Map map, int key, int value);

MapNode map_find_node(Map map, int key);

void map_remove(Map map, int key);

Vector map_find_values(Map map, int key);

void map_destroy(Map map);

MapNode map_first(Map map);

MapNode map_next(Map map, MapNode node);