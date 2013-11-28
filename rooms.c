#include <stdio.h>
#include "rooms.h"
#include "danish.h"

int readRooms (HOUSE rooms[]) {
    int i;
    
	FILE *pFile = fopen(FILE_ROOMS, "r");
	if (pFile == NULL) {
		return ERROR_OCCURRED;
	}
    
    for (i = 0; !feof(pFile); i++) {
      fscanf(pFile, " %[0-9a-zA-Z ]s ", &rooms[i]); }

    return i;
}