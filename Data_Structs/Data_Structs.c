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







//Implementation of the Set struct and functions



// struct set{
// 	set_Node root;				
// 	int size;
// };


// struct set_node{
//     set_Node left,right;
//     int value;
// };


set_Node S_node_create(int value) {
	set_Node node = malloc(sizeof(*node)) ;
	node->left = NULL;
    node->right = NULL;
	node->value = value;
	return node;
}


set_Node S_node_insert(set_Node node, int value, int * old_value) {
	//If the tree is empty we create a node that is the root
	if (node == NULL) {
		return S_node_create(value);
	}

	//The placement of the value is declared by its value 
	if (compare(value, node->value) == 0) {
		int old_value = node->value;
		node->value = value;

	} else if (compare(value, node->value) < 0) {
		//If the value is bigger we go to the left
		node->left = S_node_insert(node->left, value, old_value);

	} else {
		//If the value is smaller we got to the right
		node->right = S_node_insert(node->right, value, old_value);
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
	set->root = S_node_insert(set->root,value,&old_value);
	set->size++;
}

void set_remove(Set set,int value){
	int old_value;
	set->root = S_remove(set->root,value,&old_value);
	set->size--;
}

void set_destroy(Set set,int dvalue ){
	S_destroy(set->root, dvalue);
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

set_Node S_remove(set_Node node, int value, int * old_value) {
    //A quick check to see if the subtree we have is empty or not
	if (node == NULL) {
		printf("The subtree is empty returning");
		return NULL;
	}

	if (compare(value, node->value) == 0) {
        //In this case , we have found the value we have been searching for so now we have to remove the node
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
		node->left  = S_remove(node->left, value, old_value);
	else
		node->right = S_remove(node->right, value, old_value);

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

set_Node S_find_equal(set_Node node, int value) {
	if (node == NULL) return NULL;
	if (compare(value, node->value) == 0)
		return node;
	else if (compare(value, node->value) < 0)
		return S_find_equal(node->left, value);
	else 
		return S_find_equal(node->right, value);
}


//A main to test the functoins and make sure that they work correctly 

int main(){
	Set set = set_Create();
	set_insert(set,5);
	set_insert(set,123);
	set_insert(set,16);
	set_insert(set,316);
	set_insert(set,216);
	set_insert(set,4);
	set_insert(set,21);
	set_insert(set,1);
	set_insert(set,13);
	set_insert(set,12);
	set_insert(set,0);
	set_Node node = set->root;
	// while(node != node_find_max(set->root)){
	// 	if(node != NULL)
	// 		printf("%d", node->value);
	// 	node = set_next(set, node);
	// }

	// for (set_Node node = set->root; node != find_max(set->root); node = set_next(set, node)) {
	// 	printf("%d ", node->value);
	// }

	for (set_Node node = find_min(set->root); node != SET_EOF; node = set_next(set, node)) {
		printf("%d ", node->value);
	}


	printf("\n");


	set_remove(set, 12);

	for (set_Node node = find_min(set->root); node != SET_EOF; node = set_next(set, node)) {
		printf("%d ", node->value);
	}

	int x = 4;
	set_Node n = S_find_equal(set->root, x);
	printf(" %d \n ", n->value);
}