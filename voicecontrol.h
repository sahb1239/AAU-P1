#include "scenarie.h"
#include "controller.h"

typedef enum {
	turn_on, turn_off, status, scenarie, add_controller, remove_controller, add_scenarie, remove_scenarie, help, status_all, scenarie_all
} ACTIONTYPE;

/*
typedef struct {
	ACTIONTYPE type;
	union item { 
		CONTROLLERS controller; 
		SCENARIE scenarie;
	};
	USERS user;
} ACTION;*/

/*#define ERROR_OCCURRED -1*/
#define UNIT_NAME "jarvis"
#define SPLITSTRING_SIZE 50
#define INPUT_SIZE 80
#define SCENARIE_SIZE 50
#define CONTROLLER_SIZE 50
#define USERS_SIZE 50

#define NOTUNDERSTOOD_TEXT "Input blev ikke forst%set\n"

/* Prototyper */
void helpMe(void);
/* Input/output - view */
int readInput(char *input[]);

/* Beregningslag - controller */
int splitString(const char *input, char *out[], int maxwords);
int strcmpI(const char *string1, const char *string2);

int findAndExecuteCommand(char *input[], int len, CONTROLLERS controllers[], int *controllersLen, SCENARIE scenarier[], int *scenarierLen);

