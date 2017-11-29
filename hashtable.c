#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bucket.h"

typedef struct HashTable{
	Bucket** HTbucket;
	int size;
}HashTable;
	

HashTable* createTable(int Hsize, int Bsize){ 
	int i,j;
	HashTable* HTable = malloc(sizeof(HashTable));
	HTable->size=Hsize;
	HTable->HTbucket = malloc(sizeof(Bucket)*Hsize);
	for(i=0;i<Hsize;i++){
		HTable->HTbucket[i]=NULL;
	}
	printf("CREATE:hash table created.\n");
	return HTable;
}

void insertTable(HashTable* HTable, cdr *record, char *key, int datasize, int Bsize){
	int i, j, k, pos;
	k=0;
	for(i=0;i<14;i++){
		if(isdigit(key[i]))
			k=k+key[i];
	}
	pos=k%HTable->size;

	if(HTable->HTbucket[pos]==NULL){			//bucket has not been created yet so we make it now
		Bucket* bucket = malloc(sizeof(Bucket));
		bucket->next=NULL;
		bucket->size=Bsize;
		bucket->bentries = malloc(sizeof(Bucket_Entries)*Bsize);
		for(j=0;j<Bsize;j++){
			bucket->bentries[j] = malloc(sizeof(Bucket_Entries));
			strcpy(bucket->bentries[j]->idkey,"00000000000000");

			information* inf = malloc(sizeof(information));
			inf->size = datasize;
			inf->data = malloc(sizeof(cdr)*datasize);
			for(k=0;k<datasize;k++)
				inf->data[k]=NULL;
			inf->next = NULL;

			bucket->bentries[j]->info= inf;
		}
		HTable->HTbucket[pos] = bucket;
	}//bucket created so we now insert the record to the datastructure.
	i=insertBucket(HTable->HTbucket[pos], key, record, datasize);
	if(i)
		printf("Insert: OK\n");
	else
		printf("Insert: error\n");
}

void printTable(HashTable* HTable){
	int i;
	if(HTable==NULL) return;
	printf("**PRINT HASHTABLE** %d\n",HTable->size);
	for(i=0;i<HTable->size;i++){
		if(HTable->HTbucket[i]==NULL)
			printf("HT %d->NULL\n", i);
		else{
			printf("HT %d ->\n",i);
			printBucket(HTable->HTbucket[i]);
		}
	}	
}

void destroyTable(HashTable* HTable, int h){
	int i;
	if(HTable==NULL) return;
	for(i=0;i<HTable->size;i++)
		if(HTable->HTbucket[i]!=NULL)
			destroyBucket(HTable->HTbucket[i], h);
	free(HTable->HTbucket);
	free(HTable);
	//printf("destroyTable: Table Destroyed\n");
}

void deleteTable(HashTable* HTable, char *cdr_id, char *caller, int h){
	int i,pos,k=0;
	for(i=0;i<14;i++){
		if(isdigit(caller[i]))
			k=k+caller[i];
	}
	pos=k%HTable->size;
	if(HTable->HTbucket[pos]!=NULL)
		HTable->HTbucket[pos]=deleteBucket(HTable->HTbucket[pos], cdr_id, caller, h);
	else 
		printf("HDelete: error\n");
}

void findHash(HashTable* HTable, char *caller, char *t1, char *y1, char *t2, char *y2){
	int i,pos,k=0;
	for(i=0;i<14;i++){
		if(isdigit(caller[i]))
			k=k+caller[i];
	}
	pos=k%HTable->size;
		if(HTable->HTbucket[pos]!=NULL)
		findBucket(HTable->HTbucket[pos], caller, t1, y1, t2, y2);
	else 
		printf("Hfound: error\n");
}


void lookupHash(HashTable* HTable, char *caller, char *t1, char *y1, char *t2, char *y2){
	int i,pos,k=0;
	for(i=0;i<14;i++){
		if(isdigit(caller[i]))
			k=k+caller[i];
	}
	pos=k%HTable->size;
	if(HTable->HTbucket[pos]!=NULL)
		lookupBucket(HTable->HTbucket[pos], caller, t1, y1, t2, y2);
	else 
		printf("Lfound: error\n");
}


int commHash(HashTable* Hash1, HashTable* Hash2, char* caller1, char* caller2){
	int i,pos1, pos2, k=0;
	for(i=0;i<14;i++){
		if(isdigit(caller1[i]))
			k=k+caller1[i];
	}
	pos1=k%Hash1->size;
	for(i=0;i<14;i++){
		if(isdigit(caller1[i]))
			k=k+caller1[i];
	}
	pos2=k%Hash2->size;
	return commBucket(Hash1->HTbucket[pos1],Hash2->HTbucket[pos2], caller1, caller2);
}

void indistHash(HashTable* Hash1, HashTable* Hash2, char* caller1, char* caller2){

	int i,j, pos1, pos2, k=0, ccsize=0, found;
	for(i=0;i<14;i++){
		if(isdigit(caller1[i]))
			k=k+caller1[i];
	}
	pos1=k%Hash1->size;
	pos2=k%Hash2->size;
	communicateInfo *Cinfo1=NULL, *Cinfo2=NULL, *CC=NULL;		//a struct that has everyone caller have communicate
	if(Hash1->HTbucket[pos1]!=NULL && Hash2->HTbucket[pos2]!=NULL)
		Cinfo1 = indistBucket(Hash1->HTbucket[pos1],Hash2->HTbucket[pos2], caller1);
	else
		printf("IndistH: Error\n");
	k=0;
	for(i=0;i<14;i++){
		if(isdigit(caller2[i]))
			k=k+caller2[i];
	}
	pos1=k%Hash1->size;
	pos2=k%Hash2->size;
	if(Hash1->HTbucket[pos1]!=NULL && Hash2->HTbucket[pos2]!=NULL)
		Cinfo2 = indistBucket(Hash1->HTbucket[pos1],Hash2->HTbucket[pos2], caller2);
	else
		printf("IndistH: Error\n");
	if(Cinfo1== NULL || Cinfo2==NULL) 
		return; 
	//we have to find the common numbers in the Cinfo1, Cinfo2
	printf("%s %d\n",Cinfo2->comm[2], Cinfo2->size );
	for(i=0;i<Cinfo1->size;i++){
		for(j=0;j<Cinfo2->size;j++){
			if(Cinfo1->comm[i]==NULL || Cinfo2->comm[j]==NULL) continue;
			printf("%s %s \n",Cinfo1->comm[i],Cinfo2->comm[j] );
			if(strcmp(Cinfo1->comm[i],Cinfo2->comm[j])==0){
				ccsize++;
				continue;
			}
		}
	}
	CC=malloc(sizeof(communicateInfo));
	CC->comm = malloc(sizeof(char*)*ccsize);
	CC->size=ccsize;
	ccsize=0;
	for(i=0;i<Cinfo1->size;i++){
		for(j=0;j<Cinfo2->size;j++){
			if(Cinfo1->comm[i]==NULL || Cinfo2->comm[j]==NULL) continue;
			if(strcmp(Cinfo1->comm[i],Cinfo2->comm[j])==0){
				CC->comm[ccsize]= (char*)malloc(sizeof(char)*strlen(Cinfo1->comm[i]));
				strcpy(CC->comm[ccsize],Cinfo1->comm[i]);
				ccsize++;
				continue;
			}
		}
	}

	for(i=0;i<CC->size;i++){
		for(j=0;j<CC->size;j++){
			if(i==j)
				continue;
			else{
				found = commHash(Hash1, Hash2, CC->comm[i], CC->comm[j]);
				if(found)	break;
			}
		}
		if(!found){			//he havent communicate with noone in the list so we print him
			printf("~>%s\n",CC->comm[i]);
		}
		found=0;
	}

	//free our memory
	destroyCOMM(CC);
	destroyCOMM(Cinfo2);
	destroyCOMM(Cinfo1);
}

void topdestHash(HashTable* HTable, char *caller){
	int i,pos,k=0;
	for(i=0;i<14;i++){
		if(isdigit(caller[i]))
			k=k+caller[i];
	}
	pos=k%HTable->size;
	topdestBucket(HTable->HTbucket[pos], caller);
}

cdr* returnCDR(HashTable* HTable, char *cdr_id, char *caller){
	int i,pos,k=0;
	for(i=0;i<14;i++){
		if(isdigit(caller[i]))
			k=k+caller[i];
	}
	pos=k%HTable->size;
	if(HTable->HTbucket[pos]!=NULL)
		return cdrBucket(HTable->HTbucket[pos], cdr_id, caller);
	else{ 
		printf("Hreturn: error\n");
		return NULL;
	}
}