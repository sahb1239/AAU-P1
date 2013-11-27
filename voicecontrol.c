#include "voicecontrol.h"

/* Skal op i header fil */
int readInput(char *input[]);
int readUsers(FILE *pFile, USERS users[]);
int readControllers(FILE *pFile, CONTROLLERS controllers[]);
int readScenarie(FILE *pFile, SCENARIE scenarier[]);

int main(int argc, char *argv[]) {
	int i = 0;
	char unitname[] = "jarvis";
	char voiceinput[80];
    
    SCENARIE scenarier[50];
    FILE *pfile_scenarie;
    int len_scenarier = sizeof(scenarier) / sizeof(scenarier[0]);
    int scenarie_last_index = readScenarie(pfile_scenarie, scenarier); // læser scenarier og gemmer sidste index
    
    if (argc > 1 && strcmp("--as", argv[1])) {
    	/*addScenarie(pfile_scenarie);
    	readScenarie(pfile_scenarie, scenarier);  */
    }
    
    
	return EXIT_SUCCESS;

	do {
		fgets(voiceinput, 80, stdin);

		if (voiceinput[strlen(voiceinput) - 1] == '\n') {
			voiceinput[strlen(voiceinput) - 1] = '\0';
		}

		if (strncmp(unitname, voiceinput, 6) == 0) {
			printf("Forst%set!\n", aa);
		} else {
			printf("\nInputtet: %s. Ikke forst%set.\nPr%sv igen: ", voiceinput, aa, oe);
		}

	} while (1);

	return 0;
}

/* Splitter en sætning op i enkelte ord */
int splitString(const char *input, char *out[], int allocSize) {
	int i = 0, oi = 0, oj = 0, ialloc = allocSize, jalloc = allocSize;
	
	out = malloc(jalloc = allocSize);
	
	while (input[i] != '\0') {
		/* Alloker string */
		out[oi] = malloc(ialloc = allocSize);
	
		while (input[i] != ' ' && input[i] != '\0') {
			out[oi][oj++] = input[i];
			
			i++;
		}
		
		/* Afslut string */
		out[oi++][oj] = '\0';
		oj = 0;
		
		if (input[i] != '\0') {
			i++;
		}
	}
	
	return oi;
}

int readUsers(FILE *pFile, USERS users[]) {
	int i = 0;

	pFile = fopen(FILE_USERS, "r");
	if (pFile != NULL) {
		return ERROR_OCCURRED;
	}
    
	while (fscanf(pFile, "%s %s %d ", users[i].firstName, users[i].lastName, &users[i].priority) != EOF) {
		i++;
	}
	fclose(pFile);

	return i;
}

int readControllers(FILE *pFile, CONTROLLERS controllers[]) {
	int i = 0;

	pFile = fopen(FILE_CONTROLLERS, "r");
	if (pFile == NULL) {
		return ERROR_OCCURRED;
	}

	while (fscanf(pFile, "#%d %10[0-9a-zA-Z ]s %10[0-9a-zA-Z ]s ", &controllers[i].id, controllers[i].unit, controllers[i].position) != EOF) {
		i++;
	}
	fclose(pFile);

	return i;
}

int readScenarie(FILE *pFile, SCENARIE scenarier[]) {
	int i = 0;
    
    pFile = fopen(FILE_SCENARIE, "r");

	if (pFile == NULL) {
		return ERROR_OCCURRED;
	}
    
    for (i = 0; !feof(pFile); i++) {
      fscanf(pFile, " %d %d %d %d %d #%d %d #%d %d #%d %[0-9a-zA-Z ]s",  &scenarier[i].num, &scenarier[i].allow_p1, &scenarier[i].allow_p2,
                                                                         &scenarier[i].allow_p3, &scenarier[i].c1_state, &scenarier[i].c1_id, 
                                                                         &scenarier[i].c2_state, &scenarier[i].c2_id, &scenarier[i].c3_state,
                                                                         &scenarier[i].c3_id, &scenarier[i].desc);
      
    }
      
	fclose(pFile);

	return i;
}

int saveScenarier(FILE *pFile, const SCENARIE scenarier[], int len) {
    int i;
    
    pFile = fopen(FILE_SCENARIE, "w");
    rewind(pFile);
    
    if (pFile == NULL) {
	  return ERROR_OCCURRED;
	}
    
    for (i = 0; i < len && scenarier[i].num == i + 1; i++) {
      fprintf(pFile, "%d   %d   %d   %d   %d #%d    %d #%d    %d #%d %s\n", scenarier[i].num, scenarier[i].allow_p1, scenarier[i].allow_p2, scenarier[i].allow_p3, 
                                                                            scenarier[i].c1_state, scenarier[i].c1_id, scenarier[i].c2_state, scenarier[i].c2_id, 
                                                                            scenarier[i].c3_state, scenarier[i].c3_id, scenarier[i].desc); }

}

int addScenarie(SCENARIE scenarier[], const int last_index) {

    int i, p1, p2, p3, state1, id1, state2, id2, state3, id3;
    char command[50];

    addScenarieRW(&p1, &p2, &p3, &state1, &id1, &state2, &id2, &state3, &id3, command);
    
    scenarier[last_index].num      = last_index + 1;
    scenarier[last_index].allow_p1 = p1;
    scenarier[last_index].allow_p2 = p2;
    scenarier[last_index].allow_p3 = p3;
    scenarier[last_index].c1_id    = id1;
    scenarier[last_index].c2_id    = id2;
    scenarier[last_index].c3_id    = id3;
    scenarier[last_index].c1_state = state1;
    scenarier[last_index].c2_state = state2;
    scenarier[last_index].c3_state = state3;
    strcpy(scenarier[last_index].desc, command);

}

void addScenarieRW (int *pp1, int *pp2, int *pp3, int *pstate1, int *pid1, int *pstate2, int *pid2, int *pstate3, int *pid3, char command[]) {
       
    printf("Brug f%slgende format for at tilf%sje scenarie:\n"
           "[P1] [P2] [P3] [STATE] [ID] [STATE] [ID] [STATE] [ID] [KOMMANDO]\n", oe, oe);
           
    scanf("%d %d %d %d %d %d %d %d %d %[0-9a-zA-Z ]s", pp1, pp2, pp3, pstate1, pid1, pstate2, pid2,
                                                       pstate3, pid3, command);
                      
}

//int removeScenarie (SCENARIE scenarier[], int id) {