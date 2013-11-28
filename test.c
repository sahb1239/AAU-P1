#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "voicecontrol.h"
#include "test.h"

void testAll() {
	printf("Starter tests\n");
	
	printf("Udfører test 1...\n");
	testSplitString1();
}

void testSplitString1() {
	char *input = "jarvis test langtord";
	
	char *expected[] = {"jarvis", "test", "langtord"};
	char *actual[10];
	
	int len = splitString(input, actual, 10), i;
	
	assert(len == 3);
	for (i = 0; i < len; i++) {
		assert(strcmp(actual[i], expected[i]) == 0);
		free(actual[i]);
	}
}