#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "voicecontrol.h"
#include "danish.h"
#include "test.h"
#include "Corrector.h"

void runScenarie (SCENARIE scenarie, CONTROLLERS controllers[], int len); /* Skal fjernes */

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
     
	while(1) {
		printf("Indtast input => ");
		if (scanf(" %[^\n]s", voiceinput)) {
			/* Split input string (char *) op til et string array (char **) efter ord */
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
					
					/* Kopirer rettet input hvis lighedsgraden er mindre end lighedsgraden tilladt */
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
					printf("Procent forstået: %d\n", likeness);
					
					for (i = 0; i < numwords; i++) {
						printf("Ord: %s\n", selIndex[i]);
					}
				#endif
				
				/* Udfør kommandoen */
				if (acceptcorrection) {
					if (!findAndExecuteCommand(selIndex, numwords, controllers, &controller_len, scenarier, &scenarie_len, users, &users_len))
						printf(NOTUNDERSTOOD_TEXT, aa);
				}
			} else printf(NOTUNDERSTOOD_TEXT, aa);
			
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
			
		printf("Indtast venligst et gyldigt svar\n");
	}
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

int findAndExecuteCommand(char *input[], int len, CONTROLLERS controllers[], int *controllersLen, SCENARIE scenarier[], int *scenarierLen, USERS users[], int *usersLen) {
	int i, j, numactions;
	
	/* Generer actions */
	ACTIONTYPE type;
	char* controlScenarieTmp[5]; 
	char position[POSITION_NAME_LEN];
	
	/* Find kommando */
	for (i = 0, numactions = 0; i < len; i++) { /* *** Burde denne også splittes op i en bette funktion? *** */
		/* Find keyword */
		if (strcmpI(input[i], "tænd") == 0 || strcmpI(input[i], "taend") == 0)
			type = turn_on;
		else if (strcmpI(input[i], "sluk") == 0)
			type = turn_off;
		else if (strcmpI(input[i], "status") == 0)
			type = status;
		else if (strcmpI(input[i], "scenarie") == 0)
			type = scenarie;
        else if (strcmpI(input[i], "tilføjcontroller") == 0 || strcmpI(input[i], "tilfoejcontroller") == 0)
            type = add_controller;
        else if (strcmpI(input[i], "sletcontroller") == 0)
            type = remove_controller;
        else if (strcmpI(input[i], "tilføjscenarie") == 0 || strcmpI(input[i], "tilfoejscenarie") == 0)
            type = add_scenarie;
        else if (strcmpI(input[i], "sletscenarie") == 0)
            type = remove_scenarie;
        else if (strcmpI(input[i], "hjælp") == 0 || strcmpI(input[i], "hjaelp") == 0)
            type = help;
        else if (strcmpI(input[i], "statusalle") == 0)
            type = status_all;
        else if (strcmpI(input[i], "allescenarier") == 0)
            type = scenarie_all;
        else if (strcmpI(input[i], "tilføjbruger") == 0 || strcmpI(input[i], "tilfoejbruger") == 0)
            type = add_user;
        else if (strcmpI(input[i], "sletbruger") == 0)
            type = remove_user;
		
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
	
	if (numactions > 0)
        return executeNormalCommand(controllers, scenarier, controlScenarieTmp, position, controllersLen, scenarierLen, numactions, type);
	else
        return executeSpecialCommand(controllers, scenarier, users, controlScenarieTmp, position, controllersLen, scenarierLen, usersLen, numactions, type);
	
	/* Ved ingen match fundet */
	return 0;
}

void helpMe(void) { /* Skal opdateres med users */
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    	system("CLS");
    #else
    	system("clear");
    #endif
    
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

int executeNormalCommand (CONTROLLERS controllers[], SCENARIE scenarier[], char *controlScenarieTmp[], char position[], int *controllersLen, int *scenarierLen, int numactions, ACTIONTYPE type) {
	int i, index;
   	for (i = 0; i < numactions; i++) {
        switch (type) {
            case turn_on: case turn_off:
                index = findControllerFromName(controllers, controlScenarieTmp[i], position, *controllersLen);
                
                if (index < 0)
                    return 0;
                             
                changeControllerState(controllers, index, type == turn_on ? 1 : 0, *controllersLen);
                break;
            case status:
                index = findControllerFromName(controllers, controlScenarieTmp[i], position, *controllersLen);
                                
                if (index < 0)
                    return 0;
                        
                /* Print status */
                statusControllerPrint(controllers, index);
                break;
            case scenarie:
            	index = findScenarieFromName(scenarier, controlScenarieTmp[i], *scenarierLen);
            	
            	if (index < 0)
            		return 0;
            
                runScenarie(scenarier[index], controllers, *controllersLen);
                break;
            default:
                /* Fejl - burde være endt nede i anden gruppe */
                return 0;
            }
    }
        
    return numactions;
}

int executeSpecialCommand (CONTROLLERS controllers[], SCENARIE scenarier[], USERS users[], char *controlScenarieTmp[], char position[], int *controllersLen, int *scenarierLen, int *usersLen, int numactions, ACTIONTYPE type) {
   switch (type) {
      case add_controller:                
         if (!addController(controllers, controllersLen)) 
         	return 0;
         	
         printf("Controlleren er tilf%sjet!\n", oe);
         return 1;
      case remove_controller:
         if (!removeController(controllers, controllersLen)) 
         	return 0;
         	
         printf("Controlleren er fjernet!\n");
         return 1;
      case add_scenarie:
         if (!addScenarie(scenarier, scenarierLen)) 
         	return 0;
         	
         printf("Scenariet er tilf%sjet!\n", oe);
         return 1;
      case remove_scenarie:
         if (!removeScenarie(scenarier, scenarierLen)) 
         	return 0;
         	
         printf("Scenariet er fjernet!\n"); 
         return 1;
      case add_user:
         if (addUser(users, *usersLen) == -1) return 0;
         printf("Brugeren er nu tilf%sjet!\n", oe); (*usersLen)++;
         return 1;
      case remove_user:
         if (removeUser(users, *usersLen) == -1) return 0;
         printf("Brugeren er nu slettet!\n"); (*usersLen)--;
         return 1;
      case help:
         helpMe();
         return 1;
      case status_all:
         statusControllerPrintAll(controllers, *controllersLen);
         return 1;
      case scenarie_all:
          printAllScenarier(scenarier, *scenarierLen);
         return 1;
      default:
         /* Fejl - burde være endt i anden gruppe */
         return 0;
        }
}
