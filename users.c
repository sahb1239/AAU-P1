#include <stdio.h>
#include "users.h"
#include "danish.h"

int main(void) {
	USERS users[80];
	int i, len = readUsers(users);
	
	for (i = 0; i < len; i++) {
		printf("%s\t%d", users[i].name, users[i].priority);
	}
}

int readUsers(USERS users[]) {
	int i;
    
    FILE *pFile = fopen(FILE_USERS, "r");

	if (pFile == NULL) {
		return -1;
	}
    
    for (i = 0; !feof(pFile); i++) {
    	fscanf(pFile, " %[^1-9]s %d", users[i].name, &users[i].priority);
    	printf("HEY");
    }
      
	fclose(pFile);

	return i;
}