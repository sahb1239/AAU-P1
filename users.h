typedef struct {
	char name[80];
	int priority;
} USERS;

int readUsers(USERS users[]);
int saveUsers(FILE *pFile, const USERS users[], int len); /* NI */

#define FILE_USERS "users.txt"