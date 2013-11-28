 /*
  * Dette program laver stavekontrol vha. Edit Distance metoden.
  * Grundlag: http://norvig.com/spell-correct.html Python
  * http://marcelotoledo.com/2007/08/10/how-to-write-a-spelling-corrector/
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Implementer en funktion lignende denne */
char *correct(char input[], int *like_percent);

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

void deletion (const char *input, char output[][5]);

int main () {
  int i, len;
  char input[] = "test"; 
  len = strlen(input);
  char output[len][len + 1];
  
  deletion (input, output);
  /*for (i = 0; i < len; i++) {
    printf("%s\n",output[i]);		Det virker!
  }*/
  
  return 0;
}

// Bemærk at input parametre og funktionstyper ikke er sat på endnu




int Spellcontrol() {									// samler lortet





}

string_compare () {										// Sammenligner indtrykkede ord med ord i databasen





}

Edit1 () {												// De 4 edit funktioner kommer herunder







}

insert () {											//Indsætter et ord





}

void deletion (const char *input, char output[][5]) {											//Sletter et ord
  int i;
  char temp_word[strlen(input) + 1];
  // memmove sletter en char på plads i. 
  for (i = 0; i < strlen(input); i++) {
	strcpy(temp_word, input);
    memmove( &temp_word[i] , &temp_word[i + 1], strlen(temp_word) - i);
	strcpy(output[i], temp_word);
  }
}

replace () {											//Erstatter et ord med et andet




}

switch_words () {											// Ombytter to vedsiden af hinanden stående ord





}
