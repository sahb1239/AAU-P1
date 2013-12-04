#include "scenarie.h"
#include "controller.h"

typedef enum {
	turn_on, turn_off, status, scenarie
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
#define SPLITSTRING_SIZE 15
#define INPUT_SIZE 80
#define SCENARIE_SIZE 50
#define CONTROLLER_SIZE 50
#define USERS_SIZE 50
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define NOTUNDERSTOOD_TEXT "Input blev ikke forst\x86et\n"
#else
	#define NOTUNDERSTOOD_TEXT "Input blev ikke forstået\n"
#endif

/* Prototyper */
/* Input/output - view */
int readInput(char *input[]);

/* Beregningslag - controller */
int splitString(const char *input, char *out[], int maxwords);
int strcmpI(const char *string1, const char *string2);

int findAndExecuteCommand(char *input[], int len, CONTROLLERS controllers[], int controllersLen, const SCENARIE scenarier[], int scenarierLen);

