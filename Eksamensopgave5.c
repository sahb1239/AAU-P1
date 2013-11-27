/*
 * Eksamesopgave imperiativ programmering
 *
 * Navn:    Søren Aksel Helbo Bjergmark
 * Email:   sbjerg13@student.aau.dk
 * Gruppe:  B2-24
 * Retning: Software
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "superliga-2012-2013"
#define NUM_ROUND 33
#define NUM_TEAM 12
#define NUM_MATCH (NUM_ROUND * NUM_TEAM)
#define TEAM_NAME_LEN 4
#define POINTS_WON 3
#define POINTS_DRAWN 1

#define PRINT_INPUT_FROMHR 12
#define PRINT_INPUT_FROMMINUTE 5
#define PRINT_INPUT_TOHR 13
#define PRINT_INPUT_TOMINUTE 10

typedef struct {
   int year, month, day;
} date;

typedef struct {
   int hour, minute;
} time;

typedef struct {
   int round, homeScore, opponentScore, spectators;
   char homeName[TEAM_NAME_LEN], opponentName[TEAM_NAME_LEN];
   date date;
   time time;
} match;

typedef struct {
   char teamName[TEAM_NAME_LEN];
   int won, won_home, drawn, drawn_home, lost, lost_home, matches, goalsfor, goalsagainst, pnt, homespects;
} team;

typedef struct {
   int round, goals;
} round;

typedef struct {
   time from, to;
   match m;
} timeOfMatchSelect;

/* Prototyper */
void opgave1(match matches[], int len);
void opgave2(round rounds[], int len);
void opgave3(team teams[], int len);
void opgave4(team teams[], int len);
void opgave5(match matches[], int len, int userInput);
void opgave6(team teams[], int len);

void printTeamsAndWinns(const team teams[], int len);
void printMatches(const match matches[], int len);
void printMatchesTime(const match matches[], int len);
void printScoreboard(const team teams[], int len);
void printErrorAndExit(char error[]);

int readFile(match matches[]);
int findTeams(const match matches[], int numMatches, team teams[]);
int findRounds(const match matches[], int numMatches, round rounds[]);
void updateTeam(const match match, team teams[], int *teamslen, int isHomeTeam);
void initTeam(team *t, const char teamName[]);
void addMatchToTeam(team *t, int isHomeMatch, match m);
int findTeamIndex(const team teams[], int numTeams, const char *teamName);
void removeChar(char input[], char removeChar);

round roundMostGoals(round rounds[], int numRounds);
team teamLeastSpectators(team teams[], int numTeams);
int matchesBetweenTimes(int from_hr, int from_min, int to_hr, int to_min, match matches[], int numMatches);
int matchesBetweenTimesT(time from, time to, match matches[], int numMatches);
void sortScoreBoard(team teams[], int numTeams);

int sortRoundGoals(const void *inp1, const void *inp2);
int sortTeamScoreBoard(const void *inp1, const void *inp2);
int sortTeamSpects(const void *inp1, const void *inp2);
int sortMatchGoals(const void *inp1, const void *inp2);
int selectMatch7Goals(const void* inp);
int selectTeamMostAwayWins(const void *inp);
int selectMatchBetweenTimes(const void *inp);
int timeToMinutes(const time t);

int sselect(void *array, int size, int (*cmp)(const void*), int len);
int ssel(char *array, int size, int (*cmp)(const void*), int len);

int main(int argc, char *argv[]) {
   /* 
    * Definerer arrays - har valgt IKKE at bruge malloc til dette
    * Dette giver begrænsninger men gør det til en mere læsevenlig løsning 
    */
   match matches[NUM_MATCH];
   team teams[NUM_TEAM];
   round rounds[NUM_ROUND];
   
   /* Læs fil og indlæs kampe, teams og runder */
   int matchlen = readFile(matches),
       teamslen = findTeams(matches, matchlen, teams),
       roundslen = findRounds(matches, matchlen, rounds);
   
   /* Tjek kommandolinje */
   if (argc == 2 && strcmp(argv[1], "--print") == 0) {
      opgave1(matches, matchlen);
      opgave2(rounds, roundslen);
      opgave3(teams, teamslen);
      opgave4(teams, teamslen);
      opgave5(matches, matchlen, 0);
      opgave6(teams, teamslen);
      
      return EXIT_SUCCESS;
   }
   
   /* Læs fra tekst input */
   int input;

   printf("Indtast hvilken opgave du vil starte \n => ");
   if (scanf("%d", &input)) {
      switch (input) {
         case 1:
            opgave1(matches, matchlen);
            break;
         case 2:
            opgave2(rounds, roundslen);
            break;
         case 3:
            opgave3(teams, teamslen);
            break;
         case 4:
            opgave4(teams, teamslen);
            break;
         case 5:
            opgave5(matches, matchlen, 1);
            break;
         case 6:
            opgave6(teams, teamslen);
            break;
         default:
            printErrorAndExit("opgaven blev ikke fundet");
      }
   }
   
   return EXIT_SUCCESS;
}

/* Opgave funktionerne ændrer i arrayet den får ind og derfor skal det være input / output parameter */
/* For at gøre følgende anderledes kunne man bruge malloc til at lave et nyt array med længden len, og 
 * derefter kopiere alle elementer vha. strncpy over i det nye array - dette kan gøres pga. char fylder
 * 1 byte og man derfor kan overføre byte for byte. Men da dette program sorterer alt efter hvilket
 * resultat der ønskes er dette ikke et behov og ville bare være spild af hukommelse */
void opgave1(match matches[], int len) {
   printf("Opgave 1:\nKampe med 7 eller flere mål:\n");
   printMatches(matches, sselect(matches, sizeof(match), selectMatch7Goals, len));
}

void opgave2(round rounds[], int len) {
   round tmp = roundMostGoals(rounds, len);
   printf("Opgave 2:\nRunden med flest mål:\t%d\nAntal mål:\t\t%d\n", tmp.round, tmp.goals);
}

void opgave3(team teams[], int len) {
   printf("Opgave 3:\nHold der vinder flere kampe på udebane end på hjemmebane\n");
   printTeamsAndWinns(teams, sselect(teams, sizeof(team), selectTeamMostAwayWins, len));
}

void opgave4(team teams[], int len) {
   team tmp = teamLeastSpectators(teams, len);
   printf("Opgave 4\nFærrest tilskuere:\n%s - %d tilskuere\n", tmp.teamName, tmp.homespects);
}

void opgave5(match matches[], int len, int userInput) {
   int from_hr = PRINT_INPUT_FROMHR, from_min = PRINT_INPUT_FROMMINUTE, 
       to_hr = PRINT_INPUT_TOHR, to_min = PRINT_INPUT_TOMINUTE;
   
   printf("Opgave 5\n");
   if (userInput) {
      printf("Indtast startklokkeslettet i formatet xx.xx => ");
      scanf("%d.%d", &from_hr, &from_min);
      printf("Indtast slutklokkeslettet i formatet xx.xx => ");
      scanf("%d.%d", &to_hr, &to_min);
   }
   
   int nummatches = matchesBetweenTimes(from_hr, from_min, to_hr, to_min, matches, len);
   if (nummatches == 0)
      printf("Ingen kampe blev fundet!\n");
   else {
      printf("Kampe i tidsrummet %.2d.%.2d - %.2d.%.2d sorteret efter mest målrige kamp\n", from_hr, from_min, to_hr, to_min);
      printMatchesTime(matches, nummatches);
   }
}

void opgave6(team teams[], int len) {
   printf("Opgave 6\nSamlet score:\n");
   sortScoreBoard(teams, len);
   printScoreboard(teams, len);
}

/* Print funktioner */
void printTeamsAndWinns(const team teams[], int len) {
   int i;
   
   printf("Hold\tV hjem\tV ude\tU hjem\tU ude\tT hjem\tT ude\n");
   for (i = 0; i < len; i++)
      printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\n", teams[i].teamName, teams[i].won_home, teams[i].won - teams[i].won_home, teams[i].drawn_home, teams[i].drawn - teams[i].drawn_home, teams[i].lost_home, teams[i].lost - teams[i].lost_home);
}

void printMatchesTime(const match matches[], int len) {
   int i;
   
   printf("Runde\tHjemme\tUde\tStart\tResultat\n");
   for (i = 0; i < len; i++)
      printf("%d\t%s\t%s\t%2.2d:%2.2d\t%d - %d\n", matches[i].round, matches[i].homeName, matches[i].opponentName, matches[i].time.hour, matches[i].time.minute, matches[i].homeScore, matches[i].opponentScore);
}

void printMatches(const match matches[], int len) {
   int i;
   
   printf("Runde\tHjemme\tUde\tResultat\n");
   for (i = 0; i < len; i++)
      printf("%d\t%s\t%s\t%d - %d\n", matches[i].round, matches[i].homeName, matches[i].opponentName, matches[i].homeScore, matches[i].opponentScore);
}

void printScoreboard(const team teams[], int len) {
   int i;
   
   printf("Nr\tHold\tK\tV\tU\tT\tMf\tMi\t+/-\tP\n");
   for (i = 0; i < len; i++)
      printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, teams[i].teamName, teams[i].matches, teams[i].won, teams[i].drawn, teams[i].lost, teams[i].goalsfor, teams[i].goalsagainst, teams[i].goalsfor - teams[i].goalsagainst, teams[i].pnt);
}

/* Printer til standard error og afslutter programmet */
void printErrorAndExit(char error[]) {
   fprintf(stderr, "\nFejl: %s\n", error);
   exit(EXIT_FAILURE);
}

int readFile(match matches[]) {
   int i = 0; /* Antal elementer læst */
   char spects[10]; /* Midlertidig værdi til tilskuere */
   FILE *file;
   
   /* Åben filen */
   file = fopen(FILE_NAME, "r");
   if (file == NULL) {
      printErrorAndExit("kunne ikke indlæse filen");
   }
   
   /* Indlæs fra filen indtil EOF (End of file) eller i bliver større eller lig med antal kampe */
   /* Denne opsætning er lavet så denne funktion og andre tilsvarende funktioner hurtigt kan laves om til at benytte dynamisk lagerallokering */
   while (NUM_MATCH > i && fscanf(file, "%d %d.%d.%d kl. %d.%d %s - %s %d - %d %s ", &matches[i].round, &matches[i].date.day, &matches[i].date.month, &matches[i].date.year, &matches[i].time.hour, &matches[i].time.minute, matches[i].homeName, matches[i].opponentName, &matches[i].homeScore, &matches[i].opponentScore, spects) != EOF) {
      removeChar(spects, '.');
      sscanf(spects, "%d", &matches[i++].spectators);
   }
   
   /* Luk filen og free resourcer */
   fclose(file);
   
   /* Returner antal elementer læst og arrayet */
   return i;
}

/* Inspiret af http://stackoverflow.com/questions/5457608/c-remove-character-from-string */
void removeChar(char input[], char removeChar) {
   int i, j, len = strlen(input) + 1; /* Arbejd også med 0 tegnet */
   
   for (i = 0; i < len; i++)
      /* Hvis input[i] er lig med den karakter der skal fjernes */
      if (input[i] == removeChar) {
         len--;
         
         /* Ryk alle chars efter input 1 plads tilbage i arrayet, gælder også \0 tegnet (bemærk j + 1) */
         for (j = i; j < len; j++) {
            input[j] = input[j + 1];
         }
         i--;
      }
}

int findTeams(const match matches[], int numMatches, team teams[]) {
   int i, len;
   
   for (i = 0, len = 0; i < numMatches; i++) {
      /* Find informationer for holdene */
      updateTeam(matches[i], teams, &len, 1);
      updateTeam(matches[i], teams, &len, 0);
   }
   
   return len;
}

int findRounds(const match matches[], int numMatches, round rounds[]) {
   int i, len;
   
   for (i = 0, len = 0; i < numMatches; i++) {
      /* Nulstiller runden og sætter runde nr hvis den ikke er blevet tilgået endnu */
      if (matches[i].round != len) {
         len++;
         
         rounds[len - 1].round = len;
         rounds[len - 1].goals = 0;
      }
      
      /* Opdater antal mål i runden */
      rounds[len - 1].goals += matches[i].homeScore + matches[i].opponentScore;
   }
   
   return len;
}

void updateTeam(const match match, team teams[], int *teamslen, int isHomeTeam) {
   /* Find index for teamet ud fra holdnavnet */
   const char *teamName = isHomeTeam ? match.homeName : match.opponentName;
   int teamindex = findTeamIndex(teams, *teamslen, teamName);
   
   /* Initialiserer variablen hvis den ikke er blevet tilgået før */
   if (teamindex >= *teamslen) {
      /* Opdaterer antallet af elementer tilgået */
      /* Pga operatorrækkefølgen skal der parantes uden om dereferencing operationen */
      /* http://people.cs.aau.dk/~normark/c-prog-06/html/notes/intro_themes-op-sec.html#intro_prec-ass-table_tabular_1 */
      /* Der er dog også mulighed for at benytte -> som er netop til struct dereferncing de 2 metoder er ækvialente */
      /* -> er benyttet alle andre steder, men den ækvialente operator er vist her */
      (*teamslen)++;
      
      /* Init team */
      initTeam(&teams[teamindex], teamName);
   }
   
   /* Opdater team variablerne ud fra kampen */
   addMatchToTeam(&teams[teamindex], isHomeTeam, match);
}

/* Finder det første team der matcher med teamName */
int findTeamIndex(const team teams[], int numTeams, const char *teamName) {
   int i;
   for (i = 0; i < numTeams; i++) {
      /* strcmp returnerer 0 hvis de 2 strings er lig hinanden */
      if (strcmp(teams[i].teamName, teamName) == 0)
         return i;
   }
   
   return i;
}

void addMatchToTeam(team *t, int isHomeMatch, match m) {
   t->matches++;

   /* Opdater målforskel */
   int goalsfor = isHomeMatch ? m.homeScore : m.opponentScore;
   int goalsagainst = isHomeMatch ? m.opponentScore : m.homeScore;
   t->goalsfor += goalsfor;
   t->goalsagainst += goalsagainst;
   
   /* Opdater antal point, og antal vundne, uafgjorte og tabte kampe totalt og på hjemmebane */
   if (goalsfor > goalsagainst) {
      if (isHomeMatch) (*t).won_home++;
      t->won++;
      
      t->pnt += POINTS_WON;
   }
   else if (goalsfor < goalsagainst) { 
      if (isHomeMatch) (*t).lost_home++;
      t->lost++;
   }
   else {
      if (isHomeMatch) (*t).drawn_home++;
      t->drawn++;
      
      t->pnt += POINTS_DRAWN;
   }
   
   /* Opdater tilskuere */
   t->homespects += isHomeMatch ? m.spectators : 0;
}

/* Nulstiller et hold så der er garanteret at værdierne point, antal vundne kampe osv. er 0 - derudover kopieres holdnavnet over */
void initTeam(team *t, const char teamName[]) {
   strcpy(t->teamName, teamName);
   t->won = 0;
   t->won_home = 0;
   t->drawn = 0;
   t->drawn_home = 0;
   t->lost = 0;
   t->lost_home = 0;
   t->goalsfor = 0;
   t->goalsagainst = 0;
   t->pnt = 0;
   t->homespects = 0;
   t->matches = 0;
}

/* Problemløsningsfunktioner */
/* Finder runden med flest mål */
round roundMostGoals(round rounds[], int numRounds) {
   qsort(rounds, numRounds, sizeof(round), sortRoundGoals);
   return rounds[0];
}

/* Finder holdet med færrest tilskuere */
team teamLeastSpectators(team teams[], int numTeams) {
   qsort(teams, numTeams, sizeof(team), sortTeamSpects);
   return teams[0];
}

/* Finder alle hold der spille mellem tidspunktet repræsenteret af parametrene */
int matchesBetweenTimes(int from_hr, int from_min, int to_hr, int to_min, match matches[], int numMatches) {
   time from = { from_hr, from_min }, to = { to_hr, to_min };
   return matchesBetweenTimesT(from, to,matches, numMatches);
}

int matchesBetweenTimesT(time from, time to, match matches[], int numMatches) {
   int i;
   
   /* Opret et midlertidigt array til beregningen */
   timeOfMatchSelect *arg = (timeOfMatchSelect*) malloc(numMatches * sizeof(timeOfMatchSelect));
   
   /* Tjek at malloc kunne allokere hukommelse */
   if (arg == NULL) printErrorAndExit("Ikke nok hukommelse kunne allokeres");
   
   /* Opret argumenterne i vores midlertidige hukommelse */
   for (i = 0; i < numMatches; i++) {
      arg[i].from = from;
      arg[i].to = to;
      arg[i].m = matches[i];
   }

   int sel = sselect(arg, sizeof(timeOfMatchSelect), selectMatchBetweenTimes, numMatches);
   
   /* Ligger alle elementerne tilbage */
   for (i = 0; i < numMatches; i++) {
      matches[i] = arg[i].m;
   }
   
   /* Releaser den midlertidige hukommelse igen */
   free(arg);
   
   /* Sorterer elementerne så de mest målrige kampe kommer først */
   qsort(matches, sel, sizeof(match), sortMatchGoals);

   return sel;
}

/* Sorter holdoversigten ud fra point, målforskel, antal mål og holdnavn */
void sortScoreBoard(team teams[], int numTeams) {
   qsort(teams, numTeams, sizeof(team), sortTeamScoreBoard);
}

/* qsort funktioner til sorteringen */
/* Hvis mindre end 0 skal ip1 være før ip2 */
/* Hvis større end 0 skal ip2 være før ip1 */
int sortRoundGoals(const void *inp1, const void *inp2) {
   round *in1 = (round *)inp1, *in2 = (round *)inp2;
   return in2->goals - in1->goals;
}

int sortTeamScoreBoard(const void *inp1, const void *inp2) {
   team *in1 = (team *)inp1, *in2 = (team *)inp2;
   if (in1->pnt != in2->pnt) 
      return in2->pnt - in1->pnt;
   else if (in1->goalsfor - in1->goalsagainst != in2->goalsfor - in2->goalsagainst)
      return (in2->goalsfor - in2->goalsagainst) - (in1->goalsfor - in1->goalsagainst);
   else if (in1->goalsfor != in2->goalsfor) 
      return in2->goalsfor - in1->goalsfor;
   else 
      return strcmp(in1->teamName, in2->teamName);
}

int sortTeamSpects(const void *inp1, const void *inp2) {
   team *in1 = (team *)inp1, *in2 = (team *)inp2;
   return in1->homespects - in2->homespects;
}

int sortMatchGoals(const void *inp1, const void *inp2) {
   match *in1 = (match *)inp1, *in2 = (match *)inp2;
   int in1score = in1->homeScore + in1->opponentScore,
       in2score = in2->homeScore + in2->opponentScore;
   return in2score - in1score;
}

/* select funktioner */
int selectMatch7Goals(const void* inp) {
   match *m = (match *)inp;
   return m->homeScore + m->opponentScore >= 7;
}

int selectTeamMostAwayWins(const void *inp) {
   team *t = (team *)inp;
   return t->won - t->won_home > t->won_home;
}

int selectMatchBetweenTimes(const void *inp) {
   timeOfMatchSelect *m = (timeOfMatchSelect *)inp;
   int k1 = timeToMinutes(m->from), k2 = timeToMinutes(m->to),
       mt = timeToMinutes(m->m.time);
   
   return k1 <= mt && k2 >= mt;
}

/* Timer til minutter */
int timeToMinutes(const time t) {
   return (t.hour * 60) + t.minute;
}

/* 
 * Select funktion(erne)
 * Inspireret af http://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Quicksort#C 
 * Denne funktion er bare en "wrapper" for funktionen ssel, dette fordi at compileren vil 
 * brokke (warnings) sig når man ikke caster pointere (af andre typer end char * over i funktionen ssel) */
int sselect(void *array, int size, int (*cmp)(const void*), int len) {
   return ssel(array, size, cmp, len);
}

int ssel(char *array, int size, int (*cmp)(const void*), int len) {
   int i, j, k;
   char tmp;

   /* Pointere til vores positionen i arrayet */
   /* pntIndexPos er det index der kigges på vha. funktionspointeren */
   /* pntInsertPos er det index i starten af arrayet vi overfører til hvis funktionspointeren returnerer true */
   char *pntIndexPos = array, *pntInsertPos = array;

   for (i = 0, j = 0; i < len; i++) {
      /* Tjek resultatet fra funktionspointeren */
      if (cmp(pntIndexPos)) {
         /* Overfør elementet byte for byte - char har længden en byte */
         for (k = 0; k < size; k++) {
            /* Swap så hele arrayet bevares */
            tmp = *pntInsertPos;
            *pntInsertPos = *pntIndexPos; /* Bemærk forskellen på prefix ++ og suffix ++ */
            *pntIndexPos = tmp;
            
            /* Tæller pointerne 1 op */
            pntInsertPos++; pntIndexPos++;
         }

         j++;
      } else {
         /* Ryk elementet der ses på nu op med størrelsen af elementet */
         pntIndexPos += size;
      }
   }

   return j;
}