#define ERROR_OCCURRED -1
#define FILE_SCENARIE "scenarier.txt"
#define COMMAND_LEN 30
#define STATUS_LEN 8

typedef struct {
	int num, allow_p1, allow_p2, allow_p3, c1_id, c2_id, c3_id, c1_state, c2_state, c3_state;
	char desc[COMMAND_LEN];
} SCENARIE;

void readInputScenarie (int *pp1, int *pp2, int *pp3, int *pstate1, int *pid1, int *pstate2, int *pid2, int *pstate3, int *pid3, char *command);
void removeScenarieInput (char *name);
void printAllScenarier (const SCENARIE scenarier[], int len);

int addScenarie(SCENARIE scenarier[], int *len);
int removeScenarie(SCENARIE scenarier[], int *len);

int findScenarieFromName(const SCENARIE scenarier[], const char name[], int len);

int addScenarieC(SCENARIE scenarier[], SCENARIE scenarie, int *len);
int removeScenarieC(SCENARIE scenarier[], int index, int *len);

int readScenarie(SCENARIE scenarier[]);
int saveScenarier(const SCENARIE scenarier[], int len);