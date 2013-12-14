#include <stdio.h>
#include <string.h>
#include "voicecontrol.h"
#include "danish.h"

void readInputController (char genstand[], char placering[]) {
	printf("Hvad er navnet p%s genstanden?\nIndtast input => ", aa);
    scanf("%s", genstand);
    printf("Hvor er denne placeret?\nIndtast input => ");
    scanf("%s", placering);              
}

void statusControllerPrint (const CONTROLLERS controllers[], int index) {
	char status[STATUS_LEN];
	
	if (controllers[index].status)
    	sprintf(status, "t%sndt", ae);
	else
    	strcpy(status, "slukket");
	
	printf("%s i %s er %s\n", controllers[index].unit, controllers[index].position, status);
}

void statusControllerPrintAll (const CONTROLLERS controllers[], int len) {
    int i;
    char status[STATUS_LEN];
    
    printf("ID     GENSTAND             PLACERING                 STATUS\n\n");
    
    for (i = 0; i < len; i++) {
    	if (controllers[i].status)
    		sprintf(status, "T%sndt", ae);
    	else
    		strcpy(status, "Slukket");
    	
       	printf("#%d %-20s %-25s %s\n", controllers[i].id, controllers[i].unit, controllers[i].position, status); 
    }
}

int addController(CONTROLLERS controllers[], int *len) {
	CONTROLLERS controller;
	
	readInputController(controller.unit, controller.position);
	return addControllerC(controllers, controller, len);
}

int removeController(CONTROLLERS controllers[], int *len) {
    CONTROLLERS controller;
	
	readInputController(controller.unit, controller.position);
	
	int index = findControllerFromName(controllers, controller.unit, controller.position, *len);
	if (index != ERROR_OCCURRED)
		return removeControllerC(controllers, index , len);
	else
		return 0;
}

void changeControllerState(CONTROLLERS controllers[], int index, int state, int len) {
	controllers[index].status = state;
	statusControllerPrint(controllers, index);
	saveControllers(controllers, len);
}

/* Det er da åndsvagt (tror det er mig selv der har lavet det sådan) at len ikke er en pointer... */
/* Derudover burde den returnere output af saveControllers */
int addControllerC(CONTROLLERS controllers[], CONTROLLERS controller, int *len) {
	controllers[*len].id = controllers[*len - 1].id + 1;
	controllers[*len].status = controller.status;
	strcpy(controllers[*len].unit, controller.unit);
	strcpy(controllers[*len].position, controller.position);
	
	return saveControllers(controllers, ++(*len));
}

int removeControllerC(CONTROLLERS controllers[], int index, int *len) {
    int i;
    
    /* Tæller længden 1 ned */
    (*len)--;
    
    /* Rykker alle efterfølgende elementer (efter index) en plads tilbage */
	for (i = index; i < *len; i++) {
    	controllers[i] = controllers[i + 1];
    }
    
    return saveControllers(controllers, *len);
}

int findControllerFromName(const CONTROLLERS controllers[], const char name[], const char room[], int len) {
   int i;
   
   for (i = 0; i < len; i++) {
      if (strcmpI(room, controllers[i].position) == 0 && strcmpI(name, controllers[i].unit) == 0) {
         return i;
      }
   }
   return ERROR_OCCURRED;
}

int findControllerFromId(const CONTROLLERS controllers[], int cid, int len) {
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
		return 0;
   
    for (i = 0; !feof(pFile); i++) {
    	fscanf(pFile, " #%d %[0-9a-zA-Z ]s", &controllers[i].id, controllers[i].unit);
       	fscanf(pFile, "; %[0-9a-zA-Z ]s", controllers[i].position);
       	controllers[i].status = 0;
    }
    
	fclose(pFile);

	return i;
}

int saveControllers(const CONTROLLERS controllers[], int len) {
    int i;
    
    FILE *pFile = fopen(FILE_CONTROLLERS, "w");
    
    if (pFile == NULL) {
	  	return 0;
	}
    
    for (i = 0; i < len; i++) {
      	fprintf(pFile, "\n#%d\t%s;\t%s",  controllers[i].id, controllers[i].unit, controllers[i].position); 
    }
	
    fclose(pFile);
    
    return 1;
}
