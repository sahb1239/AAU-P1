#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char firstName[30], lastName[30];
	int priority;
} USERS;

typedef struct {
	int id;
	char unit[80], position[80];
} CONTROLLERS;

typedef struct {
	char mode[10];
} COMMANDS;

typedef struct {
	char unit[40];
} UNITNAME;

typedef struct {
	char room[40];
} HOUSE;

// Define Danish letters
#define ae "\x91"
#define oe "\x9B"
#define aa "\x86"
#define AE "\x92"
#define OE "\x9D"
#define AA "\x8F"

// Defining database-dir
#define FILE_USERS "users.txt"
#define FILE_CONTROLLERS "controllers.txt"
#define FILE_COMMANDS "commands.txt"
#define FILE_SCENARIE "scenarier.txt"
#define FILE_UNITNAME "unitname.txt"
#define FILE_HOUSE "house.txt"

// Defining
#define ERROR_OCCURRED -1

// Prototypes
/* Input/output - view */
int readInput(char *input[]);

/* Beregningslag - controller */
int splitString(const char *input, char *out[], int maxwords);
/* Controllers */
int addController(CONTROLLERS controllers[], int len); /* NI */
int removeController(CONTROLLERS controllers[], int len); /* NI */
/* Users */
int addUser(USERS users[], int len); /* NI */
int removeUser(USERS users[], int index, int len); /* NI */

/* Datalag - Model */
/* Controllers */
int readControllers(FILE *pFile, CONTROLLERS controllers[]);
int saveControllers(FILE *pFile, const CONTROLLERS controllers[], int len); /* NI */
/* Users */
int readUsers(FILE *pFile, USERS users[]);
int saveUsers(FILE *pFile, const USERS users[], int len); /* NI */