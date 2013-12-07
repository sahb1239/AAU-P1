#include <stdio.h>
#include <string.h>
#include "scenarie.h"
#include "controller.h"
#include "danish.h"

int strcmpI(const char *string1, const char *string2);

int addScenarie(SCENARIE scenarier[], int len) {
	SCENARIE scenarie;
	
	readInputScenarie(&scenarie.allow_p1, &scenarie.allow_p2, &scenarie.allow_p3, &scenarie.c1_state, &scenarie.c1_id, &scenarie.c2_state, &scenarie.c2_id, &scenarie.c3_state, &scenarie.c3_id, scenarie.desc);
	addScenarieC(scenarier, scenarie, len);
	
	return len + 1;
}

void readInputScenarie (int *pp1, int *pp2, int *pp3, int *pstate1, int *pid1, int *pstate2, int *pid2, int *pstate3, int *pid3, char *command) {
    printf("Skal prioritet 1 have adgang til dette scenarie? 0 = nej, 1 = ja\nIndtast input => ");
    scanf("%d", pp1);
    printf("Skal prioritet 2 have adgang til dette scenarie? 0 = nej, 1 = ja\nIndtast input => ");
    scanf("%d", pp2);
    printf("Skal prioritet 3 have adgang til dette scenarie? 0 = nej, 1 = ja\nIndtast input => ");
    scanf("%d", pp3);
    
    printf("Hvad er controller 1s ID?\n Indtast input => ");
    scanf("%d", pid1);
    printf("Skal denne t%sndes eller slukkes? 0 = slukket, 1 = t%sndt\n Indtast input => ", ae, ae);
    scanf("%d", pstate1);
    printf("Hvad er controller 2s ID?\n Indtast input => ");
    scanf("%d", pid2);
    printf("Skal denne t%sndes eller slukkes? 0 = slukket, 1 = t%sndt\n Indtast input => ", ae, ae);
    scanf("%d", pstate2);
    printf("Hvad er controller 3s ID?\n Indtast input => ");
    scanf("%d", pid3);
    printf("Skal denne t%sndes eller slukkes? 0 = slukket, 1 = t%sndt\n Indtast input => ", ae, ae);
    scanf("%d", pstate3);
    
    printf("Hvad skal kommandoen v%sre?\n Indtast input => ", ae);
    scanf("%s", command);
                      
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

int removeScenarie(SCENARIE scenarier[], int len) {
    int i, j, success = -1;
    char name[50];
    
    removeScenarieInput(name);
    
    for (i = 0; i < len; i++) {
       if (strcmpI(scenarier[i].desc, name) == 0) {
          	len--;
            success = 1;
          
          	for (j = i; j < len; j++) {
             	scenarier[j] = scenarier[j + 1];
             	scenarier[j].num--; 
        	}
       }
    }
    
    if (success != -1) saveScenarier(scenarier, len);
    return success;
}

void removeScenarieInput (char *name) {
    printf("Hvilket scenarie vil du gerne fjerne?\nIndtast input => ");
    scanf("%s", name);
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
      fprintf(pFile, "\n%d   %d   %d   %d   %d #%05d    %d #%05d    %d #%05d %s", scenarier[i].num, scenarier[i].allow_p1, scenarier[i].allow_p2, scenarier[i].allow_p3, 
                                                                                  scenarier[i].c1_state, scenarier[i].c1_id, scenarier[i].c2_state, scenarier[i].c2_id, 
                                                                                  scenarier[i].c3_state, scenarier[i].c3_id, scenarier[i].desc); }
	fclose(pFile);
    
    return 1;
}

void printAllScenarier (const SCENARIE scenarier[], int len) {
    int i;
    
    for (i = 0; i < len; i++) {
       printf("%d   %d   %d   %d   %d #%05d    %d #%05d    %d #%05d %s\n", scenarier[i].num, scenarier[i].allow_p1, scenarier[i].allow_p2, scenarier[i].allow_p3, 
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
    char status[8];
    
    if (scenarie.c1_id == 0) {
       if (scenarie.c1_state)
          sprintf(status, "t%sndt", ae);
       else
    	  strcpy(status, "slukket");
          
       for (i = 0; i < len; i++) {
          res = changeControllerState(controllers, i, scenarie.c1_state, len); }
       printf("Alt er nu %s\n", status);
    }
    
    else {	
       res = changeControllerState(controllers, controllerIDtoIndex(controllers, scenarie.c1_id, len), scenarie.c1_state, len);
       statusControllerPrint(controllers, controllerIDtoIndex(controllers, scenarie.c1_id, len));
       res = changeControllerState(controllers, controllerIDtoIndex(controllers, scenarie.c2_id, len), scenarie.c2_state, len);
       statusControllerPrint(controllers, controllerIDtoIndex(controllers, scenarie.c2_id, len));
       res = changeControllerState(controllers, controllerIDtoIndex(controllers, scenarie.c3_id, len), scenarie.c3_state, len);
       statusControllerPrint(controllers, controllerIDtoIndex(controllers, scenarie.c3_id, len));
       }
    
    return res;
    
}