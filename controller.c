#include <stdio.h>
#include <string.h>
#include "voicecontrol.h"
#include "danish.h"

void readInputController (int *id, char genstand[], char placering[]) {
	printf("Brug f%slgende format for at tilf%sje en controller:\n"
           "[ID] [GENSTAND] [PLACERING]\n", oe, oe);
    scanf("%d %s; %s", id, genstand, placering);                
}

void statusControllerPrint (const CONTROLLERS controllers[], int i) {
	char status[8];
	
	if (controllers[i].status)
    	sprintf(status, "t%sndt", ae);
	else
    	strcpy(status, "slukket");
	
   	printf("%s i %s er %s\n", controllers[i].unit, controllers[i].position, status);
   
}

void statusControllerPrintAll (const CONTROLLERS controllers[], int len) {
    int i;
    char status[8];
    
    printf("ID     GENSTAND             PLACERING                 STATUS\n\n");
    
    for (i = 0; i < len; i++) {
    	if (controllers[i].status)
    		sprintf(status, "T%sndt", ae);
    	else
    		strcpy(status, "Slukket");
    	
       	printf("#%d %-20s %-25s %s\n", controllers[i].id, controllers[i].unit, controllers[i].position, status); 
    }
}

int addController(CONTROLLERS controllers[], int len) {
	CONTROLLERS controller;
	
	readInputController(&controller.id, controller.unit, controller.position);
	addControllerC(controllers, controller, len);
	
	return len + 1;
}

void addControllerC(CONTROLLERS controllers[], CONTROLLERS controller, int len) {
	controllers[len].id = len + 1;
	controllers[len].status = controller.status;
	strcpy(controllers[len].unit, controller.unit);
	strcpy(controllers[len].position, controller.position);
	saveControllers(controllers, len + 1);
}

int changeControllerState(CONTROLLERS controllers[], int index, int state, int len) {
    controllers[index].status = state; 

    return state;
}

int removeController(CONTROLLERS controllers[], int index, int len) {
    int i, j;
    
    for (i = 0; i < len; i++) {
       	if (controllers[i].id == index) {
          	len--;
          
          	for (j = i; j < len; j++) {
             	controllers[j] = controllers[j + 1];
             	controllers[j].id--; 
             }
       }
    }
    return i;
}

int controllerState (const CONTROLLERS controllers[], int cid, int len) {
    int i;
    
    for (i = 0; i <= len; i++) {
      	if (controllers[i].id == cid) {
        	statusControllerPrint(controllers, i);
        	break; 
    	}
    }
    return 1;
}

int findController(const CONTROLLERS controllers[], const char name[], const char room[], int len) {
   int i;
   
   for (i = 0; i < len; i++) {
      if (strcmpI(room, controllers[i].position) == 0 && strcmpI(name, controllers[i].unit) == 0) {
         return i;
      }
   }
   return ERROR_OCCURRED;
}

int controllerIDtoIndex(const CONTROLLERS controllers[], int cid, int len) {
	int i;
	
	for (i = 0; i < len; i++) {
		if (controllers[i].id == cid)
			return i;
	}
	
	return ERROR_OCCURRED;
}

int readControllers(CONTROLLERS controllers[]) {
	int i = 0;

	FILE *pFile = fopen(FILE_CONTROLLERS, "r");
	if (pFile == NULL)
		return ERROR_OCCURRED;
   
    for (i = 0; !feof(pFile); i++) {
    	fscanf(pFile, " #%d %[0-9a-zA-Z ]s", &controllers[i].id, controllers[i].unit);
       	fseek(pFile, 2L, SEEK_CUR);
       	fscanf(pFile, " %[0-9a-zA-Z ]s", controllers[i].position);
       	controllers[i].status = 0;
    }
    
	fclose(pFile);

	return i;
}

int saveControllers(const CONTROLLERS controllers[], int len) {
    int i;
    
    FILE *pFile = fopen(FILE_CONTROLLERS, "w");
    
    if (pFile == NULL) {
	  	return ERROR_OCCURRED;
	}
    
    for (i = 0; i < len; i++) {
      	fprintf(pFile, "\n#%d\t%s;\t%s",  controllers[i].id, controllers[i].unit, controllers[i].position); 
    }
	
    fclose(pFile);
    
    return 1;
}