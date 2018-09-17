#include <stdio.h>
#include <stdlib.h>
#include "linker.h"

//two pass linker
//Ben Haderle
//9 17 2018

definition * symbolTable = 0;

int addToBack(definition ** headPointer, definition * toAdd){
	if(*headPointer == 0){
		* headPointer = toAdd;
		return 1;
	}
	else{
		definition * last = * headPointer;
		while(last->next != NULL){
			last = last->next;
		}
		last->next = toAdd;
		return 1;
	}
}

int main(){
	int moduleCount;
	scanf("%d", &moduleCount);
	
	int baseAdresses[moduleCount];
	int highestAbsoluteAddressPointer = 0;

	//first pass
	//determine base address for each module
	//determine absolute address for each symbol (construct symbol table)
	int i;
	for(i = 0; i < moduleCount; i ++){
		baseAdresses[i] = highestAbsoluteAddressPointer;

		//handling definitions
		int numDefs;
		scanf("%d", &numDefs);

		int j;
		for(j = 0; j < numDefs; j ++){
			definition * d = (definition *) malloc(sizeof(definition));

			scanf("%s", &(d->name));
			scanf("%d", &(d->absoluteAddress));
			d->absoluteAddress += highestAbsoluteAddressPointer;
			d->next = NULL;
			addToBack(&symbolTable, d);
		}

		//handling use list
		scanf("%*d", &numPairs);
		scanf("%*[^\n]s"); //use list is useless on first pass
		

		//handling instructions
		int numInstructions = 0;
		scanf("%d", &numInstructions);

		//adding numInstrutctions to high pointer
		highestAbsoluteAddressPointer += numInstructions;
		//printf("%d\n", highestAbsoluteAddressPointer);

		
		char str[100];
		scanf("%[^\n]s", &str);
		printf("%s\n", str);	
	}
/*
	char str[100];
	scanf("%s", &str);
	printf("%s", str);*/

	return 0;
}

