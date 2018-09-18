#include <stdio.h>
#include <stdlib.h>
#include "linker.h"
#include <string.h>

//two pass linker
//Ben Haderle
//9 17 2018

char symbolNames[1000][10];
int symbolAddresses[1000];
int symbolUsed[1000];
usePair * usePairs[1000];
int memoryModel[1000];
int externalResolution[1000];

int numSymbols = 0;
int numPairs = 0;

int checkForDuplicate(int length, char names[1000][10], char check[10]){
	int i;
	for(i = 0; i < length; i ++){
		if(strcmp(check, names[i]) == 0){
			return i;
		}
	}

	return -1;
}

int main(){
	int moduleCount;
	scanf("%d", &moduleCount);

	int baseAdresses[moduleCount];
	int highestAbsoluteAddressPointer = 0;

	//printing stuff for symbol table
	printf("Symbol Table\n");

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
			char name[10];
			scanf("%s", &name);
			//check for duplicate defintions
			int check = checkForDuplicate(numSymbols, symbolNames, name);
			if(check == -1){
				strcpy(symbolNames[numSymbols], name);
				scanf("%d", &(symbolAddresses[numSymbols]));
				symbolAddresses[numSymbols] += highestAbsoluteAddressPointer;
				printf("%s=%d\n", symbolNames[numSymbols], symbolAddresses[numSymbols]);
				symbolUsed[numSymbols] = 0;
				numSymbols++;
			}
			else{
				scanf("%d", &(symbolAddresses[check]));
				symbolAddresses[check] += highestAbsoluteAddressPointer;
				printf("WARNING: There are multiple defintions of '%s'! Linker will use last declared value.\n", name);
				printf("%s=%d\n", symbolNames[check], symbolAddresses[check]);
			}
		}

		//handling use list
		int numPairsInModule;
		scanf("%d", &numPairsInModule);
		for(j = 0; j < numPairsInModule; j++){
			usePair * u = (usePair *) malloc(sizeof(usePair));

			//getting name
			scanf("%s",&(u->name));

			//getting uses
			int nextUse;
			int useIndex = 0;
			scanf("%d", &nextUse);

			while(useIndex < 10 && nextUse != -1){
				u->uses[useIndex] = nextUse + highestAbsoluteAddressPointer;
				scanf("%d", &nextUse);
				useIndex ++;
			}

			u->numUses = useIndex;
			usePairs[numPairs] = u;
			numPairs ++;
		}
		

		//handling instructions
		int numInstructions = 0;
		scanf("%d", &numInstructions);

		//hchecking for out of scope definitions
		for(j = numSymbols - numDefs; j < numSymbols; j ++){
			if(symbolAddresses[j] - highestAbsoluteAddressPointer > numInstructions - 1){
				printf("WARNING: Defined address for '%s' is out of module scope.  Last word in module will be used instead.\n", symbolNames[j]);
				symbolAddresses[j] = numInstructions - 1 + highestAbsoluteAddressPointer;
				printf("%s=%d\n", symbolNames[j], symbolAddresses[j]);
			}
		}
		

		for(j = 0; j < numInstructions; j ++){
			int inst;
			scanf("%d", &inst);
			int addressType = inst % 10;
			if(addressType == 1){
				memoryModel[j + highestAbsoluteAddressPointer] = inst / 10;
			}
			else if(addressType == 2){
				if((inst / 10) % 1000 > 229){
					printf("WARNING: Absolute address @ '%d' exceeds machine size. Largest legal value will be used instead.\n", j + highestAbsoluteAddressPointer);
					int f = (inst / 10000);
					inst = (f * 10000) + 2290;
				}
				memoryModel[j + highestAbsoluteAddressPointer] = inst / 10;
			}
			else if(addressType == 3){
				memoryModel[j + highestAbsoluteAddressPointer] = (inst / 10) + highestAbsoluteAddressPointer;
			}
			else if(addressType == 4){
				memoryModel[j + highestAbsoluteAddressPointer] = (inst / 10000) * 1000;
			}
			externalResolution[j + highestAbsoluteAddressPointer] = 0;
		}

		//adding numInstrutctions to high pointer
		highestAbsoluteAddressPointer += numInstructions;	
	}

	//more printing
	printf("\nMemory Map\n");

	//second pass
	//adding external use pair addresses to memmodel
	for(i = 0; i < numPairs; i ++){
		usePair * cur = usePairs[i];
		int useAddress = 111;
		int j;

		for(j = 0; j < numSymbols; j ++){
			//printf("symbol name %d: %s\tuse name: %s\n", j, symbolNames[0], cur->name);
			//printf("%d\n", strcmp(symbolNames[j], cur->name) == 0);
			if(strcmp(symbolNames[j], cur->name) == 0){
				useAddress = symbolAddresses[j];
				symbolUsed[j] = 1;
				j = numSymbols;
			}
		}

		//symbol used but not defined error
		if(useAddress == 111){
			printf("WARNING: Symbol '%s' is used but not defined! Value '111' will be used instead of symbol value.\n", cur->name);
		}
		
		for(j = 0; j < cur->numUses; j ++){
			if(externalResolution[(cur->uses[j])] == 0){
				externalResolution[(cur->uses[j])] = useAddress;
			}
			else{
				printf("WARNING: Multiple symbols used in instruction @ '%d'.  Only last usage will be used.\n", (cur->uses[j]));
				externalResolution[(cur->uses[j])] = useAddress;
			}
		}
	}

	//symbol defined but not used error
	for(i = 0; i < numSymbols; i ++){
		if(symbolUsed[i] == 0){
			printf("WARNING: Symbol '%s' is defined, but not used!\n", symbolNames[i]);
		}
	}

	for(i = 0; i < highestAbsoluteAddressPointer; i ++){
		printf("%d:\t%d\n", i, memoryModel[i] + externalResolution[i]);
	}

	return 0;
}

