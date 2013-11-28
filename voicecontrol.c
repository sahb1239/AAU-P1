#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voicecontrol.h"
#include "scenarie.h"
#include "controller.h"
#include "danish.h"
#include "test.h"

int main(int argc, char *argv[]) {
	int i = 0;
	char unitname[] = "jarvis";
	char voiceinput[80];
    
    SCENARIE scenarier[50];
    int scenarie_len = readScenarie(scenarier); /* læser scenarier og gemmer sidste index */
    
    CONTROLLERS controllers[50];
    int controller_len = readControllers(controllers); /* læser controllers og gemmer sidste index */
    
    for (i = 1; i < argc; i++)
    	if (strcmp("--test", argv[i]) == 0) {
    		testAll();
    		return EXIT_SUCCESS;
    	}
    	else if (strcmp("--as", argv[i]) == 0) {
    		
    	} else if (strcmp("--ss", argv[i]) == 0) {
    		char *input = "jarvis dette her er en test";
    		char *parm[10];
    		
    		int len = splitString(input, parm, 10);
    		
    		for (i = 0; i < len; i++) {
    			printf("%s\n", parm[i]);
    			free(parm[i]);
    		}
    	} else if (strcmp("--print", argv[i]) == 0) {
    		printf("Test\n");
    	}
    
	return EXIT_SUCCESS;

	do {
		fgets(voiceinput, 80, stdin);

		if (voiceinput[strlen(voiceinput) - 1] == '\n') {
			voiceinput[strlen(voiceinput) - 1] = '\0';
		}

		if (strncmp(unitname, voiceinput, 6) == 0) {
			printf("Forst%set!\n", aa);
		} else {
			printf("\nInputtet: %s. Ikke forst%set.\nPr%sv igen: ", voiceinput, aa, oe);
		}

	} while (1);

	return 0;
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