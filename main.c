#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DATASIZE 5
#include "hashtable.c"
#include "heap.c"
//#include "cdr.h"

int main(int argc, char *argv[]){
	int i,h1size,h2size,bsize,bucketbytes;
	FILE *fp;
	char *filename, *confname, *line=NULL;
	size_t len = 0;
    ssize_t read;
	if (argc > 1){					//collecting the arguments
    	for (i = 1; i < argc; i++){
    		//printf("argv[%d] = %s\n",i, argv[i] );
    		if(strcmp(argv[i], "-o")==0){
    			filename=(char*)malloc(strlen(argv[i+1])*sizeof(char));
    			strcpy(filename, argv[i+1]);
    		}
	  		else if(strcmp(argv[i], "-h1")==0)
	  			h1size=atoi(argv[i+1]);
	  		else if(strcmp(argv[i], "-h2")==0)
	  			h2size =atoi(argv[i+1]);
	  		else if(strcmp(argv[i], "-b")==0)
	  			bucketbytes =atoi(argv[i+1]);
	  		else if(strcmp(argv[i], "-c")==0){
	  			confname=(char*)malloc(strlen(argv[i+1])*sizeof(char));
    			strcpy(confname, argv[i+1]);
	  		}
	  	}
	}
	HashTable *Hash1 = createTable(h1size,bsize);
	HashTable *Hash2 = createTable(h2size,bsize);
	Heap *heap = createHeap();
	bsize = bucketbytes/sizeof(Bucket);


	fp = fopen(filename, "r");
	if(fp==NULL) printf("Error: Failed to open the file.\n");
	while ((read = getline(&line, &len, fp)) != -1) {
        char *token, s[2] = " ";
   		token = strtok(line, s);
   		if(strcmp(token, "insert")==0){			//	INSERT 
   			printf("CALL INSERT\n");
   			cdr *rec = malloc(sizeof(cdr));
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	int element=0;
			    	char *record, ss[2]=";";
			    	record = strtok(token, ss);
			    	while( record != NULL ) {
			    		if(element == 0){
			    			rec->CDR_uniq_id =(char *)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->CDR_uniq_id,record);
			    		}
			    		else if(element == 1){
			    			rec->originator_number=(char *)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->originator_number,record);
			    		}
			    		else if(element == 2){
			    			rec->destination_number=(char *)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->destination_number,record);
			    		}
			    		else if(element == 3){
			    			rec->date = (char*)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->date,record);
			    		}
			    		else if(element == 4){
			    			rec->time = (char*)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->time,record);
			    		}
			    		else if(element == 5)
			    			rec->duration=atoi(record);
			    		else if(element == 6)
			    			rec->type=atoi(record);
			    		else if(element == 7)
			    			rec->tarrif=atoi(record);
			    		else if(element == 8)
			    			rec->fault_cond=atoi(record);
			    		record = strtok(NULL, ss);
			    		element++;
   					}
				}
			}
			insertTable(Hash1, rec, rec->originator_number, DATASIZE, bsize);
			insertTable(Hash2, rec, rec->destination_number, DATASIZE, bsize);
			if(rec->fault_cond==200)											//check fault cond
				InsertHeap(heap, rec, confname);
   		}
   		else if(strcmp(token, "delete")==0){
   			printf("CALL DELETE\n");
   			char *cdr_id, *caller; 
   			int element=0;
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	if(element==0){
			    		cdr_id=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(cdr_id,token);
			    		element++;
			    	}
			    	else{
			    		caller=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller,token);
			    	}
			    }
			}
			caller[14]='\0';
			cdr * CDR = returnCDR(Hash1,cdr_id,caller);
			if(CDR!=NULL) 
				deleteHeap(heap, CDR, confname);
			deleteTable(Hash1,cdr_id,caller, 1);
			deleteTable(Hash2,cdr_id,caller, 2);
			if(caller)
				free(caller);
			if(cdr_id)
				free(cdr_id);
	    }
	    else if(strcmp(token, "find")==0){				
   			char *caller, *t1=NULL, *t2=NULL, *y1=NULL, *y2=NULL; 
   			int element=1;
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	/*if (element== 0)			//for the file which called wrong
			    		element++;
			    	else*/if(element ==1){
			    		caller=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller,token);
			    		caller[14]='\0';
			    		element++;
			    	}
			    	else if(element == 2){
			    		t1=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t1,token);
			    		element++;
			    	}
			    	else if(element == 3){
			    		t2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t2,token);
			    		element++;
			    	}
			    	else if(element == 4){
			    		y1=(char*)malloc(strlen(t2)*sizeof(char));
			    		strcpy(y1,t2);
			    		free(t2);
			    		t2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t2,token);
			    		element++;
			    	}
			    	else if(element == 5){
			    		y2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(y2,token);
			    		element++;
			    	}
			    }
			}
			if(element>=3)			//fixing some '\n' problems
				if(element>5)
					y2[8]='\0';
				else
					t2[5]='\0';
			printf("CALL FIND: caller:|%s| t1:|%s| y1:|%s| t2:|%s| y2:|%s|\n",caller,t1,y1,t2,y2 );
			findHash(Hash1, caller, t1, y1, t2, y2);
			if(caller)
				free(caller);
			if(t1)
				free(t1);
			if(t2)
				free(t2);
			if(y1)
				free(y1);
			if(y2)
				free(y2);
		}
		else if(strcmp(token, "lookup")==0){
			char *caller, *t1=NULL, *t2=NULL, *y1=NULL, *y2=NULL; 
   			int element=0;
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	if(element ==0){
			    		caller=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller,token);
			    		caller[14]='\0';
			    		element++;
			    	}
			    	else if(element == 1){
			    		t1=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t1,token);
			    		element++;
			    	}
			    	else if(element == 2){
			    		t2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t2,token);
			    		element++;
			    	}
			    	else if(element == 3){
			    		y1=(char*)malloc(strlen(t2)*sizeof(char));
			    		strcpy(y1,t2);
			    		free(t2);
			    		t2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t2,token);
			    		element++;
			    	}
			    	else if(element == 4){
			    		y2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(y2,token);
			    		element++;
			    	}
			    }
			}
			if(element>=2)			//fixing some '\n' problems
				if(element>4)
					y2[8]='\0';
				else
					t2[5]='\0';
			printf("CALL LOOKUP: caller:|%s| t1:|%s| y1:|%s| t2:|%s| y2:|%s|\n",caller,t1,y1,t2,y2 );
			lookupHash(Hash2, caller, t1, y1, t2, y2);
			if(caller)
				free(caller);
			if(t1)
				free(t1);
			if(t2)
				free(t2);
			if(y1)
				free(y1);
			if(y2)
				free(y2);
		}
		else if(strcmp(token, "indist")==0){
			char *caller1, *caller2;
			int element=0;
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	if(element ==0){
			    		caller1=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller1,token);
			    		caller1[14]='\0';
			    		element++;
			    	}
			    	else{
			    		caller2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller2,token);
			    		caller2[14]='\0';
			    	}
			    }
			}
			printf("INDIST: C1:|%s| C2:|%s|\n",caller1, caller2);
			indistHash(Hash1, Hash2, caller1, caller2);
			if(caller1)
				free(caller1);
			if(caller2)
				free(caller2);
		}
		else if(strcmp(token, "topdest")==0){
			char *caller;
			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			   		caller=(char*)malloc(strlen(token)*sizeof(char));
			   		strcpy(caller,token);
			   		caller[14]='\0';
				}
			}
			topdestHash(Hash1, caller);
			if(caller)
				free(caller);
		}
		else if(strcmp(token, "bye")==0){
			destroyTable(Hash1,1);			//if this hash table destroyed we lose all the information we have !!!!!!!!!
			destroyTable(Hash2,2);
			destroyHeap(heap);
		}
		else if(strcmp(token, "print")==0){
			while( token != NULL ) {
			    token = strtok(NULL, s);
			    token[10]='\0';
			    if(strcmp(token,"hashtable1")==0){
			    	printTable(Hash1);
			    	break;
			    }
			    else{
			    	printTable(Hash2);
			    	break;
			    }
			}
		}
	}
	if(line)
		free(line)
	printf("************************************************************************************************\n");
	Hash1 = createTable(h1size,bsize);
	Hash2 = createTable(h2size,bsize);
	heap = createHeap();

	while (1) {
		printf("Give a command: \n");
		gets(line);
        char *token, s[2] = " ";
   		token = strtok(line, s);
   		if(strcmp(token, "insert")==0){			//	INSERT 
   			printf("CALL INSERT\n");
   			cdr *rec = malloc(sizeof(cdr));
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	int element=0;
			    	char *record, ss[2]=";";
			    	record = strtok(token, ss);
			    	while( record != NULL ) {
			    		if(element == 0){
			    			rec->CDR_uniq_id =(char *)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->CDR_uniq_id,record);
			    		}
			    		else if(element == 1){
			    			rec->originator_number=(char *)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->originator_number,record);
			    		}
			    		else if(element == 2){
			    			rec->destination_number=(char *)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->destination_number,record);
			    		}
			    		else if(element == 3){
			    			rec->date = (char*)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->date,record);
			    		}
			    		else if(element == 4){
			    			rec->time = (char*)malloc(strlen(record)*sizeof(char));
			    			strcpy(rec->time,record);
			    		}
			    		else if(element == 5)
			    			rec->duration=atoi(record);
			    		else if(element == 6)
			    			rec->type=atoi(record);
			    		else if(element == 7)
			    			rec->tarrif=atoi(record);
			    		else if(element == 8)
			    			rec->fault_cond=atoi(record);
			    		record = strtok(NULL, ss);
			    		element++;

   					}
				}
			}
			insertTable(Hash1, rec, rec->originator_number, DATASIZE, bsize);
			insertTable(Hash2, rec, rec->destination_number, DATASIZE, bsize);
			if(rec->fault_cond==200)											//check fault cond
				InsertHeap(heap, rec, confname);
   		}
   		else if(strcmp(token, "delete")==0){
   			printf("CALL DELETE\n");
   			char *cdr_id, *caller; 
   			int element=0;
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	if(element==0){
			    		cdr_id=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(cdr_id,token);
			    		element++;
			    	}
			    	else{
			    		caller=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller,token);
			    	}
			    }
			}
			caller[14]='\0';
			cdr * CDR = returnCDR(Hash1,cdr_id,caller);
			if(CDR!=NULL) 
				deleteHeap(heap, CDR, confname);
			deleteTable(Hash1,cdr_id,caller, 1);
			deleteTable(Hash2,cdr_id,caller, 2);
			if(caller)
				free(caller);
			if(cdr_id)
				free(cdr_id);
	    }
	    else if(strcmp(token, "find")==0){	
   			char *caller, *t1=NULL, *t2=NULL, *y1=NULL, *y2=NULL; 
   			int element=1;
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	/*if (element== 0)			//for the file which called wrong
			    		element++;
			    	else*/if(element ==1){
			    		caller=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller,token);
			    		caller[14]='\0';
			    		element++;
			    	}
			    	else if(element == 2){
			    		t1=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t1,token);
			    		element++;
			    	}
			    	else if(element == 3){
			    		t2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t2,token);
			    		element++;
			    	}
			    	else if(element == 4){
			    		y1=(char*)malloc(strlen(t2)*sizeof(char));
			    		strcpy(y1,t2);
			    		free(t2);
			    		t2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t2,token);
			    		element++;
			    	}
			    	else if(element == 5){
			    		y2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(y2,token);
			    		element++;
			    	}
			    }
			}
			if(element>=3)			//fixing some '\n' problems
				if(element>5)
					y2[8]='\0';
				else
					t2[5]='\0';
			printf("CALL FIND: caller:|%s| t1:|%s| y1:|%s| t2:|%s| y2:|%s|\n",caller,t1,y1,t2,y2 );
			findHash(Hash1, caller, t1, y1, t2, y2);
			if(caller)
				free(caller);
			if(t1)
				free(t1);
			if(t2)
				free(t2);
			if(y1)
				free(y1);
			if(y2)
				free(y2);
		}
		else if(strcmp(token, "lookup")==0){
			char *caller, *t1=NULL, *t2=NULL, *y1=NULL, *y2=NULL; 
   			int element=0;
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	if(element ==0){
			    		caller=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller,token);
			    		caller[14]='\0';
			    		element++;
			    	}
			    	else if(element == 1){
			    		t1=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t1,token);
			    		element++;
			    	}
			    	else if(element == 2){
			    		t2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t2,token);
			    		element++;
			    	}
			    	else if(element == 3){
			    		y1=(char*)malloc(strlen(t2)*sizeof(char));
			    		strcpy(y1,t2);
			    		free(t2);
			    		t2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(t2,token);
			    		element++;
			    	}
			    	else if(element == 4){
			    		y2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(y2,token);
			    		element++;
			    	}
			    }
			}
			if(element>=2)			//fixing some '\n' problems
				if(element>4)
					y2[8]='\0';
				else
					t2[5]='\0';
			printf("CALL LOOKUP: caller:|%s| t1:|%s| y1:|%s| t2:|%s| y2:|%s|\n",caller,t1,y1,t2,y2 );
			lookupHash(Hash2, caller, t1, y1, t2, y2);
			if(caller)
				free(caller);
			if(t1)
				free(t1);
			if(t2)
				free(t2);
			if(y1)
				free(y1);
			if(y2)
				free(y2);
		}
		else if(strcmp(token, "indist")==0){
			char *caller1, *caller2;
			int element=0;
   			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			    	if(element ==0){
			    		caller1=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller1,token);
			    		caller1[14]='\0';
			    		element++;
			    	}
			    	else{
			    		caller2=(char*)malloc(strlen(token)*sizeof(char));
			    		strcpy(caller2,token);
			    		caller2[14]='\0';
			    	}
			    }
			}
			printf("INDIST: C1:|%s| C2:|%s|\n",caller1, caller2);
			indistHash(Hash1, Hash2, caller1, caller2);
			if(caller1)
				free(caller1);
			if(caller2)
				free(caller2);
		}
		else if(strcmp(token, "topdest")==0){
			char *caller;
			while( token != NULL ) {
			    token = strtok(NULL, s);
			    if(token!=NULL){
			   		caller=(char*)malloc(strlen(token)*sizeof(char));
			   		strcpy(caller,token);
			   		caller[14]='\0';
				}
			}
			topdestHash(Hash1, caller);
			if(caller)
				free(caller);
		}
		else if(strcmp(token, "bye")==0){
			destroyTable(Hash1,1);			//if this hash table destroyed we lose all the information we have !!!!!!!!!
			destroyTable(Hash2,2);
			destroyHeap(heap);


		}
		else if(strcmp(token, "exit")==0)
			break;
		else if(strcmp(token, "print")==0){
			while( token != NULL ) {
			    token = strtok(NULL, s);
			    token[10]='\0';
			    if(strcmp(token,"hashtable1")==0){
			    	printTable(Hash1);
			    	break;
			    }
			    else{
			    	printTable(Hash2);
			    	break;
			    }
			}
		}
	} 																												
    //printTable(Hash1);		
 	if(filename)
 		free(filename);
 	if(confname)
 		free(confname);
	fclose(fp);
	printf("END\n");
}