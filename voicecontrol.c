#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "voicecontrol.h"
#include "rooms.h"
#include "danish.h"
#include "test.h"
#include "users.h"
#include "Corrector.h"

int runScenarie(SCENARIE scenarie, CONTROLLERS controllers[], int len); /* Skal fjernes */

int main(int argc, char *argv[]) {
	int i, numstrings,
		scenarie_len, controller_len, users_len;
	char voiceinput[INPUT_SIZE];
    SCENARIE scenarier[SCENARIE_SIZE];
    CONTROLLERS controllers[CONTROLLER_SIZE];
    USERS users[USERS_SIZE];
    
    /* Indlæser scenarier og controllers og gemmer antallet af elemnter læst */
	scenarie_len = readScenarie(scenarier);
    controller_len = readControllers(controllers);
    users_len = readUsers(users);
    
    /* Tjekker kommandolinje */
    for (i = 1; i < argc; i++)
    	if (strcmp("--test", argv[i]) == 0) {
    		testAll();
    		return EXIT_SUCCESS;
    	} else if (strcmp("--print", argv[i]) == 0) {
    		printUsers(users, users_len);
    	}else if(strcmp("--print1", argv[i]) == 0){
    		char inputname[80];
    		printf("Indtast navnet på den som skal slettes:\n");
    		scanf("%s", inputname);
    		deleteUsers(users, users_len, inputname);
    	}else if(strcmp("--print2", argv[i]) == 0){
    		char inputname[80];
    		int inputpriority;
    		printf("Opret Bruger ved følgende syntax [prioritet] [Navn] \n");
    		scanf("%d%s", &inputpriority, inputname);
    		addUsers(users, users_len, inputname, inputpriority);
    	} else if (strcmp("--testfree", argv[i]) == 0) {
    		char *out[10]; int len = splitString("Søren tænd printer pa kontoret", out, 10);
			
			printf("START");
			for (i = 0; i < len; i++) {
					int tmp2;
					char *t = correct(out[i], &tmp2);
					if (t != NULL) {
						printf("%s\n", t);
						free(t);
					}
			}

			printf("STEP1");
    		/* Free splitString array */
			for (i = 0; i < len; i++) {
				printf("%s\n", out[i]);
    			free(out[i]);
    		}
    		printf("SUCCES");
    		return EXIT_SUCCESS;
    	}
     
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
				int percentUnderstood = 100, acceptCorrection = 0;
				for (i = 0; i < numstrings; i++) {
					int likeness = 10;
					char *tmp = correct(ptr[i], &likeness);
					if (tmp != NULL) {
						if (strcmpI(tmp, ptr[i]) != 0) {
							percentUnderstood -= (100 - likeness) / numstrings;
							strcpy(ptr[i], tmp);
						}
						free(tmp); /* Free output */
					} else percentUnderstood = -1;
				}
				
				/* Spørg om kommando skal udføres hvis antal procent var mindre end 80 */
				if (percentUnderstood < 80 && percentUnderstood > 0) {
					printf("Mente du:");
					for (i = 0; i < numstrings; i++) {
						printf(" %s", ptr[i]);
					}
					printf(" (j/n) => ");
					
					char yn;
					scanf(" %c", &yn);
					
					acceptCorrection = yn == 'j' || yn == 'y';
				} else if (percentUnderstood <= 0)
					printf(NOTUNDERSTOOD_TEXT);
					
				#ifdef DEBUG
					printf("Procent forstået: %d\n", percentUnderstood);
				#endif
			
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
		if (strcmpI(input[i], "tænd") == 0 || strcmpI(input[i], "taend") == 0)
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
				return runScenarie(scenarier[findScenarie(scenarier, controlScenarieTmp[i], scenarierLen)], controllers, controllersLen) != -1;
		}
	}
	
	return numactions != 0;
}