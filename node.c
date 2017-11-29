#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct Node{
	char* key;
	float ammount;
	struct Node* left;
	struct Node* right;
}Node;


float calculateAmmount(cdr* record, char* config){
	int element=0, type, tarif;
	float cost;
	FILE *fp;
	char *line=NULL;
	size_t len = 0;
    ssize_t read;
	fp = fopen(config, "r");
	if(fp==NULL) printf("Error: Failed to open the file.\n");
	while ((read = getline(&line, &len, fp)) != -1) {
		/*if(line[0]=="#")
			continue;*/
        char *token, s[2] = ";";
   		token = strtok(line, s);
   		while( token != NULL ) {
			if(element==0)
				type=atoi(token);
			else if(element==1)
				tarif=atoi(token);
			else if(element==2){
				cost=atof(token);
			}
			token = strtok(NULL, s);
			element++;
		}
		element=0;
		//printf("type:%d tarrif:%d cost:%f\n",type, tarif, cost );
		//printf("-and we have type:%d tarrif:%d duration: %d\n",record->type, record->tarrif, record->duration );
		if(type == 0 && record->type==0){
			fclose(fp);
			return cost;
		}
		if(type==record->type){
			if(tarif==record->tarrif){
				fclose(fp);
				return record->duration*cost;
			}
		}
	}
	if(line) free(line);
	printf("ERROR:calculate ammount\n");
	fclose(fp);
	return 0;
}


Node* createNode(cdr *record, char* config){
	Node* node = malloc(sizeof(Node));
	node->key = (char*)malloc(strlen(record->originator_number)*sizeof(char));
	strcpy(node->key,record->originator_number);
	node->left=NULL;
	node->right=NULL;
	node->ammount=calculateAmmount(record, config);			//Calculate the ammount
	printf("AMMOUNT %f\n",node->ammount );
	return node;
}

void updateNode(Node* found,cdr* record, char* config, char act){
	if(act=='+')
		found->ammount = found->ammount + calculateAmmount(record, config);
	else
		found->ammount = found->ammount - calculateAmmount(record, config);
}

void destroyNode(Node* node){
	if(node->key)
		free(node->key);
	free(node);
}

