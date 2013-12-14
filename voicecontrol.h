#define STATUS_LEN 8
#include "scenarie.h"
#include "controller.h"
#include "users.h"

typedef enum {
	turn_on, turn_off, status, scenarie, add_controller, remove_controller, add_scenarie, remove_scenarie, help, status_all, scenarie_all, add_user, remove_user
} ACTIONTYPE;

#define ERROR_OCCURRED -1
#define UNIT_NAME "jarvis"
#define SPLITSTRING_SIZE 50
#define INPUT_SIZE 80
#define SCENARIE_SIZE 50
#define CONTROLLER_SIZE 50
#define USERS_SIZE 50
#define SPLITTED_INPUT_LEN 80
#define ANSWER_LEN 10

#define PERCENT_UNDERSTOOD_OK 80

#define NOTUNDERSTOOD_TEXT "Input blev ikke forst%set\n"
#define MEMORYERROR_TEXT "Fejl: ikke nok hukommelse"

/* Prototyper */
void helpMe(void);
/* Input/output - view */
int readInput(char *input[]);
int yesno(const char *text);

/* Beregningslag */
int splitString(const char *input, char *out[], int maxwords);
int strcmpI(const char *string1, const char *string2);
void checkPTRALLOC(void **ptr);

int findAndExecuteCommand(char *input[], int len, CONTROLLERS controllers[], int *controllersLen, SCENARIE scenarier[], int *scenarierLen, USERS users[], int *usersLen);

int executeNormalCommand (CONTROLLERS controllers[], SCENARIE scenarier[], char *controlScenarieTmp[], char position[], int *controllersLen, int *scenarierLen, int numactions, ACTIONTYPE type);
int executeSpecialCommand (CONTROLLERS controllers[], SCENARIE scenarier[], USERS users[], char *controlScenarieTmp[], char position[], int *controllersLen, int *scenarierLen, int *usersLen, int numactions, ACTIONTYPE type);