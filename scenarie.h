#define ERROR_OCCURRED -1
#define FILE_SCENARIE "scenarier.txt"

/* Danske karakterer i Windows */
#define ae "\x91"
#define oe "\x9B"
#define aa "\x86"
#define AE "\x92"
#define OE "\x9D"
#define AA "\x8F"

typedef struct {
	int num, allow_p1, allow_p2, allow_p3, c1_id, c2_id, c3_id, c1_state, c2_state, c3_state;
	char desc[80];
} SCENARIE;

/* View */
void addScenarieRW (int *pp1, int *pp2, int *pp3, int *pstate1, int *pid1, int *pstate2, int *pid2, int *pstate3, int *pid3, char command[]);

/* Controller */
int addScenarie(SCENARIE scenarier[], int len);
int removeScenarie(SCENARIE scenarier[], int index, int len);

/* Model */
int readScenarie(SCENARIE scenarier[]);
int saveScenarier(const SCENARIE scenarier[], int len);