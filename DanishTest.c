#include <stdio.h>
#include <string.h>

int main(void) {
	FILE *file = fopen("dtest", "r");
	
	char chr[10], chr2[10];
	
	printf("Indtast input => ");
	fscanf(file, "%s", chr);
	scanf("%s", chr2);
	
	printf("Fil: %s\nKonsol: %s\n", chr, chr2);
	
	if (chr[0] == chr2[0])
		printf("ER LIG\n");
	else
		printf("ER IKKE LIG\n");
		
	fclose(file);
	
	return 0;
}