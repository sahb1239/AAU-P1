#define ERROR_OCCURRED -1
#define FILE_CONTROLLERS "controllers.txt"

typedef struct {
	int id, status;
	char unit[80], position[80];
} CONTROLLERS;

/* Controller */
int addController(CONTROLLERS controllers[], int len); /* NI */
int removeController(CONTROLLERS controllers[], int len); /* NI */

/* Model */
int readControllers(CONTROLLERS controllers[]);
int saveControllers(const CONTROLLERS controllers[], int len); /* NI */

/* View (tror jeg? */
int changeController(CONTROLLERS controllers[], int cid, int state, int len);