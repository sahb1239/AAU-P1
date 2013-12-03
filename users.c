#include <stdio.h>
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
    	printf("%d\t%s\n", users[i].priority, users[i].name);
	}
}