#define ALPHABET "abcdefghijklmnopqrstuvwxyz\x91\x9B\x86"
#define FILE_WORDS "ord"
#define DATABASE_SIZE 200
#define DATABASE_SIZE_WORD 30

char *correct (const char input[], int *likeness);
char **database_extract (int *len);

int findInsertLen(const char *input);
int insert (const char *input, char** output);
int findDeletionLen(const char *input);
int deletion (const char *input, char **output);
int findReplaceLen(const char *input);
int replace(const char *input, char **output);
int findTranspotionsLen(const char *input);
int transpose(const char *input, char **output);