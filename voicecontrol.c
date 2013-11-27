#include "voicecontrol.h"

/* Skal op i header fil */
int readInput(char *input[]);
int readUsers(FILE *pFile, USERS users[]);
int readControllers(FILE *pFile, CONTROLLERS controllers[]);
int readScenarie(FILE *pFile, SCENARIE scenarier[]);
int addScenarie(FILE *pFile);

int main(int argc, char *argv[]) {
	int i = 0;
	char unitname[] = "jarvis";
	char voiceinput[80];
    
    SCENARIE scenarier[50];
    FILE *pfile_scenarie;
    
    if (argc > 1 && strcmp("--as", argv[1])) {
    	addScenarie(pfile_scenarie);
    	readScenarie(pfile_scenarie, scenarier);  
    }
    
    /*
	initialisering();*/

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

int splitString(const char *input, char *out[], int allocSize) {
	int i = 0, oi = 0, oj = 0, ialloc = allocSize, jalloc = allocSize;
	
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
   /* Test */
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
    
    for (i = 0; i < 3; i++) {
      printf("Scenarie %d: %s\n", scenarier[i].num, scenarier[i].desc); }
      
	fclose(pFile);

	return i;
}

int saveScenarier(FILE *pFile, const SCENARIE scenarier[], int len) {

}

int addScenarie(FILE *pFile) {
    // Tilføjelse af nyt scenarie kræver genstart af programmet
    int i, highID, p1, p2, p3, state1, id1, state2, id2, state3, id3;
    char command[50];
    
    pFile = fopen(FILE_SCENARIE, "a+");
    rewind(pFile);
    
    if (pFile == NULL) {
	  return ERROR_OCCURRED;
	}
    
    printf("Brug f%slgende format for at tilf%sje scenarie:\n"
           "[P1] [P2] [P3] [STATE] [ID] [STATE] [ID] [STATE] [ID] [KOMMANDO];\n", oe, oe); // Tjek om ; er nødvendig
           
    scanf("%d %d %d %d %d %d %d %d %d %[0-9a-zA-Z ]s;", &p1, &p2, &p3, &state1, &id1, &state2, &id2,
                                                       &state3, &id3, &command);
    
    while (!feof(pFile)) {
      fscanf(pFile, " %d %*d %*d %*d %*d #%*d %*d #%*d %*d #%*d %*[0-9a-zA-Z ]s", &highID); }

    fseek(pFile, 0L, SEEK_END);
    fprintf(pFile, "\n%d   %d   %d   %d   %d #%d    %d #%d    %d #%d %s", highID + 1, p1, p2, p3, state1, id1, 
                                                                          state2, id2, state3, id3, command);

}