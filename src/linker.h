#ifndef LINKER_H
#define LINKER_H

typedef struct usePair{
	char name[10];
	int numUses;
	int uses[10];
} usePair;

int checkForDuplicate(int length, char names[1000][10], char check[10]);

#endif
