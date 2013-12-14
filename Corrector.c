#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Corrector.h"
#include "voicecontrol.h"

/* Hjælpefunktion til lighedsgrad ud fra et string array */
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

char *correct (const char input[], int *likeness) {
	int num_words, i, j;
	char **db_words = database_extract(&num_words);

	/* Punkt 1: compare af input og database ord. */
  	for (i = 0; i < num_words; i++) {
    	if (strcmp(input, db_words[i]) == 0) {
	  		/* Rydder op og returnerer */
	  		for (j = 0; j < num_words; j++)
	  			free(db_words[j]);
			free(db_words);
			
			*likeness = 100;
	  		
	  		char *tmp = malloc((strlen(input) + 1) * sizeof(char));
	  		/* Tjek pointeren */
	  		checkPTRALLOC((void **) &tmp);
	  		
  			strcpy(tmp, input);
  			return tmp;
		}
  	}
  	
  	/* Punkt 2: redigering af input */
  	int totalLen = findInsertLen(input) + findDeletionLen(input) + findReplaceLen(input) + findTranspotionsLen(input);
  	char **combinations = malloc(totalLen * sizeof(char *));
	int startIndex = 0;
	
	/* Tjek pointeren */
	checkPTRALLOC((void **) &combinations);
	
  	startIndex += insert(input, combinations, startIndex);
  	startIndex += deletion(input, combinations, startIndex);
  	startIndex += replace(input, combinations, startIndex);
  	startIndex += transpose(input, combinations, startIndex);
  	
  	/* Punkt 3: sammenligning af output fra edit1 med input. Tjek om der er flere match og returner det første element. */
  	int matchingWordsNum = 0;
  	char *matchingWords[20];
  	for (i = 0; i < totalLen; i++) {
  		for (j = 0; j < num_words; j++) {
  	 		if (strcmpI(combinations[i], db_words[j]) == 0) {
  	 			matchingWords[matchingWordsNum] = malloc(strlen(combinations[i]) * sizeof(char));
  	 			
  	 			/* Tjek pointeren */
  	 			checkPTRALLOC((void **) &matchingWords[matchingWordsNum]);
  	 			
  	 			strcpy(matchingWords[matchingWordsNum++], combinations[i]);
  			}
  		}
  	}
  	
  	if (matchingWordsNum > 0) {
  		*likeness = 60;
  		
  		/* Tjek om der er flere der er lig med hinanden */
  		for (i = 0; i < matchingWordsNum; i++)
  			for (j = 0; j < matchingWordsNum; j++)
  				if (i != j && strcmpI(matchingWords[i], matchingWords[j]) != 0)
  					*likeness = 30;
  	
  		char *tmp = malloc((strlen(matchingWords[0]) + 1) * sizeof(char));
  		
  		/* Tjek pointeren */
  		checkPTRALLOC((void **) &tmp);
  		
  		strcpy(tmp, matchingWords[0]);
  		
  		for (j = 0; j < num_words; j++)
			free(db_words[j]);
		free(db_words);
  	 	
  		for (j = 0; j < totalLen; j++)
  			free(combinations[j]);
  		free(combinations);
  		
  		return tmp;
  	}
  	
  	/* Punkt 4: Redigering af input til redigeringsafstand 2. */
  	/* Find total mængde der skal allokeres af plads */
  	int totalLen2 = 0;
  	for (j = 0; j < totalLen; j++) {
  		totalLen2 += findInsertLen(combinations[j]) + findDeletionLen(combinations[j]) + 
  					  findReplaceLen(combinations[j]) + findTranspotionsLen(combinations[j]);
  	}
  	
  	char **combinations2 = malloc(totalLen2 * sizeof(char *));
  	/* Tjek pointeren */
  	checkPTRALLOC((void **) &combinations2);
  	
	int startIndex2 = 0;
  	for (i = 0; i < totalLen; i++) {
  		startIndex2 += insert(combinations[i], combinations2, startIndex2);
  		startIndex2 += deletion(combinations[i], combinations2, startIndex2);
  		startIndex2 += replace(combinations[i], combinations2, startIndex2);
  		startIndex2 += transpose(combinations[i], combinations2, startIndex2);

		/* Fjerner gammel memory */
		free(combinations[i]);
  	}
	free(combinations);
  	
  	/* Punkt 5: sammenligning af output fra edit2 med input. Returnering hvis match. */
  	for (i = 0; i < totalLen2; i++) {
  		for (j = 0; j < num_words; j++) {
  	 		if (strcmp(combinations2[i], db_words[j]) == 0) {
  	 			char *out = malloc((strlen(combinations2[i]) + 1) * sizeof(char));
  	 			
  	 			/* Tjek pointeren */
  	 			checkPTRALLOC((void **) &out);
  	 			
  	 			strcpy(out, combinations2[i]);
  	 			
  	 			/* Rydder op */
  	 			for (j = 0; j < num_words; j++)
	  				free(db_words[j]);
				free(db_words);
  	 			
  	 			for (j = 0; j < totalLen2; j++)
  	 				free(combinations2[j]);
  	 			free(combinations2);
  	 		
  	 			*likeness = 20;
  	 			/* Returnerer */
  				return out;
  			}
  		}
  	}
  	
  	/* Rydder op */
  	for (j = 0; j < num_words; j++)
		free(db_words[j]);
	free(db_words);
  	 			
  	for (j = 0; j < totalLen2; j++)
  	 	free(combinations2[j]);
  	free(combinations2);
  	
  	/* Punkt 6: Hvis enden nås uden at en korrektion findes returneres en null pointer. */
  	return NULL;
}

char **database_extract (int *len) {
	int i, alloc_size = DATABASE_SIZE;
	FILE *file = fopen(FILE_WORDS, "r");
	char **out = malloc(alloc_size * sizeof(char *));
	
	/* Tjek pointeren */
	checkPTRALLOC((void **) &out);
	
	for (i = 0; !feof(file); i++) {
		out[i] = malloc(DATABASE_SIZE_WORD * sizeof(char));
		fscanf(file, " %s", out[i]);
		
		if (i >= alloc_size) {
			out[i] = realloc(out[i], sizeof(char *) * (alloc_size += DATABASE_SIZE));
			
			/* Tjek pointeren */
			checkPTRALLOC((void **) &out[i]);	
		}
	}
	
	fclose(file);
	
	/* Output */
	*len = i;
	return out;
}

int findInsertLen(const char *input) {
	int len = strlen(input), alen = strlen(ALPHABET);
	return (len + 1) * alen;
}

int insert (const char *input, char** output, int startIndex) {
	int i, j, k, l;
	for (j = 0, i = startIndex, l = 0; j < strlen(ALPHABET); j++) {
		for (k = 0; k < strlen(input) + 1; k++, i++, l++) {
			output[i] = (char *)malloc((strlen(input) + 2) * sizeof(char)); /* Plads til 0 tegnet og et ekstra tegn */
			
			/* Tjek pointeren */
			checkPTRALLOC((void **)&output[i]);
			
			/* Kopierer input */
			strcpy(output[i], input);
			
			/* Rykker memory og indsætter bogstav */
			memmove(output[i] + (k + 1) * sizeof(char), output[i] + k * sizeof(char), strlen(input) - k);
			output[i][k] = ALPHABET[j];
			
			/* Indsætter \0 tegnet */
			output[i][strlen(input) + 1] = '\0';
		}
	}
	
	return l;
}

int findDeletionLen(const char *input) {
	int len = strlen(input);
	return len;
}

int deletion (const char *input, char **output, int startIndex) {
	int i, j;
	for (i = startIndex, j = 0; j < strlen(input); i++, j++) {
		output[i] = (char *)malloc(strlen(input) * sizeof(char));
		/* Tjek pointeren */
		checkPTRALLOC((void **)&output[i]);
		
		/* Kopierer input (uden 0 tegnet) */
		strncpy(output[i], input, strlen(input));
		
		memmove(output[i] + (j * sizeof(char)), output[i] + ((j + 1) * sizeof(char)), strlen(input) - j - 1);
		
		/* Indsætter 0 tegnet */
		output[i][strlen(input) - 1] = '\0';
	}
	
	return j;
}

int findReplaceLen(const char *input) {
	int len = strlen(input), alen = strlen(ALPHABET);
	return len * alen;
}

int replace(const char *input, char **output, int startIndex) {
	int i, j, k, l;
	for (j = 0, i = startIndex, l = 0; j < strlen(ALPHABET); j++) {
		for (k = 0; k < strlen(input); k++, i++, l++) {
			output[i] = (char *)malloc((strlen(input) + 1) * sizeof(char)); /* Plads til \0 */
			/* Tjek pointeren */
			checkPTRALLOC((void **) &output[i]);
			
			/* Kopierer input */
			strcpy(output[i], input);
			
			output[i][k] = ALPHABET[j];
		}
	}
		
	return l;
}

int findTranspotionsLen(const char *input) {
	return strlen(input) - 1;
}

int transpose(const char *input, char **output, int startIndex) {
	int outLen = findTranspotionsLen(input), i, j;
	for (i = startIndex, j = 0; j < outLen; i++, j++) {
		output[i] = (char *)malloc((strlen(input) + 1) * sizeof(char)); /* Plads til \0 */
		
		/* Tjek pointeren */
		checkPTRALLOC((void **) &output[i]);
		
		/* Kopierer input */
		strcpy(output[i], input);
		
		char tmp = output[i][j];
		output[i][j] = output[i][j + 1];
		output[i][j + 1] = tmp;
	}
	
	return j;
}