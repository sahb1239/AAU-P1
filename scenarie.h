#define ERROR_OCCURRED -1
#define FILE_SCENARIE "scenarier.txt"

typedef struct {
	int num, allow_p1, allow_p2, allow_p3, c1_id, c2_id, c3_id, c1_state, c2_state, c3_state;
	char desc[80];
} SCENARIE;

/* View */
int addScenarie(SCENARIE scenarier[], int len);
void readInputScenarie (int *pp1, int *pp2, int *pp3, int *pstate1, int *pid1, int *pstate2, int *pid2, int *pstate3, int *pid3, char *command);
void printAllScenarier (const SCENARIE scenarier[], int len);

/* Controller */
void addScenarieC(SCENARIE scenarier[], SCENARIE scenarie, int len);
int removeScenarie(SCENARIE scenarier[], int len);
void removeScenarieInput (char *name);
int findScenarie(const SCENARIE scenarier[], const char name[], int len);

/* Model */
int readScenarie(SCENARIE scenarier[]);
int saveScenarier(const SCENARIE scenarier[], int len);