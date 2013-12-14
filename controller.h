#define ERROR_OCCURRED -1
#define FILE_CONTROLLERS "controllers.txt"
#define STATUS_LEN 8
#define UNIT_NAME_LEN 30
#define POSITION_NAME_LEN 30

typedef struct {
	int id, status;
	char unit[UNIT_NAME_LEN], position[POSITION_NAME_LEN];
} CONTROLLERS;

void readInputController (char genstand[], char placering[]);
void statusControllerPrint (const CONTROLLERS controllers[], int index);
void statusControllerPrintAll (const CONTROLLERS controllers[], int len);

int addController(CONTROLLERS controllers[], int *len);
int removeController(CONTROLLERS controllers[], int *len);
void changeControllerState(CONTROLLERS controllers[], int index, int state, int len);

int findControllerFromName(const CONTROLLERS controllers[], const char name[], const char room[], int len);
int findControllerFromId(const CONTROLLERS controllers[], int cid, int len);

int addControllerC(CONTROLLERS controllers[], CONTROLLERS controller, int *len);
int removeControllerC(CONTROLLERS controllers[], int index, int *len);

int readControllers(CONTROLLERS controllers[]);
int saveControllers(const CONTROLLERS controllers[], int len);