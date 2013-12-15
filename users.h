#define FILE_USERS "users.txt"
#define ERROR_OCCURRED -1
#define NAME_LEN 20
int strcmpI(const char *string1, const char *string2);

typedef struct {
	int priority;
	char name[NAME_LEN];
} USERS;

int readUsers(USERS users[]);
int selectUser(const USERS users[], int len, USERS *currentUser);
void printUsers(const USERS users[], int len);
void readInputUser(char name[], int askPriority, int *priority);
int removeUser(USERS users[], int len);
int removeUserC(USERS users[], int len, USERS user);
int addUser(USERS users[], int len);
void addUserC(USERS users[], USERS user, int len);
int saveUsers(const USERS users[], int len);