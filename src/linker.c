#include <stdio.h>
#include <stdlib.h>
#include "linker.h"

definition * symbolTable = 0;

int addToBack(definition ** headPointer, definition * toAdd){
	if(*headPointer == 0){
		* headPointer = toAdd;
		return 1;
	}
	else{
		definition * last = (* headPointer)->next;
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
			addToBack(&symbolTable, d);
		}

		
	}
	
	return 0;
}

