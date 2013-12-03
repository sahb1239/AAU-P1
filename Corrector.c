#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET "abcdefghijklmnopqrstuvwxyz\x91\x9B\x86"
#define FILE_WORDS "ord"
#define DATABASE_SIZE 20
#define DATABASE_SIZE_WORD 30

char *correct (char input[]);
char **database_extract (int *len);

int findInsertLen(const char *input);
int insert (const char *input, char** output);
int findDeletionLen(const char *input);
int deletion (const char *input, char **output);
int findReplaceLen(const char *input);
int replace(const char *input, char **output);
int findTranspotionsLen(const char *input);
int transpose(const char *input, char **output);

char alphabet (int i) {
  return ALPHABET[i];
}

int main(void) {
	char *out = correct("hejj");
	printf("%s\n", out);
}

char *correct (char input[]) {
	int num_words, i, j;
	char **db_words = database_extract(&num_words);

	/* Punkt 1: compare af input og database ord. */
  	for (i = 0; i < num_words; i++) {
    	if (strcmp(input, db_words[i]) == 0) {
	  		/* Rydder op og returnerer */
	  		for (j = 0; j < num_words; j++)
	  			free(db_words[j]);
			free(db_words);
			
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
  	for (i = 0; i < totalLen; i++) {
  		for (j = 0; j < num_words; j++) {
  	 		if (strcmp(combinations[i], db_words[j]) == 0) {
  	 			char *out = malloc(strlen(combinations[i]) * sizeof(char));
  	 			strcpy(out, combinations[i]);
  	 			
  	 			/* Rydder op */
  	 			for (j = 0; j < num_words; j++)
	  				free(db_words[j]);
				free(db_words);
  	 		
  	 			for (j = 0; j < totalLen; j++) {
  	 				free(combinations[j]);
  	 			}
  	 			free(combinations);
  	 		
  	 			/* Returnerer */
  				return out;
  			}
  		}
  	}
  	
  	return NULL;
}

char **database_extract (int *len) {
	int i, alloc_size = DATABASE_SIZE;
	FILE *file = fopen(FILE_WORDS, "r");
	char **out = malloc(alloc_size * sizeof(char *));
	
	for (i = 0; !feof(file); i++) {
		out[i] = malloc(DATABASE_SIZE_WORD * sizeof(char));
		fscanf(file, " %s", out[i]);
		
		if (i >= alloc_size)
			out[i] = realloc(out[i], sizeof(char *) * (alloc_size += DATABASE_SIZE));
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
	int outLen = findInsertLen(input), i, j, k;
	for (j = 0, i = 0; j < strlen(ALPHABET); j++) {
		for (k = 0; k < strlen(input) + 1; k++, i++) {
			output[i] = malloc((strlen(input) + 2) * sizeof(char)); /* Plads til 0 tegnet og et ekstra tegn */
			
			/* Kopierer input */
			strcpy(output[i], input);
			
			memmove(&(output[i][k + 1]), &(output[i][k]), strlen(input) - k + 1);
			output[i][k] = alphabet(j);
		}
	}
	
	return i;
}

int findDeletionLen(const char *input) {
	int len = strlen(input);
	return len;
}

int deletion (const char *input, char **output) {
	int outLen = findDeletionLen(input), i;
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
	int outLen = findReplaceLen(input), i, j, k;
	for (j = 0, i = 0; j < strlen(ALPHABET); j++) {
		for (k = 0; k < strlen(input); k++, i++) {
			output[i] = malloc((strlen(input) + 1) * sizeof(char)); /* Plads til \0 */
			
			/* Kopierer input */
			strcpy(output[i], input);
			
			output[i][k] = alphabet(j);
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