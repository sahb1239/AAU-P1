#define ERROR_OCCURRED -1
#define FILE_CONTROLLERS "controllers.txt"

typedef struct {
	int id, status;
	char unit[80], position[80];
} CONTROLLERS;

/* View */
void readInputController (int *id, char genstand[], char placering[]);
void statusControllerPrint (const CONTROLLERS controllers[], int i);

/* Controller */
int addControllerC(CONTROLLERS controllers[], int len); /* NI */
int removeController(CONTROLLERS controllers[], int index, int len);
int changeController(CONTROLLERS controllers[], int cid, int state, int len);
int statusController (const CONTROLLERS controllers[], int cid, int len);

/* Model */
int readControllers(CONTROLLERS controllers[]);
int saveControllers(const CONTROLLERS controllers[], int len); /* NI */