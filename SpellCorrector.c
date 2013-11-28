 /*
  * Dette program laver stavekontrol vha. Edit Distance metoden.
  * Grundlag: http://norvig.com/spell-correct.html Python
  * http://marcelotoledo.com/2007/08/10/how-to-write-a-spelling-corrector/
  */


//#include <danish.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET 29

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
void deletion (const char *input, int len, char **output);
void insert (const char *input, int len, char output[][len + 1]);
void replace (const char *input, int len, char output[][len]);
void switch_word (const char *input, int len, char output[][len + 1]);



int main () {											// omdøbes senere
  int i, len;
  char input[] = "test";         						// fejlordet
  len = strlen(input);
  char **del_output, **ins_output, **rep_output, **swi_output;
  

  
  //del_output = (char **)malloc(len * len * sizeof(char));									//	Størelse: len rækker, len kolonner.
  //ins_output = (char **)malloc(ALPHABET * len + 1 * len + 1 * sizeof(char));				//	Størelse: ALPHABET * len + 1 rækker, len + 2 kolonner.
  //rep_output = (char **)malloc(ALPHABET * len * len * sizeof(char));						//	Størelse: ALPHABET * len rækker, len kolonner.
  //swi_output = (char **)malloc(len - 1 * len * sizeof(char));								//	Størelse: len rækker, len kolonner.
  //deletion(input, len ,del_output);
  
  
  return 0;
}




void string_compare () {										// Sammenligner indtrykkede ord med ord i databasen





}

Edit1 () {												// De 4 edit funktioner kommer herunder







}

void insert (const char *input, int len, char output[][len + 1]) {			//Indsætter et bogstav
  int i, j, k = 0;
  char temp_word[len + 2], temp_alph[2] = "a";
  for (j = 0; j < len; j++) {
	strcpy(temp_word, input);
	// memmove flytter j hen på j + 1. bogstav bliver sat på js plads.	
    memmove( &temp_word[j + 1] , &temp_word[j], strlen(temp_word) - j + 1);
    for (i = 0; i < ALPHABET; i++) {   				
      temp_word[j] = alphabet(i);
	  strcpy(output[k], temp_word);
	  k++;
    }
  }
  for (i = 0; i < ALPHABET; i++) {   				
    strcpy(temp_word, input);
	temp_alph[0] = alphabet(i);
    strcat(temp_word, temp_alph);
 	strcpy(output[k], temp_word);
	k++;
  }
}

void deletion (const char *input, int len, char **output) {											//Sletter et bogstav
  int i;
  char temp_word[len + 1];
  // memmove flytter i + 1 ned på i's plads og sletter derved char i. 
  for (i = 0; i < len; i++) {
	strcpy(temp_word, input);
    memmove( &temp_word[i] , &temp_word[i + 1], strlen(temp_word) - i);
	strcpy(output[i], temp_word);
  }
}

void replace (const char *input, int len, char output[][len]) {											//Erstatter et bogstav med et andet
  int i, j, k = 0;
  char temp_word[len];
  for (j = 0; j < len; j++) {
	strcpy(temp_word, input);
    for (i = 0; i < ALPHABET; i++) {   				
      temp_word[j] = alphabet(i);
	  strcpy(output[k], temp_word);
	  k++;
    }
  }
}

void switch_word (const char *input, int len, char output[][len + 1]) {											// Ombytter to vedsiden af hinanden stående bogstaver
  int i;
  char temp_word[len + 1], temp_char;
  for (i = 0; i < len - 1; i++) {
    strcpy(temp_word, input);
	temp_char = temp_word[i];
	temp_word[i] = temp_word[i + 1];
	temp_word[i + 1] = temp_char;
	strcpy(output[i], temp_word);
  }
}

char alphabet (int i) {
  char alphabet[] = "abcdefghijklmnopqrstuvwxyz\x91\x9B\x86";
  return alphabet[i];
}



















