#include <stdio.h>
#include <string.h>

int main(void) {
	FILE *file = fopen("dtest", "r");
	
	char chr[10], chr2[10], chr3[] = "æ";
	
	printf("Indtast input => ");
	fscanf(file, "%s", chr);
	scanf("%s", chr2);
	
	printf("Fil: %s\nKonsol: %s\nVariabel: %s\n", chr, chr2, chr3);
	
	if (chr[0] == chr2[0] && chr[0] == chr3[0])
		printf("ER LIG\n");
	else
		printf("ER IKKE LIG\n");
		
	fclose(file);
	
	return 0;
}