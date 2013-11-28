#include <stdio.h>
#include "controller.h"
#include "danish.h"

void readInputController (int *id, char genstand[], char placering[]) {
	printf("Brug f%slgende format for at tilf%sje en controller:\n"
           "[ID] [GENSTAND] [PLACERING]\n", oe, oe);
    scanf("%d %s; %s", id, genstand, placering);                
}

int removeController(CONTROLLERS controllers[], int index, int len) {
    int i, j;
    
    for (i = 0; i < len - 44; i++) {
       	if (controllers[i].id == index) {
          	len--;
          
          	for (j = i; j < len - 44; j++) {
             	controllers[j] = controllers[j + 1];
             	controllers[j].id--; 
             }
       }
    }
    return i;
}


int readControllers(CONTROLLERS controllers[]) {
	int i = 0;

	FILE *pFile = fopen(FILE_CONTROLLERS, "r");
	if (pFile == NULL) {
		return ERROR_OCCURRED;
	}
   
    for (i = 0; !feof(pFile); i++) {
    	fscanf(pFile, " #%d %[0-9a-zA-Z ]s", &controllers[i].id, controllers[i].unit);
       	fseek(pFile, 2L, SEEK_CUR);
       	fscanf(pFile, " %[0-9a-zA-Z ]s", controllers[i].position);
       	controllers[i].status = 0;
    }
    
	fclose(pFile);

	return i;
}

int changeController(CONTROLLERS controllers[], int cid, int state, int len) {
    int i;
    
    for (i = 0; i <= len; i++) {
      	if (controllers[i].id == cid) {
        	controllers[i].status = state; 
        	break; 
        }
    }
    
    return state;
}



int saveControllers(const CONTROLLERS controllers[], int len) {
    int i;
    
    FILE *pFile = fopen(FILE_CONTROLLERS, "w");
    rewind(pFile);
    
    if (pFile == NULL) {
	  	return ERROR_OCCURRED;
	}
    
    for (i = 0; i < len && controllers[i].id == i + 1; i++) {
      	fprintf(pFile, "#%d\t%s;\t%s",  controllers[i].id, controllers[i].unit, controllers[i].position); 
    }
	return 1;
}

int statusController (const CONTROLLERS controllers[], int cid, int len) {
    int i;
    
    for (i = 0; i <= len; i++) {
      	if (controllers[i].id == cid) {
        	statusControllerPrint(controllers, i);
        	break; 
    	}
    }
    return 1;
}

void statusControllerPrint (const CONTROLLERS controllers[], int i) {

	/* Skal indkodes med danish header for at supportere linux og mac brugere */
   printf("#%d %s - %s: %s", controllers[i].id, controllers[i].unit, controllers[i].position, controllers[i].status == 1 ? "T\x92NDT" : "SLUKKET", AE);
   
}