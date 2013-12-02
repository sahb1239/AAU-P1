typedef struct {
	char name[80];
	int priority;
} USERS;

int saveUsers(FILE *pFile, const USERS users[], int len); /* NI */

int addUser(USERS users[], int len); /* NI */
int removeUser(USERS users[], int index, int len); /* NI */

int readUsers(USERS users[]);

#define FILE_USERS "users.txt"