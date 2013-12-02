#define ERROR_OCCURRED -1
#define FILE_CONTROLLERS "controllers.txt"

typedef struct {
	int id, status;
	char unit[80], position[80];
} CONTROLLERS;

/* View */
int addController(CONTROLLERS controllers[], int len);
void readInputController (int *id, char genstand[], char placering[]);
void statusControllerPrint (const CONTROLLERS controllers[], int i);
void statusControllerPrintAll (const CONTROLLERS controllers[], int len);

/* Controller */
void addControllerC(CONTROLLERS controllers[], CONTROLLERS controller, int len);
int removeController(CONTROLLERS controllers[], int index, int len);
int changeControllerState(CONTROLLERS controllers[], int cid, int state, int len);
int controllerState (const CONTROLLERS controllers[], int cid, int len);

/* Model */
int readControllers(CONTROLLERS controllers[]);
int saveControllers(const CONTROLLERS controllers[], int len); /* NI */