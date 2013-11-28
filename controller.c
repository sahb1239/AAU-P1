#include <stdio.h>
#include "controller.h"

void addController (int *id, char *genstand[], int *char[]) {
	printf("Brug f%slgende format for at tilf%sje en controller:\n"
           "[ID] [GENSTAND] [PLACERING]\n", oe, oe);
    scanf("%d %d; %d", id, genstand, placering);                
}

int removeController(SCENARIE controllers[], int index, int len) {
    int i, j;
    
    for (i = 0; i < len - 44; i++) {
       if (controllers[i].num == index) {
          len--;
          
          for (j = i; j < len - 44; j++) {
             controllers[j] = controllers[j + 1];
             controllers[j].num--; }
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
        break; }
    }
    
    return state;
}



int saveControllers(const CONTROLLERS controllers[], int len) {
    int i;
    
    FILE *pFile = fopen(FILE_SCENARIE, "w");
    rewind(pFile);
    
    if (pFile == NULL) {
	  return ERROR_OCCURRED;
	}
    
    for (i = 0; i < len && controllers[i].num == i + 1; i++) {
      fprintf(pFile, "#%d %d; %d",  controllers[i].id, controllers[i].genstand, controllers[i].placering); 
    }
	return 1;
}