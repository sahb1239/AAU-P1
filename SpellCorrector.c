 /*
  * Dette program laver stavekontrol vha. Edit Distance metoden.
  * Grundlag: http://norvig.com/spell-correct.html Python
  * http://marcelotoledo.com/2007/08/10/how-to-write-a-spelling-corrector/
  */


//#include <danish.h>			// denne giver fejl på trods af at danish.h filen er i min mappe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>			// bruges til finde filstørelse til malloc  // ikke længere nødvendig
#define ALPHABET 29
#define DB_WORDS 9		// antal ord i databasen

/* Implementer en funktion lignende denne */
//char *correct(char input[], int *like_percent);

/*
Input/output til programmet:

Indput:			Fejlord
Output: 		korrektionsforslag


Program handlingsrækkefølge:

Hver gang et ord køres igennem stavekontrol skal følgende ske:

1: 	En funktion som sammenligner indtrykkede ord med alle ord i databasen for den del af tilstandsmaskinen.
	Hvis intrykkede ord passer med et ord heri skal stavekontrollen ende. Hvis ikke fortsættes der.
	
2:	Edit1 funktioner udføres. Der kontroleres efter det udføres om fejlordet er blevet til et ord i databasen.
	selvom der kommer en positiv match skal programmet fortsætte med edit1 til alle muligheder er gennem kontroleret.
	Hver der kun er en mulig korrektion skal programmet tage dette uden at spørge brugeren om noget.
	Hvis der er flere mulige korrektioner skal programmet se på hyppigheden af de mulige korrektioner, og foreslå
	det ord som forekommer hyppigst. Bemærk at der skal FORESLÅS, ikke bare udføres, for hvis der er to ord der ligger
	inden for edit1 er de højst sandsynligt så tæt på hindanden at brugeren kunne mene det ene eller det andet.
	Hvis edit1 ikke returnerer et rettet ord:
	
3: 	Edit2 funktioner udføres. Samme som Edit1, bortset fra at de udføres på alle de kombinationer som edit 1 outputtede.
	Hvis der er udslag spørges brugeren om det mest hyppigt brugte rettede ord var det han/hun mente. Hvis ingen udslag udmelder programmet at input ikke forståes
	og nyt input spørges om.
	
	PS: Jeg regner ikke med at der kan komme mere end et muligt retteord ud, da vi har så få ord at arbejde med, så meget af dette
	kommer højst sandsynligt ikke i spil. Jeg dækker bare alle baser.
	


Funktioner som skal være i programmet:	

Compare funktion: 
	Skal sammenligne indtrykkede ord med ordene i valgte database.
	
	input: Indtrykkede ord.
	
	
Edit1: 
	En samling af funktioner som redigerer ordet en gang
	Indsættelse		//Indsætter et bogstav i fejlordet og checker.
	Slettelse		//Sletter et bogstav
	Erstatning		//Erstatter et bogstav med et andet
	Ombytning		//Bytter rundt på to tilstødende bogstaver

	Input: 		Indtrykkede ord.
	Output: 	Ord som er redigeret af ovenstående funktioner 1 gang.

Edit2:			
	Funktioner i Edit1 udføres på output fra Edit1.
	
	indput: 	Ord redigeret af Edit1.
	Output:		Ord redigeret af funktioner i Edit1 endnu en gang.

*/

char alphabet (int i);
char *database_extract (FILE *f_data);
void deletion (const char *input, int len, char ***output);
void insert (const char *input, int len, char ***output);
void replace (const char *input, int len, char ***output);
void switch_letter (const char *input, int len, char ***output);
void edit1 (char *input, int len, char ***word_combinations);
char *correct (char *input);

// der skal gøres noget ved æøå i filen eller programmet

int main () {
  char input[20] = "lyss";				// fejlordet     // ly giver seg fault
  //scanf("%s",input);					// input kommer ind fra andetsteds
  printf("%s", correct(input));
}

// Hvis denne returnerer en null pointer var der ikke en rettelse inden for redigerings afstand to.
char *correct (char *input) {
  int i, j, len = strlen(input), database_len;							
  static char correct_word[20];
  char **letter_combinations;  
  FILE *f_data;
  f_data = fopen("midlertidig_kommando_database.txt","r"); 	
  char data_words[DB_WORDS][20];
  
  // indlæser database ord til data_words.
  for (i = 0; i < DB_WORDS; i++) {
    strcpy(data_words[i], database_extract(f_data));
	//printf("%s ", data_words[i]);
  }
  fclose(f_data);
  // Punkt 1: compare af input og database ord.
  for (i = 0; i < DB_WORDS; i++) {
    if (strcmp(data_words[i], input) == 0) {
	  strcpy(correct_word, data_words[i]);
	  return correct_word;
	}
  }
  // Punkt 2: redigering af input
  edit1 (input, len, &letter_combinations);
  
  // Punkt 3: sammenligning af output fra edit1 med input. Returnering hvis match.
  for (j = 0; j < DB_WORDS; j++) {
    for (i = 0; i < ALPHABET * (len + 1) + len + (ALPHABET * len) + len - 1; i++) {
      if (strcmp(data_words[j], letter_combinations[i]) == 0) return letter_combinations[i];
    }
  }
  return NULL;
}

char *database_extract (FILE *f_data) {
  char *data_words;
  int i;
  data_words = (char *)malloc(20);
  fscanf(f_data, "%s ", data_words);
  return data_words;
}


char *string_compare () {										// Sammenligner indtrykkede ord med ord i databasen





}


void edit1 (char *input, int len, char ***letter_combinations) {												// De 4 edit funktioner kommer herunder
  int i, k = 0;
  char **del_output, **ins_output, **rep_output, **swi_output;
  *letter_combinations = (char **)calloc(10000, sizeof(char));
  for (i = 0; i < ALPHABET * (len + 1) + len + ALPHABET * len + (len - 1); i++) {
    (*letter_combinations)[i] = (char *)calloc((len + 1), sizeof(char));
  }
  
  // behandler input i redigeringsfunktionerne og outputter resultaterne i form af et array af strings.
  insert (input, len, &ins_output);
  deletion (input, len, &del_output);
  replace (input, len, &rep_output);
  switch_letter (input, len, &swi_output);
  
  // kopierer output fra red. funktioner ind i tænkt output af edit1.
  for (i = 0; i < ALPHABET * (len + 1); i++) {   		
	strcpy((*letter_combinations)[k], ins_output[i]); k++;
  }
  for (i = 0; i < len; i++) {
	strcpy((*letter_combinations)[k], del_output[i]); k++;
  }
  for (i = 0; i < ALPHABET * len; i++) {
	strcpy((*letter_combinations)[k], rep_output[i]); k++;
  }
  for (i = 0; i < len - 1; i++) {
	strcpy((*letter_combinations)[k], swi_output[i]); k++;
  }
  /*for (i = 0; i < ALPHABET * (len + 1) + len + (ALPHABET * len) + len - 1; i++) {
    //printf("%s\t", (*letter_combinations)[i]);
  }*/
  
  //befrier plads brugt til redigeringsfunktioner.
  free (del_output);
  free (ins_output);
  free (rep_output);
  free (swi_output);
}

void insert (const char *input, int len, char ***output) {			//Indsætter et bogstav
  int i, j, k = 0;
  char temp_word[len + 2], temp_alph[2] = "a";
  *output = (char **)calloc((ALPHABET * (len + 2)) * (ALPHABET * (len + 1)), sizeof(char));
  for (i = 0; i < ALPHABET * (len + 1); i++) {
    (*output)[i] = (char *)calloc((len + 1), sizeof(char));
  }
  for (j = 0; j < len; j++) {
	strcpy(temp_word, input);
	// memmove flytter j hen på j + 1. bogstav bliver sat på js plads.	
    memmove( &temp_word[j + 1] , &temp_word[j], strlen(temp_word) - j + 1); // j + 1 for \0 skal med
    for (i = 0; i < ALPHABET; i++) {
      temp_word[j] = alphabet(i);
	  strcpy((*output)[k], temp_word);
	  k++;
    }
  }
  for (i = 0; i < ALPHABET; i++) {
    strcpy(temp_word, input);
	temp_alph[0] = alphabet(i);
    strcat(temp_word, temp_alph);
 	strcpy((*output)[k], temp_word);
	k++;
  } 
}

void deletion (const char *input, int len, char ***output) {											//Sletter et bogstav
  int i;
  char temp_word[len + 1];
  *output = (char **)malloc((len + 1) * (len + 1));  // ved input af længde 7 og 8 virker denne linje ikke. mærkeligt mærkeligt.
  for (i = 0; i < len; i++) { 
      //printf("%d\n", i);  
	(*output)[i] = (char *)malloc(len);
    //printf("ss\n");  
  }
  // memmove flytter i + 1 ned på i's plads og sletter derved char i. 
  for (i = 0; i < len; i++) {
	strcpy(temp_word, input);
    memmove( &temp_word[i] , &temp_word[i + 1], strlen(temp_word) - i);
	strcpy((*output)[i], temp_word);
  }
}

void replace (const char *input, int len, char ***output) {											//Erstatter et bogstav med et andet
  int i, j, k = 0;
  char temp_word[len];
  *output = (char **)calloc((ALPHABET * len) * len, sizeof(char));		
  for (i = 0; i < ALPHABET * len; i++) {
    (*output)[i] = (char *)calloc((len), sizeof(char));
  }
  for (j = 0; j < len; j++) {
	strcpy(temp_word, input);
    for (i = 0; i < ALPHABET; i++) {   				
      temp_word[j] = alphabet(i);
	  strcpy((*output)[k], temp_word);
	  k++;
    }
  }
}

void switch_letter (const char *input, int len, char ***output) {											// Ombytter to vedsiden af hinanden stående bogstaver
  int i;
  char temp_word[len + 1], temp_char;
  *output = (char **)calloc((len - 1) * len, sizeof(char));		
  for (i = 0; i < len - 1; i++) {
    (*output)[i] = (char *)calloc((len), sizeof(char));
  }
  for (i = 0; i < len - 1; i++) {
    strcpy(temp_word, input);
	temp_char = temp_word[i];
	temp_word[i] = temp_word[i + 1];
	temp_word[i + 1] = temp_char;
	strcpy((*output)[i], temp_word);
  }
}

char alphabet (int i) {
  char alphabet[] = "abcdefghijklmnopqrstuvwxyz\x91\x9B\x86";
  return alphabet[i];
}







// husk at frigøre mallocs


