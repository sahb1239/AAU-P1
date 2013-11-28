typedef struct {
	char firstName[30], lastName[30];
	int priority;
} USERS;

typedef struct {
	char mode[10];
} COMMANDS;

typedef struct {
	char unit[40];
} UNITNAME;

typedef struct {
	char room[40];
} HOUSE;

/* Definerer database filer */
#define FILE_USERS "users.txt"
#define FILE_CONTROLLERS "controllers.txt"
#define FILE_COMMANDS "commands.txt"
#define FILE_SCENARIE "scenarier.txt"
#define FILE_UNITNAME "unitname.txt"
#define FILE_HOUSE "house.txt"

#define ERROR_OCCURRED -1

/* Prototyper */
/* Input/output - view */
int readInput(char *input[]);

/* Beregningslag - controller */
int splitString(const char *input, char *out[], int maxwords);
/* Users */
int addUser(USERS users[], int len); /* NI */
int removeUser(USERS users[], int index, int len); /* NI */

/* Datalag - Model */
/* Users */
int readUsers(FILE *pFile, USERS users[]);
int saveUsers(FILE *pFile, const USERS users[], int len); /* NI */