#define FILE_USERS "users.txt"

typedef struct {
	int priority;
	char name[80];
} USERS;

int readUsers(USERS users[]);
int saveUsers(FILE *pFile, const USERS users[], int len); /* NI */
void printUsers(const USERS users[], int len);
void deleteUsers(const USERS users[], int len, char inputname[]);
void addUsers(const USERS users[], int len, char inputname[], int inputpriority);