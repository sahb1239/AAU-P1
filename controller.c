#include <stdio.h>
#include "controller.h"

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
       }
    
	fclose(pFile);

	return i;
}