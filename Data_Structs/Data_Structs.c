#include "Data_Structs.h"

Node Create(){
    Node new;
    new = (Node)malloc(sizeof(struct List));
    new->nextNode = NULL;
    return new;
}


Node add(Node head, int value){
    Node temp = Create();
    temp->data = value;
    if(head == NULL){
        head = temp;
    }else{
        Node temp_head = head;
        while(temp_head != NULL){
            temp_head = temp_head->nextNode;
        }
        temp_head = temp;
    }
    return head;
}