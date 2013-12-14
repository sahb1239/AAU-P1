#define FILE_USERS "users.txt"
int strcmpI(const char *string1, const char *string2);

typedef struct {
	int priority;
	char name[80];
} USERS;

int readUsers(USERS users[]);
void printUsers(const USERS users[], int len);
void readInputUser(char name[], int askPriority, int *priority);
int removeUser(USERS users[], int len);
int addUser(USERS users[], int len);
void addUserC(USERS users[], USERS user, int len);
int saveUsers(const USERS users[], int len);