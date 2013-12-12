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
	int i, scenarie_len, controller_len, users_len;
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
    	} else if (strcmp("--printusers", argv[i]) == 0) {
    		printUsers(users, users_len);
    	}else if(strcmp("--sletbruger", argv[i]) == 0){
    		char inputname[80];
    		printf("Indtast navnet på den som skal slettes:\n");
    		scanf("%s", inputname);
    		deleteUsers(users, users_len, inputname);
    	}else if(strcmp("--opretbruger", argv[i]) == 0){
    		char inputname[80];
    		int inputpriority;
    		printf("Opret Bruger ved følgende syntax [prioritet] [Navn] \n");
    		scanf("%d%s", &inputpriority, inputname);
    		addUsers(users, users_len, inputname, inputpriority);
    	} else if (strcmp("--testfree", argv[i]) == 0) {
    		char *t[10];
    		int m, k = splitString("scenare printer stue printer hyggeaften", t,  10);
    		for (i = 0; i < k; i++) {
    			char *j = correct(t[i], &m);
    			printf("%s\n", j);
    			free(j);
    		}
    		return EXIT_SUCCESS;
    	}
     
	/* TODO: Mangler at define de flest længder på arraysne */
	while(1) {
		printf("Indtast input => ");
		if (scanf(" %[^\n]s", voiceinput)) {
			/* Split vores string (char *) op til et string array (char **) efter ord */
			char *splittedInput[SPLITTED_INPUT_LEN], **selIndex = NULL;
			int numwords = splitString(voiceinput, splittedInput, SPLITTED_INPUT_LEN),
				totalwords = numwords,
				likeness,
				acceptcorrection = 0;
			
			/* Udfør stavekontrol på input - ignorerer output */
			correctInput(splittedInput, &likeness, numwords);
			
			/* Tjekker hvilket index ordet UNIT_NAME ligger i og sætter pointeren selIndex til positionen */
			for (i = 0; i < numwords; i++) {
				if (strcmpI(splittedInput[i], UNIT_NAME) == 0) {
					numwords -= i + 1;
					selIndex = splittedInput + (i + 1) * sizeof(char); /* Sizeof char is 1 */
					break;
				}
			}
			
			if (selIndex != NULL) {
				/* Udfør stavekontrol igen og fortsæt kun denne gang hvis at alle ord kunne findes - vi bruger likeness fra tidligere kørsel af stavekontrollen */
				int tmp;
				if (correctInput(selIndex, &tmp, numwords)) {
					char *voiceInputPtr = voiceinput;
				
					if (!(likeness > PERCENT_UNDERSTOOD_OK)) {
						for (i = 0; i < numwords; i++) {
							strcpy(voiceInputPtr, selIndex[i]);
							
							voiceInputPtr += (strlen(selIndex[i]) * sizeof(char));
							
							/* Tilføj mellemrum */
							voiceInputPtr[0] = ' ';
							
							/* Tæl pointeren op */
							voiceInputPtr += sizeof(char);
						}
						
						/* Afslut med \0 tegnet */
						voiceInputPtr[0] = '\0';
					}
					
					/* Finder ud af om rettelsen skal accepteres */
					acceptcorrection = likeness > PERCENT_UNDERSTOOD_OK || 
										yesno(voiceinput);
				} else
					printf(NOTUNDERSTOOD_TEXT, aa);
					
				#ifdef DEBUG
					printf("DEBUG INFO\n");
					printf("Procent forstået: %d\n", percentUnderstood);
					
					for (i = 0; i < numstrings; i++) {
						printf("Ord: %s\n", selIndex[i]);
					}
				#endif
				
				/* Udfør kommandoen */
				if (acceptcorrection) {
					if (!findAndExecuteCommand(selIndex, numwords, controllers, &controller_len, scenarier, &scenarie_len))
						printf(NOTUNDERSTOOD_TEXT, aa);
				}
			}
			
			/* Free midlertidigt hukommelse */
			for (i = 0; i < totalwords; i++) {
    			free(splittedInput[i]);
    		}
		}
	}
}

int yesno(const char *text) {
	char ans[ANSWER_LEN];
	while (1) {
		printf("Mente du: %s(ja/nej)? => ", text);
		scanf(" %s", ans);
		
		if (strcmpI(ans, "j") || strcmpI(ans, "y") ||
			strcmpI(ans, "ja") || strcmpI(ans, "yes"))
			return 1;
		else if (strcmpI(ans, "n") ||
			strcmpI(ans, "nej") || strcmpI(ans, "no"))
			return 0;
			
		printf("Indtast venligst et gyldigt svar");
	}
}

int correctInput(char **in, int *understoodPercent, int numwords) {
	/* Variabler til at indeholde hvor mange procent af sætningen/ordet at stavekontrollen forstod */
	int understood = 100,
		likeness,
		i;
		
	/* Indeholder det rettede ord */
	char *corrected_word;
	
	for (i = 0; i < numwords; i++) {
		/* Resetter procent forstået af ordet */
		likeness = 0;
		
		/* Udfører stavekontrollen */
		corrected_word = correct(in[i], &likeness);
		
		/* 
		 * Tjekker at der ikke blev fundet et match (pointeren er NULL hvis der ikke blev fundet et). 
		 * Afslutter funktionen med false for at symbolere at inputtet kunne ikke forstås 
		 */
		if (corrected_word == NULL) {
			return 0;
		}
		
		/* Free'er det gamle memory og allokerer nyt */
		free(in[i]);
		in[i] = malloc((1 + strlen(corrected_word)) * sizeof(char)); /* +1 for plads til \0 tegnet */
		
		/* Tjek at der blev allokeret hukommelse */
		checkPTRALLOC((void **) &in[i]);
		
		/* Kopierer output over i in */
		strcpy(in[i], corrected_word);
		
		/* Free'er det midlertidige hukommelse fra funktionen */
		free(corrected_word);
		
		/* Opdaterer graden at sætningen matcher den oprindelige sætning */
		understood -= (100 - likeness) / numwords;
	}
	
	/* Returnerer i hvor høj grad at input matcher med det nye */
	*understoodPercent = understood;
	return 1;
}

/* Splitter en sætning op i enkelte ord */
int splitString(const char *input, char *out[], int maxwords) {
	int i = 0, oi = 0, oj = 0, alloc_size;
	
	while (input[i] != '\0') {
		/* Alloker string */
		out[oi] = malloc(alloc_size = SPLITSTRING_SIZE);
		
		/* Tjek pointeren */
		checkPTRALLOC((void **) &out[oi]);
	
		while (input[i] != ' ' && input[i] != '\0') {
			out[oi][oj++] = input[i];
			
			/* Tjek om der er behov for at gøre arrayet større + 1 fordi der også skal være plads til \0 */
			if (oi + 1 >= alloc_size) {
				out[oi] = realloc(out[oi], alloc_size += SPLITSTRING_SIZE);
				
				/* Tjekker at der kunne allokeres hukommelse */
				checkPTRALLOC((void **) &out[oi]);
			}
			
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
	
	/* Kopirer over i 2 nye strings da de 2 strings bliver ændret (tolower) */
	char *str1 = malloc((str1len + 1) * sizeof(char)), *str2 = malloc((str2len + 1) * sizeof(char));
	
	/* Tjekker at der kunne allokeres hukommelse */
	checkPTRALLOC((void **) &str1);
	checkPTRALLOC((void **) &str2);
	
	/* Kopierer de 2 strings */
	strcpy(str1, string1);
	strcpy(str2, string2);
	
	/* Gør alle bogstaver små */
	for (i = 0; i < str1len; i++)
		str1[i] = tolower(str1[i]);
	for (i = 0; i < str2len; i++)
		str2[i] = tolower(str2[i]);
	
	out = strcmp(str1, str2);
	
	free(str1);
	free(str2);
	
	return out;
}

int findAndExecuteCommand(char *input[], int len, CONTROLLERS controllers[], int *controllersLen, SCENARIE scenarier[], int *scenarierLen) {
	int i, j, numactions;
	
	/* Generer actions */
	ACTIONTYPE type;
	char* controlScenarieTmp[5]; /* Skal denne have symbolsk konstant? */
	char position[POSITION_NAME_LEN];
	
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
        else if (strcmpI(input[i], "tilføjcontroller") == 0 || strcmpI(input[i], "tilfoejcontroller") == 0) {
            type = add_controller;
            numactions++; /* Fix for http://goo.gl/qznNd1 */}
        else if (strcmpI(input[i], "sletcontroller") == 0) {
            type = remove_controller;
            numactions++; }
        else if (strcmpI(input[i], "tilføjscenarie") == 0 || strcmpI(input[i], "tilfoejscenarie") == 0) {
            type = add_scenarie;
            numactions++; }
        else if (strcmpI(input[i], "sletscenarie") == 0) {
            type = remove_scenarie;
            numactions++; }
        else if (strcmpI(input[i], "hjælp") == 0 || strcmpI(input[i], "hjaelp") == 0) {
            type = help;
            numactions++; }
        else if (strcmpI(input[i], "statusalle") == 0) {
            type = status_all;
            numactions++; }
        else if (strcmpI(input[i], "allescenarier") == 0) {
            type = scenarie_all;
            numactions++; }
		
		/* Find controllers */
		for (j = 0; j < *controllersLen; j++) {
			if (strcmpI(input[i], controllers[j].unit) == 0)
				controlScenarieTmp[numactions++] = input[i];
				
			if (strcmpI(input[i], controllers[j].position) == 0)
				strcpy(position, input[i]);
		}
		
		/* Find scenarier */
		for (j = 0; j < *scenarierLen; j++) {
			if (strcmpI(input[i], scenarier[j].desc) == 0)
				controlScenarieTmp[numactions++] = input[i];
		}
	}
	
	/* Udfør kommando */
	int id;
	for (i = 0; i < numactions; i++) {
		switch (type) {
			case turn_on: case turn_off:
				id = findController(controllers, controlScenarieTmp[i], position, *controllersLen);
				
				/* Tjek id om ID'et er gyldigt */
				if (id < 0)
					return 0;
				
				changeControllerState(controllers, id, type == turn_on ? 1 : 0, *controllersLen);
			case status:
				id = findController(controllers, controlScenarieTmp[i], position, *controllersLen);
				
				if (id < 0)
					return 0;
			
				/* Print status */
				statusControllerPrint(controllers, id);
                break;
			case scenarie:
				return runScenarie(scenarier[findScenarie(scenarier, controlScenarieTmp[i], *scenarierLen)], controllers, *controllersLen) != -1;
                break;
            case add_controller:                
                if (addController(controllers, *controllersLen) == -1) return 0;
                printf("Controlleren er tilf%sjet!\n", oe); (*controllersLen)++; break;
            case remove_controller:
                if (removeController(controllers, *controllersLen) == -1) return 0;
                printf("Controlleren er fjernet!\n"); (*controllersLen)--; break;
            case add_scenarie:
                if (addScenarie(scenarier, *scenarierLen) == -1) return 0;
                printf("Scenariet er tilf%sjet!\n", oe); (*scenarierLen)++; break;
            case remove_scenarie:
                if (removeScenarie(scenarier, *scenarierLen) == -1) return 0;
                printf("Scenariet er fjernet!\n"); (*scenarierLen)--; break;
            case help:
                helpMe();
                break;
            case status_all:
                statusControllerPrintAll(controllers, *controllersLen);
                break;
            case scenarie_all:
                printAllScenarier(scenarier, *scenarierLen);
                break;
		}
	}
	
	return numactions != 0;
}

void helpMe(void) {
    system("CLS");
    
    printf("STATUS FOR CONTROLLER\n\"Jarvis status for [genstand] i [position]\"\n\n");
    
    printf("T%sND/SLUK EN CONTROLLER\n\"Jarvis t%snd/sluk for [genstand] i [position]\"\n\n", AE, ae);
    
    printf("K%sR SCENARIE\n\"Jarvis scenarie [scenarienavn]\"\n\n", OE);
    
    printf("TILF%sJ OG SLET CONTROLLER\n\"Jarvis tilfoejcontroller/sletcontroller\" (efterfulgt af en dialog)\n\n", OE);
    
    printf("TILF%sJ OG SLET SCENARIE\n\"Jarvis tilfoejscenarie/sletscenarie\" (efterfulgt af en dialog)\n\n", OE);
    
    printf("Nedenst%sende kommandoer bruges som hj%slp i simulationen\n"
           "-------------------------------------------------------\n\n", aa, ae);
    
    printf("STATUS FOR ALLE CONTROLLERS\n\"Jarvis statusalle\"\n\n");
    
    printf("UDSKRIV ALLE SCENARIER\n\"Jarvis allescenarier\"");
    
    printf("\n\n");
}

void checkPTRALLOC(void **ptr) {
	if (*ptr == NULL) {
		printf(MEMORYERROR_TEXT);
		exit(EXIT_FAILURE);
	}
}