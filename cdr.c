#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Κλήση της find.
//Στο αρχείο1 η συνάρτηση find καλείται ως εξής: find caller 030-4595559805 .Ενώ στα υπόλοιπα αρχεία καλείται χωρίς το "caller". Εμείς πως θα πρέπει να παίρνουμε την κλήση της συνάρτησης? Μήπως έχει γίνει κάποιο λάθος αφού βλέπω και στην lookup αλλα και στην εκφώνηση δεν υπάρχει το "caller" στην find.
typedef struct cdr{
	char *CDR_uniq_id;
	char *originator_number;
	char *destination_number;
	char *date;
	char *time;
	int duration;
	int type;
	int tarrif;
	int fault_cond;
}cdr;


void printCDR(cdr *CDR){
	printf("%s\n","*CDR PRINT*" );
	printf("\tCDR_uniq_id:%s \n\toriginator_number:%s \n\tdestination_number:%s \n\tdate:%s \n\ttime:%s \n\tduration:%d \n\ttype:%d \n\ttarrif:%d \n\tfault_cond:%d\n", CDR->CDR_uniq_id, CDR->originator_number, CDR->destination_number, CDR->date
		, CDR->time, CDR->duration, CDR->type, CDR->tarrif, CDR->fault_cond);
}

void destroyCDR(cdr *CDR){
	//printf("destroyCDR\n");
	if(CDR->CDR_uniq_id!=NULL)
		free(CDR->CDR_uniq_id);
	if(CDR->originator_number!=NULL)
		free(CDR->originator_number);
	if(CDR->destination_number!=NULL)
		free(CDR->destination_number);
	if(CDR->date!=NULL)	
		free(CDR->date);
	if(CDR->time!=NULL)
		free(CDR->time);
	free(CDR);
}

