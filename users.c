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