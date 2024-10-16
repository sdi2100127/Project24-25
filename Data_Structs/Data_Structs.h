#include <stdio.h>
#include <stdlib.h>


typedef struct List
{
    int data;
    struct List * nextNode;
}list;      // typo??

typedef struct List  * Node;

Node create_list();

Node add_to_list(Node head, int value);


// to implement the adjecency list we will be using a hashMap because of
// the advantages it presents such as: being able to store large amounts of data
// and being very fast and effective while performing simple operations

typedef struct map* Map;
