#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Corrector.h"
#include "voicecontrol.h"

char *correct (const char input[], int *likeness) {
	int num_words, i, j;
	char **db_words = database_extract(&num_words);

	/* Punkt 1: compare af input og database ord. */
  	for (i = 0; i < num_words; i++) {
    	if (strcmpI(input, db_words[i]) == 0) {
	  		/* Rydder op og returnerer */
	  		for (j = 0; j < num_words; j++)
	  			free(db_words[j]);
			free(db_words);
			
			*likeness = 100;
	  		return input;
		}
  	}
  	
  	/* Punkt 2: redigering af input */
  	int totalLen = findInsertLen(input) + findDeletionLen(input) + findReplaceLen(input) + findTranspotionsLen(input);
  	char **combinations = malloc(totalLen * sizeof(char *)), **curptr = combinations; /*  */
  	curptr += insert(input, curptr);
  	curptr += deletion(input, curptr);
  	curptr += replace(input, curptr);
  	curptr += transpose(input, curptr);
  	
  	/* Punkt 3: sammenligning af output fra edit1 med input. Returnering hvis match. */
  	char **matchingWords = malloc(1000 * sizeof(char *));
  	int matchingWordsNum = 0;
  	
  	for (i = 0; i < totalLen; i++) {
  		for (j = 0; j < num_words; j++) {
  	 		if (strcmpI(combinations[i], db_words[j]) == 0) {
  	 			matchingWords[matchingWordsNum] = malloc(strlen(combinations[i]) * sizeof(char));
  	 			strcpy(matchingWords[matchingWordsNum++], combinations[i]);
  			}
  		}
  	}
  	
  	/* Rydder op */
  	for (j = 0; j < num_words; j++)
	  	free(db_words[j]);
	free(db_words);
  	 		
  	for (j = 0; j < totalLen; j++)
  	 	free(combinations[j]);
  	free(combinations);
  	
  	if (matchingWordsNum > 0) {
  		*likeness = 80; /* TODO: KONSTANT */
  		/* Loop resultaterne igennem og tjek at de er lig med hinanden */
  		for (i = 0; i < matchingWordsNum; i++)
  			for (j = 0; j < matchingWordsNum; j++)
  				if (i != j)
  					if (strcmpI(matchingWords[i], matchingWords[j]) != 0)
  						*likeness = 60;
  		
  		return matchingWords[0];
  	}
  	else
  		return NULL;
}

char **database_extract (int *len) {
	int i, alloc_size = DATABASE_SIZE;
	FILE *file = fopen(FILE_WORDS, "r");
	char **out = malloc(alloc_size * sizeof(char *));
	
	for (i = 0; !feof(file); i++) {
		out[i] = malloc(DATABASE_SIZE_WORD * sizeof(char));
		fscanf(file, " %s", out[i]);
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

int insert (const char *input, char** output) {
	int i, j, k;
	for (j = 0, i = 0; j < strlen(ALPHABET); j++) {
		for (k = 0; k < strlen(input) + 1; k++, i++) {
			output[i] = malloc((strlen(input) + 2) * sizeof(char)); /* Plads til 0 tegnet og et ekstra tegn */
			
			/* Kopierer input */
			strcpy(output[i], input);
			
			memmove(&(output[i][k + 1]), &(output[i][k]), strlen(input) - k + 1);
			output[i][k] = ALPHABET[j];
		}
	}
	
	return i;
}

int findDeletionLen(const char *input) {
	int len = strlen(input);
	return len;
}

int deletion (const char *input, char **output) {
	int i;
	for (i = 0; i < strlen(input); i++) {
		output[i] = malloc(strlen(input) * sizeof(char));
		
		/* Kopierer input */
		strcpy(output[i], input);

		memmove(&(output[i][i]), &(output[i][i + 1]), strlen(input) - i + 1);
	}
	
	return i;
}

int findReplaceLen(const char *input) {
	int len = strlen(input), alen = strlen(ALPHABET);
	return len * alen;
}

int replace(const char *input, char **output) {
	int i, j, k;
	for (j = 0, i = 0; j < strlen(ALPHABET); j++) {
		for (k = 0; k < strlen(input); k++, i++) {
			output[i] = malloc((strlen(input) + 1) * sizeof(char)); /* Plads til \0 */
			
			/* Kopierer input */
			strcpy(output[i], input);
			
			output[i][k] = ALPHABET[j];
		}
	}
		
	return i;
}

int findTranspotionsLen(const char *input) {
	int len = strlen(input);
	return len - 1;
}

int transpose(const char *input, char **output) {
	int outLen = findTranspotionsLen(input), i;
	for (i = 0; i < outLen; i++) {
		output[i] = malloc((strlen(input) + 1) * sizeof(char)); /* Plads til \0 */
		
		/* Kopierer input */
		strcpy(output[i], input);
		
		char tmp = output[i][i];
		output[i][i] = output[i][i + 1];
		output[i][i + 1] = tmp;
	}
	
	return i;
}