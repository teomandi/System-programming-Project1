#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Bucket_Entries{
	char idkey[14];
	information* info;
}Bucket_Entries;

typedef struct Bucket{
	Bucket_Entries** bentries;
	struct Bucket* next;
	int size;
}Bucket;

int insertBucket(Bucket* bucket , char *key, cdr *record, int datasize){
	int i,j, res;
	Bucket* temp = bucket;
	while(1){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, key)==0){
				res=insertInfo(temp->bentries[i]->info, record);
				/*check*/
				return res;
			}
		}
		if(temp->next!=NULL){
			temp=temp->next;
			continue;
		}
		else
			break; // in this case key does not exist in the bucket so we must added for first time
	}
	temp=bucket;
	while(1){
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, "00000000000000")==0){
				strcpy(temp->bentries[i]->idkey,key);
				res=insertInfo(temp->bentries[i]->info, record);
				/*check*/
				return res;
			}
		}
		if(temp->next!=NULL){
			temp=temp->next;
			continue;
		}
		else{	//no space so we create a new bucket and we put it in there;
			Bucket *b = malloc(sizeof(Bucket));
			b->next=NULL;
			b->size = bucket->size;
			b->bentries = malloc(sizeof(Bucket_Entries)*b->size);
			for(j=0;j<b->size;j++){
				b->bentries[j] = malloc(sizeof(Bucket_Entries));
				strcpy(b->bentries[j]->idkey,"00000000000000");

				information* inf = malloc(sizeof(information));
				inf->size = datasize;
				inf->data = malloc(sizeof(cdr)*datasize);
				int k;
				for(k=0;k<datasize;k++)
					inf->data[k]=NULL;
				inf->next = NULL;

				b->bentries[j]->info= inf;
			}
			temp->next=b;
			strcpy(temp->bentries[0]->idkey,key);
			res=insertInfo(b->bentries[0]->info, record);
			return res;
		}
	}
	return 0;
}

void printBucket(Bucket *bucket){
	int i;
	do{
		for(i=0;i<bucket->size;i++){
			printf("\t%s {\n",bucket->bentries[i]->idkey);
			if(bucket->bentries[i]->info!=NULL)
				printInfo(bucket->bentries[i]->info);
			else
				printf("\t\t->!NULL");
			printf("\t}\n");
		}
		bucket=bucket->next;
		if(bucket!=NULL)
			printf("\t|-------------------------|\n");
	}while(bucket!=NULL);
	
}

void destroyBucket(Bucket *bucket, int h){
	int i;
	do{
		for(i=0;i<bucket->size;i++){
			if(bucket->bentries[i]->info!=NULL)
				destroyInfo(bucket->bentries[i]->info, h);
			free(bucket->bentries[i]);
		}
		Bucket *nextb = bucket->next;
		free(bucket->bentries);
		free(bucket);
		bucket=nextb;

	}while(bucket!=NULL);
	//printf("destroyBucket: bucket Destroyed\n");
}

Bucket* deleteBucket(Bucket* bucket, char *cdr_id, char *caller, int h){
	int i, found=0, change=0;
	Bucket* temp = bucket;
	information* res;
	while(!found){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller)==0){
				res=deleteInfo(temp->bentries[i]->info, cdr_id, caller, h);
				if(res!=temp->bentries[i]->info){					//that means that we took something differnet than that we had
					if(res!=NULL)				//new data
						temp->bentries[i]->info=res;
					else{						//means that all the cdr was deleted so we have to mark it as a free entrie
						strcpy(temp->bentries[i]->idkey,"00000000000000");
						temp->bentries[i]->info=NULL;
					}
					change=1;
				}
				found=1;
			}
		}
		if(temp->next!=NULL && !found)  
			temp=temp->next;
		if(temp->next==NULL) break;
	}
	if(!found){							//not found
		printf("DeleteB:error caller: %s not found.\n",caller);
		return bucket;
	}
	if(!change)
		return bucket;
	else{						//we have to check if the bucket is empty
		int empty=0;
		for(i=0;i<temp->size;i++){
			if(strcmp(temp->bentries[i]->idkey,"00000000000000")!=0)
				empty++;
		}
		if(empty==0){
			Bucket *temp2;
			if(temp == bucket){				//bucket is empty so we have changes here
				temp2 = temp->next;
				for(i=0;i<temp->size;i++)
					free(temp->bentries[i]);
				free(temp->bentries);
				free(temp);
				return temp2;
			}
			else{
				temp2=bucket;
				while(temp2->next != temp)
					temp2=temp2->next;
				Bucket *temp3;
				temp3=temp->next;
				for(i=0;i<temp->size;i++)
					free(temp->bentries[i]);
				free(temp->bentries);
				free(temp);
				temp2->next =temp3;		//fix the row of buckets and we return the usual
				return bucket;
			}

		}
		else	//nothing to care return the usual
			return bucket;

	}
}


void findBucket(Bucket* bucket, char *caller, char *t1, char *y1, char *t2, char *y2){
	int i, found=0;
	Bucket* temp = bucket;
	while(!found){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller)==0){
				findInfo(temp->bentries[i]->info, caller, t1, y1, t2, y2);
				found=1;
			}
		}
		if(temp->next!=NULL && !found)  
			temp=temp->next;
		else
			break;
	}
	if(!found)							//not found
		printf("FindB:error caller: %s not found.\n",caller);
}

void lookupBucket(Bucket* bucket, char *caller, char *t1, char *y1, char *t2, char *y2){
	int i, found=0;
	Bucket* temp = bucket;
	while(!found){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller)==0){
				lookupInfo(temp->bentries[i]->info, caller, t1, y1, t2, y2);
				found=1;
			}
		}
		if(temp->next!=NULL && !found)
			temp=temp->next;
		else
			break;
	}
	if(!found)							//not found
		printf("LookupB:error caller: %s not found.\n",caller);
}



void topdestBucket(Bucket* bucket, char *caller){
	int i, found=0;
	Bucket* temp = bucket;
	while(!found){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller)==0){
				topdestInfo(temp->bentries[i]->info, caller);
				return;
			}
		}
		if(temp->next!=NULL && !found)
			temp=temp->next;
		if(temp->next==NULL) break;
	}
	if(!found)							//not found
		printf("topdestB:error caller: %s not found.\n",caller);
}

cdr* cdrBucket(Bucket* bucket, char *cdr_id, char *caller){
	int i, found=0;
	Bucket* temp = bucket;
	while(!found){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller)==0)
				return cdrInfo(temp->bentries[i]->info, cdr_id, caller);
		}
		if(temp->next!=NULL && !found)  
			temp=temp->next;
		if(temp->next==NULL) break;
	}
	if(!found){							//not found
		printf("ReturnB:error caller: %s not found.\n",caller);
		return NULL;
	}
}


communicateInfo* indistBucket(Bucket* bucket1, Bucket* bucket2, char* caller){	//returns a struct that has all the numbers caller have communicate with
	int i, found1=0, found2=0;
	Bucket* temp = bucket1;
	information *inf1, *inf2;
	while(!found1){																	
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller)==0){
				inf1=temp->bentries[i]->info;
				found1=1;
			}
		}
		if(temp->next!=NULL && !found1)
			temp=temp->next;
		else
			break;
	}
	temp=bucket2;
	while(!found2){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller)==0){
				inf2=temp->bentries[i]->info;
				found2=1;
			}
		}
		if(temp->next!=NULL && !found2)
			temp=temp->next;
		else
			break;
	}
	if(found1 && found2)
		return indistInfo(inf1,inf2);
	else{
		printf("IndistB: Error\n");
		return NULL;
	}
}


int commBucket(Bucket* bucket1, Bucket* bucket2, char* caller1, char* caller2){
	int i, found1=0, found2=0;
	Bucket* temp = bucket1;
	information *inf1, *inf2;
	while(!found1){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller1)==0){
				inf1=temp->bentries[i]->info;
				found1=1;
			}
		}
		if(temp->next!=NULL && !found1)
			temp=temp->next;
		else
			break;
	}
	temp=bucket2;
	while(!found2){																	//search in the bucket
		for(i=0;i<temp->size;i++){ 
			if(strcmp(temp->bentries[i]->idkey, caller1)==0){
				inf2=temp->bentries[i]->info;
				found2=1;
			}
		}
		if(temp->next!=NULL && !found2)
			temp=temp->next;
		else
			break;
	}
	if(found1 && found2)
		return commInfo(inf1,inf2, caller2);
	else{
		printf("IndistB: Error\n");
		return 0;
	}
}