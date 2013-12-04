#include <stdio.h>
#include <string.h>
#include "scenarie.h"
#include "controller.h"
#include "danish.h"

int addScenarie(SCENARIE scenarier[], int len) {
	SCENARIE scenarie;
	
	readInputScenarie(&scenarie.allow_p1, &scenarie.allow_p2, &scenarie.allow_p3, &scenarie.c1_state, &scenarie.c1_id, &scenarie.c2_state, &scenarie.c2_id, &scenarie.c3_state, &scenarie.c3_id, scenarie.desc);
	addScenarieC(scenarier, scenarie, len);
	
	return len + 1;
}

void readInputScenarie (int *pp1, int *pp2, int *pp3, int *pstate1, int *pid1, int *pstate2, int *pid2, int *pstate3, int *pid3, char *command) {
       
    printf("Brug f%slgende format for at tilf%sje scenarie:\n"
           "[P1] [P2] [P3] [STATE] [ID] [STATE] [ID] [STATE] [ID] [KOMMANDO]\n", oe, oe);
           
    scanf("%d %d %d %d %d %d %d %d %d %[0-9a-zA-Z ]s", pp1, pp2, pp3, pstate1, pid1, pstate2, pid2,
                                                       pstate3, pid3, command);
                      
}

void addScenarieC(SCENARIE scenarier[], SCENARIE scenarie, int len) {
    scenarier[len].num      = len + 1;
    scenarier[len].allow_p1 = scenarie.allow_p1;
    scenarier[len].allow_p2 = scenarie.allow_p2;
    scenarier[len].allow_p3 = scenarie.allow_p3;
    scenarier[len].c1_id    = scenarie.c1_id;
    scenarier[len].c2_id    = scenarie.c2_id;
    scenarier[len].c3_id    = scenarie.c3_id;
    scenarier[len].c1_state = scenarie.c1_state;
    scenarier[len].c2_state = scenarie.c2_state;
    scenarier[len].c3_state = scenarie.c3_state;
    strcpy(scenarier[len].desc, scenarie.desc);
    saveScenarier(scenarier, len + 1);
}

void removeScenarie(SCENARIE scenarier[], int index, int len) {
    int i, j;
    
    for (i = 0; i < len - 44; i++) {
       if (scenarier[i].num == index) {
          	len--;
          
          	for (j = i; j < len - 44; j++) {
             	scenarier[j] = scenarier[j + 1];
             	scenarier[j].num--; 
        	}
       }
    }
    
    saveScenarier(scenarier, len - 1);
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
    
    if (pFile == NULL)
	  return ERROR_OCCURRED;
    
    for (i = 0; i < len; i++) {
      fprintf(pFile, "\n%d   %d   %d   %d   %d #%d    %d #%d    %d #%d %s", scenarier[i].num, scenarier[i].allow_p1, scenarier[i].allow_p2, scenarier[i].allow_p3, 
                                                                            scenarier[i].c1_state, scenarier[i].c1_id, scenarier[i].c2_state, scenarier[i].c2_id, 
                                                                            scenarier[i].c3_state, scenarier[i].c3_id, scenarier[i].desc); }
	fclose(pFile);
    
    return 1;
}

void printAllScenarier (const SCENARIE scenarier[], int len) {
    int i;
    
    for (i = 0; i < len; i++) {
       printf("%d   %d   %d   %d   %d #%d    %d #%d    %d #%d %s\n", scenarier[i].num, scenarier[i].allow_p1, scenarier[i].allow_p2, scenarier[i].allow_p3, 
                                                                     scenarier[i].c1_state, scenarier[i].c1_id, scenarier[i].c2_state, scenarier[i].c2_id, 
                                                                     scenarier[i].c3_state, scenarier[i].c3_id, scenarier[i].desc); }

}

int findScenarie(const SCENARIE scenarier[], const char name[], int len) {
    int i;
    
    for (i = 0; i < len; i++) {
      if (strcmpI(name, scenarier[i].desc) == 0) {
         return i;
      }
   }
   return ERROR_OCCURRED;
   
}

int runScenarie (SCENARIE scenarie, CONTROLLERS controllers[], int len) {
    int i, res = ERROR_OCCURRED;
    
    if (scenarie.c1_id == 00000) {
       for (i = 0; i < len; i++) {
          res = changeControllerState(controllers, i, scenarie.c1_state, len); }
    }
    
    else {	
       res = changeControllerState(controllers, controllerIDtoIndex(controllers, scenarie.c1_id, len), scenarie.c1_state, len);
       res = changeControllerState(controllers, controllerIDtoIndex(controllers, scenarie.c2_id, len), scenarie.c2_state, len);
       res = changeControllerState(controllers, controllerIDtoIndex(controllers, scenarie.c3_id, len), scenarie.c3_state, len);
       }
    
    return res;
    
}