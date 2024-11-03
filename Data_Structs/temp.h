typedef struct vector* Vector;
typedef struct vector_node* VecNode;

#define VECTOR_BOF (VecNode)0
#define VECTOR_EOF (VecNode)0

struct vector_node {
	int value;
};

struct vector {
	VecNode array;			
	int size;					
	int capacity;
};

//Creation of a vector. Needs an int for the size and returns a pointer to a vector sturct.
Vector vec_Create(int );

//Inserts a new value to the end of the vector and the size expands by 1
void vec_insert(Vector , int);

//
void vec_remove(Vector );


//Takes you to the value of the posiotion you given it
int vec_get_at(Vector , int );

//Changes the position of a value 
void vec_set_at(Vector vec, int pos, int value);

//Finds the position of a value
int vec_find(Vector , int );

// 
void vec_set_destroy_value(Vector );

void vec_destroy(Vector);



VecNode vector_first(Vector vec);
VecNode vector_last(Vector vec);
VecNode vector_next(Vector vec, VecNode node);
VecNode vector_previous(Vector vec, VecNode node);