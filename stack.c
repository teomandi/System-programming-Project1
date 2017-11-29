#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct kombos{
	char act;
	struct kombos* next;
}kombos;

typedef struct stack{
	int size;
	kombos* start;	
}stack;

stack* createStack(){
	stack* stoiva=malloc(sizeof(stack));
	stoiva->size=0;
	stoiva->start=NULL;
	return stoiva;
}

void destroyStack(stack* stoiva){
	if(stoiva->start!=NULL){
		while(stoiva->start!=NULL){
			kombos* temp=stoiva->start->next;
			free(stoiva->start);
			stoiva->start= temp;
		}
	}
	free(stoiva);
}

void pushStack(stack* stoiva, char action){
	kombos* komb=malloc(sizeof(kombos));
	komb->act=action;
	if(stoiva->size==0){						//add to empty stack	
		komb->next=NULL;
		stoiva->start=komb;
	}
	else{
		kombos* temp=stoiva->start;
		stoiva->start=komb;
		komb->next=temp;
	}
	stoiva->size++;
}

char popStack(stack* stoiva){
	kombos* temp=stoiva->start->next;
	char a = stoiva->start->act;
	free(stoiva->start);
	stoiva->start=temp;
	stoiva->size--;
	return a;
}

int sizeStack(stack* stoiva){
	return stoiva->size;
}

int emptyStack(stack* stoiva){
	return stoiva->size==0;
}

void printStack(stack* stoiva){
	kombos* temp = stoiva->start;
	printf("prin STACK!\n");
	while(temp!=NULL){
		printf("\t%c\n",temp->act);
		temp=temp->next;
	}
}