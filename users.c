#include <stdio.h>
#include <string.h>
#include "users.h"
#include "danish.h"

int readUsers(USERS users[]) {
	int i;
    
    FILE *pFile = fopen(FILE_USERS, "r");

	if (pFile == NULL) 
	   return -1;
    
    for (i = 0; !feof(pFile); i++) {
    	fscanf(pFile, " %d %[^\n]s", &users[i].priority, users[i].name);
    }
    
	fclose(pFile);

	return i;
}

int selectUser(const USERS users[], int len, USERS *currentUser) {
   int i;
   USERS tmpUser;
   readInputUser(tmpUser.name, 0, &tmpUser.priority);
   
   for (i = 0; i < len; i++) {
     if (strcmpI(tmpUser.name, users[i].name) == 0) {
        strcpy(currentUser->name, users[i].name);
        currentUser->priority = users[i].priority;
        return 1;}
   }
   return ERROR_OCCURRED;
}

void printUsers(const USERS users[], int len) {
    int i;
    printf("Prioritet:\tBruger:\n");
    
    for (i = 0; i < len; i++) {
    	printf("%d\t\t%s\n", users[i].priority, users[i].name);
	}
}

void readInputUser(char name[], int askPriority, int *priority) {
	printf("Hvilken person er der tale om?\nIndtast input => ");
    scanf("%s", name);    
    if (askPriority == 1) {
      printf("Hvilken prioritet?\nIndtast input => ");
      scanf("%d", priority); }
}

int removeUser(USERS users[], int len) {
    
    USERS user;
	
	readInputUser(user.name, 0, &user.priority);

    return removeUserC(users, len, user);
}

int removeUserC(USERS users[], int len, USERS user) {
   int i, j, success = -1;
   
    for (i = 0; i < len; i++) {
       if (strcmpI(users[i].name, user.name) == 0) {
         len--;
          success = 1;
          
         for (j = i; j < len; j++) {
           	users[j] = users[j + 1];
         }
       }
    }
    if (success == 1)
       return saveUsers(users, len);
    else
       return ERROR_OCCURRED;
}

int addUser(USERS users[], int len) {
	USERS user;
	
	readInputUser(user.name, 1, &user.priority);
	addUserC(users, user, len);
	
	return 1;
}

void addUserC(USERS users[], USERS user, int len) {
    users[len].priority = user.priority;
	strcpy(users[len].name, user.name);
	saveUsers(users, len + 1);
}

int saveUsers(const USERS users[], int len) {
	int i;
   	FILE *pFile = fopen(FILE_USERS, "w");
    
    if (pFile == NULL)
	   return 0;
    
    for (i = 0; i < len; i++) {
    	fprintf(pFile, "\n%d\t%s", users[i].priority, users[i].name); }
        
	fclose(pFile);  
    
    return 1;
}