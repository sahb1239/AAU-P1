#define ERROR_OCCURRED -1
#define FILE_CONTROLLERS "controllers.txt"

typedef struct {
	int id, status;
	char unit[80], position[80];
} CONTROLLERS;

int addController(CONTROLLERS controllers[], int len);
void readInputController (char genstand[], char placering[]);
void statusControllerPrint (const CONTROLLERS controllers[], int index);
void statusControllerPrintAll (const CONTROLLERS controllers[], int len);

void addControllerC(CONTROLLERS controllers[], CONTROLLERS controller, int len);
int removeController(CONTROLLERS controllers[], int len);
int changeControllerState(CONTROLLERS controllers[], int cid, int state, int len);
int controllerState (const CONTROLLERS controllers[], int cid, int len);
int findController(const CONTROLLERS controllers[], const char name[], const char room[], int len);
int controllerIDtoIndex(const CONTROLLERS controllers[], int cid, int len);

int readControllers(CONTROLLERS controllers[]);
int saveControllers(const CONTROLLERS controllers[], int len);