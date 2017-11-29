#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "node.c"
#include "stack.c"



typedef struct Heap{
	Node* head;
	int size; 						// the size of the heap at a current moment
}Heap;

Heap* createHeap(){					//inits and returns our heap;
	Heap* heap = malloc(sizeof(Heap));
	heap->head = NULL;
	heap->size = 0;
	return heap;
}


void insert2End(Heap* heap, Node* our_Node){		//add the node in the end of the heap
	int currnet_node = heap->size;
	stack *Stack = createStack();
	while(currnet_node>1){
		if(currnet_node%2==1)
			pushStack(Stack,'r');
		else
			pushStack(Stack,'l');
		currnet_node=currnet_node/2;
	}
	Node* temp = heap->head;
	char act;
	printStack(Stack);
	while(!emptyStack(Stack)){
		act = popStack(Stack);
		if(sizeStack(Stack)!=0){					//if its the last move we add it there
			if(act=='r')
				temp=temp->right;
			else
				temp=temp->left;
		}
		else{
			if(act=='r')
				temp->right=our_Node;
			else
				temp->left=our_Node;
		}
	}
	destroyStack(Stack);
}

Node* searchNode(Node* our_Node, char* key){			//using anadromi
	if(our_Node==NULL)
		return NULL;
	if(strcmp(our_Node->key,key)==0){
		//printf("found!\n");
		return our_Node;
	}
	Node* found=NULL;
	if(our_Node->left!=NULL)
		found = searchNode(our_Node->left,key);
	if(found!=NULL)
		return found;
	if(our_Node->right!=NULL)
		found = searchNode(our_Node->right,key);
	if(found!=NULL)
		return found;
	return NULL;
}


void swapNodes(Node* N1, Node* N2){						//here we just change the information of those 2 nodes avoiding to make changes in the heap
	char* temp_key=(char*)malloc(sizeof(char)*strlen(N1->key));
	float temp_ammount = N1->ammount;
	strcpy(temp_key, N1->key);
	strcpy(N1->key,N2->key);
	N1->ammount=N2->ammount;
	N2->ammount=temp_ammount;
	strcpy(N2->key,temp_key);
	free(temp_key);
}


Node* findNode(Heap* heap, int currnet_node){		//using the stack-way to find the node with that number
	stack *Stack = createStack();
	while(currnet_node>1){
		if(currnet_node%2==1)
			pushStack(Stack,'r');
		else
			pushStack(Stack,'l');
		currnet_node=currnet_node/2;
	}
	Node* temp = heap->head;
	char act;
	while(!emptyStack(Stack)){
		act = popStack(Stack);
		//printf("%c!!!\n",act );
		if(act=='r')
			temp=temp->right;
		else
			temp=temp->left;
	}
	destroyStack(Stack);
	return temp;
}


void printHeap(Heap* heap){
	int i;
	printf("PRINT HEAP: %d\n", heap->size);
	for(i=1;i<=heap->size;i++){
		Node* temp = findNode(heap, i);
		if(temp!=NULL)
			printf("\tELEM: %d ->key: %s, AMMOUNT: %f\n",i,temp->key,temp->ammount);
		else
			printf("PrintHEAP:error %d not found\n",i );
	}
}


void HeapSort(Heap* heap){					//fixes all the heap
	int i;
	Node *our_Node, *father;
	for(i=heap->size;i>1;i--){				//takes every node and his fatther
		our_Node=findNode(heap,i);
		father = findNode(heap,i/2);
		if(our_Node->ammount>father->ammount)		//if father is smaller they are getting swapped
			swapNodes(our_Node, father);
	}

}

void InsertHeap(Heap* heap, cdr *record, char* config){				
	Node* found=searchNode(heap->head, record->originator_number);	//if the caller exists we update his ammount
	if(found != NULL){
		printf("InsertHeap: node exist so we update him\n");
		updateNode(found, record, config, '+');
	}
	else{	
		heap->size++;												//else we inserts him in the end
		Node* our_Node = createNode(record, config);
		if(heap->head == NULL)										//heap is empty 
			heap->head = our_Node;
		else
			insert2End(heap, our_Node);	
	}
	HeapSort(heap);
	printf("heap insertDONE\n");
	printHeap(heap);
}


void destroyHeap(Heap *heap){
	int i;
	Node* our_Node;
	for(i=heap->size;i>1;i--){
		our_Node=findNode(heap,i);
		destroyNode(our_Node);
	}
	if(heap->head!=NULL)
		destroyNode(heap->head);
	free(heap);
}

void deleteHeap(Heap *heap, cdr *record, char* config){
	Node* found=searchNode(heap->head, record->originator_number);
	if(found == NULL){
		printf("Node with key: %s does not exists.\n",record->originator_number);
	}
	updateNode(found, record, config, '-');
	if(found->ammount<0)
		deleteDATnodeHeap(heap, found);
	else
		HeapSort(heap);
}


void deleteDATnodeHeap(Heap *heap, Node* found){
	if(found==NULL){
		printf("DeleteHeap:Error\n");
		return;
	}
	else{		//swap with the last node
		Node* last_node=findNode(heap, heap->size);
		Node* father=findNode(heap, heap->size/2);
		swapNodes(last_node, found);
		if(father->left==last_node)
			father->left=NULL;
		else
			father->right=NULL;
		destroyNode(last_node);
		heap->size--;
		HeapSort(heap);
		printf("Caller deleted\n");
	}
	printHeap(heap);
}


