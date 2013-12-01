#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voicecontrol.h"
#include "rooms.h"
#include "danish.h"
#include "test.h"

#define UNIT_NAME "jarvis"

int main(int argc, char *argv[]) {
	int i, numstrings,
		scenarie_len, controller_len;
	char voiceinput[80];
    SCENARIE scenarier[50];
    CONTROLLERS controllers[50];
    
    /* Tjekker kommandolinje */
    for (i = 1; i < argc; i++)
    	if (strcmp("--test", argv[i]) == 0) {
    		testAll();
    	}
    	else if (strcmp("--stringsplit", argv[i]) == 0) {
    		char *input = "jarvis dette her er en test";
    		char *parm[10];
    		
    		int len = splitString(input, parm, 10);
    		
    		for (i = 0; i < len; i++) {
    			printf("%s\n", parm[i]);
    			free(parm[i]);
    		}
    	}
	
	/* Indlæser scenarier og controllers og gemmer antallet af elemnter læst */
	scenarie_len = readScenarie(scenarier);
    controller_len = readControllers(controllers);
    
	/* TODO: Mangler at define de flest længder på arraysne */
	while(1) {
		printf("Indtast input => ");
		if (scanf(" %[^\n]s", voiceinput)) {
			char *out[80];
			char **ptr;
			numstrings = splitString(voiceinput, out, 80);
			int len = numstrings;
			
			/* Tjekker hvilket index ordet UNIT_NAME ligger i */
			for (i = 0; i < numstrings; i++) {
				if (strcmp(out[i], UNIT_NAME) == 0) {
					numstrings -= i + 1;
					ptr = out + i * sizeof(char) + 1; /* Sizeof char is 1 */
					break;
				}
			}
			
			if (ptr != NULL) {
				/* Tjek ord at de er stavet korrekt */
				int percentUnderstood = 100;
				for (i = 0; i < numstrings; i++) {
					/* TODO: Stavekontrolsfunktion her */
				}
				
				if (percentUnderstood < 80) {
					printf("Mente du:");
					for (i = 0; i < numstrings; i++) {
						printf(" %s", ptr[i]);
					}
					printf(" (j/n) => ");
					
					char yn;
					scanf(" %c", &yn);
					if (yn == 'j' || yn == 'y')
						percentUnderstood = 80;
				}
			
				if (percentUnderstood >= 80) {
					for (i = 0; i < numstrings; i++) {
						printf("Ord: %s\n", ptr[i]);
					}
				}
			} else printf("Input blev ikke forstået\n");
			
			/* Free splitString array */
			for (i = 0; i < len; i++) {
    			free(ptr[i]);
    		}
		}
	}
}

/* Splitter en sætning op i enkelte ord */
int splitString(const char *input, char *out[], int maxwords) {
	int i = 0, oi = 0, oj = 0, alloc_size;
	
	while (input[i] != '\0') {
		/* Alloker string */
		out[oi] = malloc(alloc_size = 10); /* TODO: konstant */
	
		while (input[i] != ' ' && input[i] != '\0') {
			out[oi][oj++] = input[i];
			
			/* Tjek om der er behov for at gøre arrayet større + 1 fordi der også skal være plads til \0 */
			if (oi + 1 >= alloc_size)
				out[oi] = realloc(out[oi], alloc_size += 10); /* TODO Konstant */
			
			i++;
		}
		
		/* Afslut string */
		out[oi++][oj] = '\0';
		oj = 0;
		
		if (input[i] != '\0') {
			i++;
			
			if (oi > maxwords)
				return oi - 1;
		}
	}
	
	return oi;
}

int readUsers(FILE *pFile, USERS users[]) {
	int i = 0;

	pFile = fopen(FILE_USERS, "r");
	if (pFile != NULL) {
		return ERROR_OCCURRED;
	}
    
	while (fscanf(pFile, "%s %s %d ", users[i].firstName, users[i].lastName, &users[i].priority) != EOF) {
		i++;
	}
	fclose(pFile);

	return i;
}