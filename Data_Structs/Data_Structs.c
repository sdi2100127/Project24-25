#include "Data_Structs.h"

//Implementaion of the List struct and functions

struct list{
	list_node head;
	list_node tail;
	int size;
};

struct List_Node{
	list_node next_node;
	int data;
};

List create_list(){
	List l = malloc(sizeof(List));
	l->size = 0;
	


    list_node new;
    new = malloc(sizeof(list_node));
    new->next_node = NULL;
	l->head = new;
    return l;
}

void add(List l, int value){
    list_node temp = Create();
    temp->data = value;
    if(l->head == NULL){
        l->head = temp;
    }else{
        List temp_head = l->head;
        while(temp_head != NULL){
            temp_head = temp_head->head->next_node;
        }
        temp_head = temp;
    }
	l->size++;
}

void remove(List l ,list_node node , int value){
	if(node == NULL)
		return;
	list_node temp = node;
	if(node->next_node != NULL)
		node->next_node = temp->next_node->next_node;
	free(temp);
	l->size --;
	if(l->tail == NULL){
		l->tail = node->next_node;
	}
}

void list_destroy(List l){
	list_node temp = l->head;
	while (temp != NULL){
		list_node next = temp->next_node;
		free(temp);
		temp = next;
	}
	free(l);
}


//Implementation of the Set struct and functions
struct set{
	set_Node root;				
	int size;
};

struct set_node{
    set_Node left,right;
    int value;
};

set_Node node_create(int value) {
	set_Node node = malloc(sizeof(*node)) ;
	node->left = NULL;
    node->right = NULL;
	node->value = value;
	return node;
}


set_Node node_insert(set_Node node, int value, int old_value) {
	//If the tree is empty we create a node that is the root
	if (node == NULL) {
		return node_create(value);
	}

	//The placement of the value is declared by its value 
	if (compare(value, node->value) == 0) {
		int *old_value = node->value;
		node->value = value;

	} else if (compare(value, node->value) < 0) {
		//If the value is bigger we go to the left
		node->left = node_insert(node->left, value, old_value);

	} else {
		//If the value is smaller we got to the right
		node->right = node_insert(node->right, value, old_value);
	}

	return node;
}

//A simple function that creates our set and save some space in our memory
Set set_Create(){
	Set set = malloc(sizeof(Set));
	set->root = NULL;
	set->size = 0;

	return set;
}


void set_insert(Set set, int value){
	int *old_value;
	set->root = node_insert(set->root,value,&old_value);
	set->size++;
}

void set_remove(Set set,int value){
	int *old_value;
	set->root = S_node_remove(set->root,value,&old_value);
	set->size--;
}

void set_destroy(Set set,int dvalue ){
	S_node_destroy(set->root, dvalue);
	free(set);
}

set_Node min_remove(set_Node node,set_Node *min){
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


int compare(int value , int cur_value){
    if(value > cur_value)
        return -1;
    else if (value < cur_value)
        return 1;
    else 
        return 0;
}

set_Node S_remove(set_Node node, int value, int old_value) {
    //A quick check to see if the subtree we have is empty or not
	if (node == NULL) {
		printf("The subtree is empty returning");
		return NULL;
	}

	if (compare(value, node->value) == 0) {
        //In this case , we have found the value we have been searching for so now we have to remove the node
		old_value = node->value;

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
			node->right = min_remove(node->right, temp);

			// Σύνδεση του min_right στη θέση του node
			temp->left = node->left;
			temp->right = node->right;

			free(node);
			return temp;
		}
	}
	if (compare(value,node->value) < 0)
		node->left  = node_remove(node->left, value, old_value);
	else
		node->right = node_remove(node->right, value, old_value);

	return node;
}


void S_destroy(set_Node node,int dvalue){
	if(node == NULL){
		return;
	}
	S_destroy(node->left, dvalue);
	S_destroy(node->right, dvalue);

	free(node);
}





//A main to test the functoins and make sure that they work correctly 

int main(){
	List l = Create();
	add(l,2);
	add(l,2);
	add(l,2);
	add(l,2);
	add(l,2);
	add(l,2);
	add(l,2);
	add(l,2);
	add(l,2);

}