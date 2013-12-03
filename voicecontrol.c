#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "voicecontrol.h"
#include "rooms.h"
#include "danish.h"
#include "test.h"
#include "users.c"

int runScenarie(SCENARIE scenarie, CONTROLLERS controllers[], int len); /* Skal fjernes */

int main(int argc, char *argv[]) {
	
	
	int i, numstrings,
		scenarie_len, controller_len;
	char voiceinput[INPUT_SIZE];
    SCENARIE scenarier[SCENARIE_SIZE];
    CONTROLLERS controllers[CONTROLLER_SIZE];
    
    /* Tjekker kommandolinje */
    for (i = 1; i < argc; i++)
    	if (strcmp("--test", argv[i]) == 0) {
    		printUsers(USERS users[], int len); /* Skal fjernes */
    		testAll();
    		return EXIT_SUCCESS;
    	}
	
	/* Indlæser scenarier og controllers og gemmer antallet af elemnter læst */
	scenarie_len = readScenarie(scenarier);
    controller_len = readControllers(controllers);
    
	/* TODO: Mangler at define de flest længder på arraysne */
	while(1) {
		printf("Indtast input => ");
		if (scanf(" %[^\n]s", voiceinput)) {
			char *out[80];
			char **ptr = NULL;
			numstrings = splitString(voiceinput, out, 80);
			int len = numstrings;
			
			/* Tjekker hvilket index ordet UNIT_NAME ligger i og flytter pointeren til positionen */
			for (i = 0; i < numstrings; i++) {
				if (strcmp(out[i], UNIT_NAME) == 0) {
					numstrings -= i + 1;
					ptr = out + (i + 1) * sizeof(char); /* Sizeof char is 1 */
					break;
				}
			}
			
			if (ptr != NULL) {
				/* Tjek ord at de er stavet korrekt */
				int percentUnderstood = 100, acceptCorrection;
				for (i = 0; i < numstrings; i++) {
					/* TODO: Stavekontrolsfunktion her */
				}
				
				/* Spørg om kommando skal udføres hvis antal procent var mindre end 80 */
				if (percentUnderstood < 80) {
					printf("Mente du:");
					for (i = 0; i < numstrings; i++) {
						printf(" %s", ptr[i]);
					}
					printf(" (j/n) => ");
					
					char yn;
					scanf(" %c", &yn);
					
					acceptCorrection = yn == 'j' || yn == 'y';
				}
			
				if (percentUnderstood >= 80 || acceptCorrection) {
					if (!findAndExecuteCommand(ptr, numstrings, controllers, controller_len, scenarier, scenarie_len))
						printf(NOTUNDERSTOOD_TEXT);
					else {
						/* TODO: Gem ny output */
					}
					
					/* Print hvis debug er defined */
					#ifdef DEBUG
						for (i = 0; i < numstrings; i++) {
							printf("Ord: %s\n", ptr[i]);
						}
					#endif
				}
			} else printf(NOTUNDERSTOOD_TEXT);
			
			/* Free splitString array */
			for (i = 0; i < len; i++) {
    			free(out[i]);
    		}
		}
	}
}

/* Splitter en sætning op i enkelte ord */
int splitString(const char *input, char *out[], int maxwords) {
	int i = 0, oi = 0, oj = 0, alloc_size;
	
	while (input[i] != '\0') {
		/* Alloker string */
		out[oi] = malloc(alloc_size = SPLITSTRING_SIZE);
	
		while (input[i] != ' ' && input[i] != '\0') {
			out[oi][oj++] = input[i];
			
			/* Tjek om der er behov for at gøre arrayet større + 1 fordi der også skal være plads til \0 */
			if (oi + 1 >= alloc_size)
				out[oi] = realloc(out[oi], alloc_size += SPLITSTRING_SIZE);
			
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

/* Stringcompare case insensitive */
int strcmpI(const char *string1, const char *string2) {
	int out, i, str1len = strlen(string1), str2len = strlen(string2);
	char *str1 = malloc((str1len + 1) * sizeof(char)), *str2 = malloc((str2len + 1) * sizeof(char));
	
	strcpy(str1, string1);
	strcpy(str2, string2);
	
	for (i = 0; i < str1len; i++)
		str1[i] = tolower(str1[i]);
	for (i = 0; i < str2len; i++)
		str2[i] = tolower(str2[i]);
		
	out = strcmp(str1, str2);
	
	free(str1);
	free(str2);
	
	return out;
}

int findAndExecuteCommand(char *input[], int len, CONTROLLERS controllers[], int controllersLen, const SCENARIE scenarier[], int scenarierLen) {
	int i, j, numactions;
	
	/* Generer actions */
	ACTIONTYPE type;
	char* controlScenarieTmp[5];
	char position[20];
	
	/* Find kommando */
	for (i = 0, numactions = 0; i < len; i++) {
		/* Find keyword */
		if (strcmpI(input[i], "tænd") == 0)
			type = turn_on;
		else if (strcmpI(input[i], "sluk") == 0)
			type = turn_off;
		else if (strcmpI(input[i], "status") == 0)
			type = status;
		else if (strcmpI(input[i], "scenarie") == 0)
			type = scenarie;
		
		/* Find controllers */
		for (j = 0; j < controllersLen; j++) {
			if (strcmpI(input[i], controllers[j].unit) == 0)
				controlScenarieTmp[numactions++] = input[i];
				
			if (strcmpI(input[i], controllers[j].position) == 0)
				strcpy(position, input[i]);
		}
		
		/* Find scenarier */
		for (j = 0; j < scenarierLen; j++) {
			if (strcmpI(input[i], scenarier[j].desc) == 0)
				controlScenarieTmp[numactions++] = input[i];
		}
	}
	
	/* Udfør kommando */
	int id;
	for (i = 0; i < numactions; i++) {
		switch (type) {
			case turn_on: case turn_off:
				id = findController(controllers, controlScenarieTmp[i], position, controllersLen);
				
				/* Tjek id om ID'et er gyldigt */
				if (id < 0)
					return 0;
				
				changeControllerState(controllers, id, type == turn_on ? 1 : 0, controllersLen);
			case status:
				id = findController(controllers, controlScenarieTmp[i], position, controllersLen);
				
				if (id < 0)
					return 0;
			
				/* Print status */
				statusControllerPrint(controllers, id);
				break;
			case scenarie:
				break;
		}
	}
	
	return numactions != 0;
}