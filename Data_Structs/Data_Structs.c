#include "Data_Structs.h"
//Testing
Node create_list(){
    Node new;
    new = (Node)malloc(sizeof(struct List));
    new->nextNode = NULL;
    return new;
}


Node add_to_list(Node head, int value){
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






//Set implementation

struct Set{
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


void S_destroy(set_Node node){

}