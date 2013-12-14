#define FILE_USERS "users.txt"
#define ERROR_OCCURRED -1
int strcmpI(const char *string1, const char *string2);

typedef struct {
	int priority;
	char name[80];
} USERS;

int readUsers(USERS users[]);
void selectUser(const USERS users[], int len, USERS *currentUser);
void printUsers(const USERS users[], int len);
void readInputUser(char name[], int askPriority, int *priority);
int removeUser(USERS users[], int len);
int removeUserC(USERS users[], int len, USERS user);
int addUser(USERS users[], int len);
void addUserC(USERS users[], USERS user, int len);
int saveUsers(const USERS users[], int len);