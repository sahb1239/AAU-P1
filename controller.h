#define ERROR_OCCURRED -1
#define FILE_CONTROLLERS "controllers.txt"
#define STATUS_LEN 8
#define UNIT_NAME_LEN 30
#define POSITION_NAME_LEN 30

typedef struct {
	int id, status;
	char unit[UNIT_NAME_LEN], position[POSITION_NAME_LEN];
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