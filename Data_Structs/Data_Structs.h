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




typedef struct HashTable
{
    int data;

}Hash;

