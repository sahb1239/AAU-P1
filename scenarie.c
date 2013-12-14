#include <stdio.h>
#include <string.h>
#include "scenarie.h"
#include "controller.h"
#include "danish.h"

int strcmpI(const char *string1, const char *string2);

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

void removeScenarieInput (char *name) {
    printf("Hvilket scenarie vil du gerne fjerne?\nIndtast input => ");
    scanf("%s", name);
}

void printAllScenarier (const SCENARIE scenarier[], int len) {
    int i;
    
    for (i = 0; i < len; i++) {
       printf("%d   %d   %d   %d   %d #%05d    %d #%05d    %d #%05d %s\n", scenarier[i].num, scenarier[i].allow_p1, scenarier[i].allow_p2, scenarier[i].allow_p3, 
                                                                           scenarier[i].c1_state, scenarier[i].c1_id, scenarier[i].c2_state, scenarier[i].c2_id, 
                                                                           scenarier[i].c3_state, scenarier[i].c3_id, scenarier[i].desc); }

}

void runScenarie (SCENARIE scenarie, CONTROLLERS controllers[], int len) {
    int i;
    char status[STATUS_LEN];
    
    if (scenarie.c1_id == 0) {
       	if (scenarie.c1_state)
          	sprintf(status, "t%sndt", ae);
       	else
    	  	strcpy(status, "slukket");
          
       	for (i = 0; i < len; i++)
          	changeControllerState(controllers, i, scenarie.c1_state, len); 
       
       	printf("Alt er nu %s\n", status);
    }
    else {
    	changeControllerState(controllers, findControllerFromId(controllers, scenarie.c1_id, len), scenarie.c1_state, len);
       	changeControllerState(controllers, findControllerFromId(controllers, scenarie.c2_id, len), scenarie.c2_state, len);
       	changeControllerState(controllers, findControllerFromId(controllers, scenarie.c3_id, len), scenarie.c3_state, len);
    }
}

int addScenarie(SCENARIE scenarier[], int *len) {
	SCENARIE scenarie;
	
	readInputScenarie(&scenarie.allow_p1, &scenarie.allow_p2, &scenarie.allow_p3, &scenarie.c1_state, &scenarie.c1_id, &scenarie.c2_state, &scenarie.c2_id, &scenarie.c3_state, &scenarie.c3_id, scenarie.desc);
	return addScenarieC(scenarier, scenarie, len);
}

int removeScenarie(SCENARIE scenarier[], int *len) {
	int index;
    char name[COMMAND_LEN];
    
    removeScenarieInput(name);
    
    index = findScenarieFromName(scenarier, name, *len);
    
    if (index != ERROR_OCCURRED) return removeScenarieC(scenarier, index, len);
    else return 0;
}

int findScenarieFromName(const SCENARIE scenarier[], const char name[], int len) {
    int i;
    
    for (i = 0; i < len; i++) {
      if (strcmpI(name, scenarier[i].desc) == 0) {
         return i;
      }
   }
   return ERROR_OCCURRED;
}

int addScenarieC(SCENARIE scenarier[], SCENARIE scenarie, int *len) {
    scenarier[*len].num      = *len + 1;
    scenarier[*len].allow_p1 = scenarie.allow_p1;
    scenarier[*len].allow_p2 = scenarie.allow_p2;
    scenarier[*len].allow_p3 = scenarie.allow_p3;
    scenarier[*len].c1_id    = scenarie.c1_id;
    scenarier[*len].c2_id    = scenarie.c2_id;
    scenarier[*len].c3_id    = scenarie.c3_id;
    scenarier[*len].c1_state = scenarie.c1_state;
    scenarier[*len].c2_state = scenarie.c2_state;
    scenarier[*len].c3_state = scenarie.c3_state;
    strcpy(scenarier[*len].desc, scenarie.desc);
    return saveScenarier(scenarier, ++(*len));
}

int removeScenarieC(SCENARIE scenarier[], int index, int *len) {
	int i;
    
    /* Tæller længden 1 ned */
    (*len)--;
    
    /* Rykker alle efterfølgende elementer (efter index) en plads tilbage */
	for (i = index; i < *len; i++) {
    	scenarier[i] = scenarier[i + 1];
    }
    
    return saveScenarier(scenarier, *len);
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