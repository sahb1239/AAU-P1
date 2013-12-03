#include <stdio.h>
#include <string.h>
#include "users.h"
#include "danish.h"

int readUsers(USERS users[]) {
	int i;
    
    FILE *pFile = fopen(FILE_USERS, "r");

	if (pFile == NULL) {
		return -1;
	}
    
    for (i = 0; !feof(pFile); i++) {
    	fscanf(pFile, " %d %[^\n]s", &users[i].priority, users[i].name);
    }
    
	fclose(pFile);

	return i;
}

void printUsers(const USERS users[], int len) {
    int i;
    	printf("Prioritet:\tBruger:\n");
    for (i = 0; i < len; i++) {
    	printf("%d\t\t%s\n", users[i].priority, users[i].name);
	}
}

void deleteUsers(const USERS users[], int len, char inputname[80]){
	int i;
   	FILE *pFile1 = fopen(FILE_USERS, "w");
    for (i = 0; i<len; i++) {
    	if (strcmp(users[i].name, inputname) == 0){
    	}else{
    		fprintf("%d\t%s\n", users[i].priority, users[i].name);
    	}
    }
	fclose(pFile1);  
}