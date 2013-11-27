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
	int num, allow_p1, allow_p2, allow_p3, c1_id, c2_id, c3_id, c1_state, c2_state, c3_state;
	char desc[80];
} SCENARIE;

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
int addScenarie(SCENARIE scenarier[], const int last_index);
void addScenarieRW (int *pp1, int *pp2, int *pp3, int *pstate1, int *pid1, int *pstate2, int *pid2, int *pstate3, int *pid3, char command[]);
