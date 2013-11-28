#include <stdio.h>
#include <string.h>
#include "scenarie.h"
#include "danish.h"

void addScenarieRW (int *pp1, int *pp2, int *pp3, int *pstate1, int *pid1, int *pstate2, int *pid2, int *pstate3, int *pid3, char command[]) {
       
    printf("Brug f%slgende format for at tilf%sje scenarie:\n"
           "[P1] [P2] [P3] [STATE] [ID] [STATE] [ID] [STATE] [ID] [KOMMANDO]\n", oe, oe);
           
    scanf("%d %d %d %d %d %d %d %d %d %[0-9a-zA-Z ]s", pp1, pp2, pp3, pstate1, pid1, pstate2, pid2,
                                                       pstate3, pid3, command);
                      
}

int addScenarie(SCENARIE scenarier[], int len) {
    int p1, p2, p3, state1, id1, state2, id2, state3, id3;
    char command[50];

    addScenarieRW(&p1, &p2, &p3, &state1, &id1, &state2, &id2, &state3, &id3, command);
    
    scenarier[len].num      = len + 1;
    scenarier[len].allow_p1 = p1;
    scenarier[len].allow_p2 = p2;
    scenarier[len].allow_p3 = p3;
    scenarier[len].c1_id    = id1;
    scenarier[len].c2_id    = id2;
    scenarier[len].c3_id    = id3;
    scenarier[len].c1_state = state1;
    scenarier[len].c2_state = state2;
    scenarier[len].c3_state = state3;
    strcpy(scenarier[len].desc, command);
    
    return 1;

}

int removeScenarie(SCENARIE scenarier[], int index, int len) {
    int i, j;
    
    for (i = 0; i < len - 44; i++) {
       if (scenarier[i].num == index) {
          len--;
          
          for (j = i; j < len - 44; j++) {
             scenarier[j] = scenarier[j + 1];
             scenarier[j].num--; }
       }
    }
    return i;
}

int readScenarie(SCENARIE scenarier[]) {
	int i = 0;
    
    FILE *pFile = fopen(FILE_SCENARIE, "r");

	if (pFile == NULL) {
		return ERROR_OCCURRED;
	}
    
    for (i = 0; !feof(pFile); i++) {
      fscanf(pFile, " %d %d %d %d %d #%d %d #%d %d #%d %[0-9a-zA-Z ]s",  &scenarier[i].num, &scenarier[i].allow_p1, &scenarier[i].allow_p2,
                                                                         &scenarier[i].allow_p3, &scenarier[i].c1_state, &scenarier[i].c1_id, 
                                                                         &scenarier[i].c2_state, &scenarier[i].c2_id, &scenarier[i].c3_state,
                                                                         &scenarier[i].c3_id, scenarier[i].desc);
      
    }
      
	fclose(pFile);

	return i;
}

int saveScenarier(const SCENARIE scenarier[], int len) {
    int i;
    
    FILE *pFile = fopen(FILE_SCENARIE, "w");
    rewind(pFile);
    
    if (pFile == NULL) {
	  return ERROR_OCCURRED;
	}
    
    for (i = 0; i < len && scenarier[i].num == i + 1; i++) {
      fprintf(pFile, "%d   %d   %d   %d   %d #%d    %d #%d    %d #%d %s\n", scenarier[i].num, scenarier[i].allow_p1, scenarier[i].allow_p2, scenarier[i].allow_p3, 
                                                                            scenarier[i].c1_state, scenarier[i].c1_id, scenarier[i].c2_state, scenarier[i].c2_id, 
                                                                            scenarier[i].c3_state, scenarier[i].c3_id, scenarier[i].desc); }
	return 1;
}