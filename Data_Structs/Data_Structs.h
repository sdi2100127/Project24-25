#include <stdio.h>
#include <stdlib.h>


typedef struct List
{
    int data;
    struct List * nextNode;
}list;

typedef struct List  * Node;

Node Create();

Node add(Node head, int value);

void L_delete();

typedef struct set_node * set_Node;

set_Node node_create(int);


int compare (int , int );

set_Node node_insert(set_Node node, int value, int old_value);

set_Node min_remove(set_Node node,set_Node *);

set_Node S_remove(set_Node node, int value, int old_value);

void S_destroy(Set);