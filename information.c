#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct information {
	cdr** data;
	int size;
	struct information* next;
}information;

typedef struct communicateInfo{			//krataei plhroforia me poious exei epikoinwnisei o caller
	char **comm;
	int size;
}communicateInfo;

int insertInfo(information* info, cdr *record){
	int ii;
	information *temp=info;
	while(1){
		for(ii=0;ii<temp->size;ii++){
			if(temp->data[ii]==NULL){
				temp->data[ii]=record;
				return 1;
			}	
		}
		if(temp->next!=NULL){
			temp=temp->next;
			continue;
		}
		else{//in this case we dont have space so we create the next one;
			printf("INSERTinfo: create next info\n");
			information *i= malloc(sizeof(information));
			i->size=info->size;
			i->next=NULL;
			i->data = malloc(sizeof(cdr)*info->size);
			int k;
			for(k=0;k<info->size;k++)
				i->data[k]=NULL;
			temp->next=i;
			i->data[0]=record;
			return 1;
		}
	}
	return 0;
}

void printInfo(information *inf){
	int i;
	do{
		for(i=0;i<inf->size;i++){
			if(inf->data[i]==NULL)
				printf("\t\t->NULL\n");
			else
				printf("\t\t->CDR:%s\n",inf->data[i]->CDR_uniq_id);
		}
		inf=inf->next;
	}while(inf!=NULL);
}

void destroyInfo(information *inf, int h){
	int i;
	do{
		for(i=0;i<inf->size;i++){
			if(inf->data[i]!=NULL){
				if(h==1)		//only if its the first hash delete the cdrs
					destroyCDR(inf->data[i]);
				inf->data[i]=NULL;				
			}
		}
		information *nexti=inf->next;
		free(inf->data);
		free(inf);
		inf=nexti;
	}while(inf!=NULL);
	//printf("destroyInfo: information Destroyed\n");
}

information* deleteInfo(information* inf, char *cdr_id, char *caller, int h){
	int i, found=0, empty=0;
	information* temp=inf;
	while(!found){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				printf("%s===%s\n",temp->data[i]->CDR_uniq_id,cdr_id );
				if(strcmp(temp->data[i]->CDR_uniq_id,cdr_id)==0){
					found=1;
					if(h==1)
						destroyCDR(temp->data[i]);
					temp->data[i]=NULL;
				}
			}
		}
		if(temp->next!=NULL && !found) {
			temp=temp->next;
		}
		if(temp->next==NULL) break;
	}
	if(!found){
		printf("DeleteI:error caller: %s not found.\n",caller);
		return inf;
	}
	for(i=0;i<temp->size;i++){
		if(temp->data[i]!=NULL)
			empty++;
	}
	if(empty==0){ //we have an empty data fix here
		information *temp2;
		if(temp== inf){
			temp2=temp->next;
			for(i=0;i<temp->size;i++)
				free(temp->data[i]);
			free(temp);
			return temp2;
		}
		else{
			temp2=inf;
			while(temp2->next != temp)
				temp2=temp2->next;
			information *temp3;
			temp3=temp->next;
			for(i=0;i<temp->size;i++)
				free(temp->data[i]);
			free(temp);
			temp2->next =temp3;		//fix the row of inf and we return the usual
			return inf;
		}
	}
	else
		return inf;
}


void findInfo(information* inf, char *caller, char *t1, char *y1, char *t2, char *y2){
	information* temp=inf;
	int i;
	while(1){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				if(t1==NULL){			//in this case he gave us only the caller so we print them all
					printCDR(temp->data[i]);
				}
				else if(y1==NULL){		// in this case he gave us only time 
					if(strcmp(t1,t2)<0){
						if(strcmp(t1,temp->data[i]->time)<0 && strcmp(t2,temp->data[i]->time)>0)
							printCDR(temp->data[i]);
					}
					else{
						if(strcmp(t1,temp->data[i]->time)>0 && strcmp(t2,temp->data[i]->time)<0)
							printCDR(temp->data[i]);
					}
				}
				else{
					if(strcmp(y1,y2)<0){
						if(strcmp(y1,temp->data[i]->date)<0 && strcmp(y2,temp->data[i]->date)>0){
							if(strcmp(t1,t2)<0){
								if(strcmp(t1,temp->data[i]->time)<0 && strcmp(t2,temp->data[i]->time)>0)
									printCDR(temp->data[i]);
							}
							else{
								if(strcmp(t1,temp->data[i]->time)>0 && strcmp(t2,temp->data[i]->time)<0)
									printCDR(temp->data[i]);
							}
						}
					}
					else{
						if(strcmp(y2,temp->data[i]->date)<0 && strcmp(y2,temp->data[i]->date)>0){
							if(strcmp(t1,t2)<0){
								if(strcmp(t1,temp->data[i]->time)<0 && strcmp(t2,temp->data[i]->time)>0)
									printCDR(temp->data[i]);
							}
							else{
								if(strcmp(t1,temp->data[i]->time)>0 && strcmp(t2,temp->data[i]->time)<0)
									printCDR(temp->data[i]);
							}
						}
					}
				}
			}
		}
		if(temp->next!=NULL)
			temp=temp->next;
		else
			break;
	}
}


void lookupInfo(information* inf, char *caller, char *t1, char *y1, char *t2, char *y2){
	information* temp=inf;
	int i;
	while(1){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				if(t1==NULL){			//in this case he gave us only the caller so we print them all
					printf("Called form : %s\n",temp->data[i]->originator_number);
				}
				else if(y1==NULL){		// in this case he gave us only time 
					if(strcmp(t1,t2)<0){
						if(strcmp(t1,temp->data[i]->time)<0 && strcmp(t2,temp->data[i]->time)>0)
							temp->data[i]->originator_number;
					}
					else{
						if(strcmp(t1,temp->data[i]->time)>0 && strcmp(t2,temp->data[i]->time)<0)
							temp->data[i]->originator_number;
					}
				}
				else{
					if(strcmp(y1,y2)<0){
						if(strcmp(y1,temp->data[i]->date)<0 && strcmp(y2,temp->data[i]->date)>0){
							if(strcmp(t1,t2)<0){
								if(strcmp(t1,temp->data[i]->time)<0 && strcmp(t2,temp->data[i]->time)>0)
									temp->data[i]->originator_number;
							}
							else{
								if(strcmp(t1,temp->data[i]->time)>0 && strcmp(t2,temp->data[i]->time)<0)
									temp->data[i]->originator_number;
							}
						}
					}
					else{
						if(strcmp(y2,temp->data[i]->date)<0 && strcmp(y2,temp->data[i]->date)>0){
							if(strcmp(t1,t2)<0){
								if(strcmp(t1,temp->data[i]->time)<0 && strcmp(t2,temp->data[i]->time)>0)
									temp->data[i]->originator_number;
							}
							else{
								if(strcmp(t1,temp->data[i]->time)>0 && strcmp(t2,temp->data[i]->time)<0)
									temp->data[i]->originator_number;
							}
						}
					}
				}
			}
		}
		if(temp->next!=NULL)
			temp=temp->next;
		else
			break;
	}
}


void topdestInfo(information* inf, char *caller){
	information *temp=inf, *temp2;
	int i,k,j, count, max=0;
	char maxcode[3], code[3], code2[3];
	while(1){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				for(j=0;j<3;j++)
					code[j]=temp->data[i]->destination_number[j];
			temp2=inf;
			count = 0;
			do{	
				for(k=0;k<temp2->size;k++){
					if(temp2->data[k]!=NULL){
						for(j=0;j<3;j++)
							code2[j]=temp->data[k]->destination_number[j];
						if(strcmp(code,code2)==0)
							count++;
					}
				}
				temp2=temp2->next;
			}while(temp2!=NULL);
			if(count>max){
				max=count;
				strcpy(maxcode,code);
			}
		}
	}
	if(temp->next!=NULL)
		temp=temp->next;
	else
		break;
	}
	printf("Topdest: CALLER: %s\nMOST CALLED CODE: %s\nCOUNT: %d\n",caller, maxcode, max);
}

/*
int checkInfo(information* inf, char *caller2, int h){									//returns 1 if they have communicate
	information* temp=inf;
	int i;
	while(1){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				if(h==1){
					if(strcmp(temp->data[i]->destination_number,caller2)==0)			//caller1 called caller2
						return 1;
				}
				else{
					if(strcmp(temp->data[i]->originator_number,caller2)==0)				//caller2 called caller1
						return 1;
				}
			}
		}
		if(temp->next!=NULL)
			temp=temp->next;
		else
			break;
	}
	return 0;			
}
*/
cdr* cdrInfo(information* inf, char *cdr_id, char *caller){
	int i, found=0;
	information* temp=inf;
	while(!found){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				if(strcmp(temp->data[i]->CDR_uniq_id,cdr_id)==0)
					return temp->data[i];
			}
		}
		if(temp->next!=NULL && !found) {
			temp=temp->next;
		}
		if(temp->next==NULL) break;
	}
	if(!found){
		printf("returnI:error caller: %s not found.\n",caller);
		return NULL;
	}
}


communicateInfo* indistInfo(information* inf1, information* inf2){
	int size=0, k, i;
	information* temp=inf1;
	while(temp!=NULL){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL)
				size++;
		}
		temp=temp->next;
	}
	temp = inf2;
	while(temp!=NULL){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL)
				size++;
		}
		temp=temp->next;
	}
	communicateInfo* Cinfo = malloc(sizeof(communicateInfo));
	Cinfo->comm = malloc(sizeof(char*)*size);
	k=0;
	while(temp!=NULL){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				Cinfo->comm[k]=(char*)malloc(sizeof(char)*strlen(temp->data[i]->destination_number));
				strcpy(Cinfo->comm[k],temp->data[i]->destination_number);
				k++;
			}
		}
		temp=temp->next;
	}
	temp = inf2;
	while(temp!=NULL){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				Cinfo->comm[k]=(char*)malloc(sizeof(char)*strlen(temp->data[i]->originator_number));
				strcpy(Cinfo->comm[k],temp->data[i]->originator_number);
				k++;
			}
		}
		temp=temp->next;
	}
	printf("!!!!!!!!!!!!!!%d k%d\n",size, k );
	Cinfo->size=size;
	return Cinfo;
}

int commInfo(information* inf1, information* inf2, char* caller){									//returns 1 if they have communicate
	int size=0, k, i;
	information* temp=inf1;
	while(temp!=NULL){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				if(strcmp(caller, temp->data[i]->destination_number)==0)
					return 1;
			}
		}
		temp=temp->next;
	}
	temp = inf2;
	while(temp!=NULL){
		for(i=0;i<temp->size;i++){
			if(temp->data[i]!=NULL){
				if(strcmp(caller, temp->data[i]->originator_number)==0)
					return 1;
			}
		}
		temp=temp->next;
	}
	return 0;
}

void destroyCOMM(communicateInfo *C){
	int i;
	for(i=0;i<C->size;i++)
		free(C->comm[i]);
	free(C->comm);
	free(C);
}
