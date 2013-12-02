#include "scenarie.h"
#include "controller.h"

typedef struct {
	char mode[10];
} COMMANDS;

typedef struct {
	char unit[40];
} UNITNAME;

typedef enum { 
	turn_on, turn_off, status, scenarie
} ACTIONTYPE;

typedef struct {
	ACTIONTYPE type;
	union item { 
		CONTROLLERS controller; 
		SCENARIE scenarie;
	};
	USERS user;
} ACTION;

/* Definerer database filer */
#define FILE_USERS "users.txt"

#define ERROR_OCCURRED -1

/* Prototyper */
/* Input/output - view */
int readInput(char *input[]);

/* Beregningslag - controller */
int splitString(const char *input, char *out[], int maxwords);
char *correct(char input[], int *like_percent);
/* Users */
int addUser(USERS users[], int len); /* NI */
int removeUser(USERS users[], int index, int len); /* NI */

/* Datalag - Model */
/* Users */
int readUsers(FILE *pFile, USERS users[]);
int saveUsers(FILE *pFile, const USERS users[], int len); /* NI */