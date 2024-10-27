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
void set_destroy(Set,int);

//Creation of a set node
set_Node S_node_create(int);      

//Insertion of a set node
set_Node S_node_insert(set_Node,int * , int , int *);      

//Removal of the min node of the tree
set_Node min_remove(set_Node ,set_Node * );

//Removal of a node from the set
set_Node S_remove(set_Node , int, int* , int* );

//Total deletion of a set node
void S_destroy(set_Node , int );

//Other functions that will be very helpfull

set_Node find_min(set_Node);

set_Node find_max(set_Node);

set_Node S_find_equal(set_Node, int);


//A function that compares 2 numbers that returns -1 if the second one is bigger 1 if the first is bigger and 0 if the numbers are equal
int compare (int , int );       