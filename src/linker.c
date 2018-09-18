#include <stdio.h>
#include <stdlib.h>
#include "linker.h"
#include <string.h>

//two pass linker
//Ben Haderle
//9 17 2018

char symbolNames[1000][10];
int symbolAddresses[1000];
usePair * usePairs[1000];
int memoryModel[1000];

int numSymbols = 0;
int numPairs = 0;

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
			strcpy(symbolNames[numSymbols], name);
			scanf("%d", &(symbolAddresses[numSymbols]));
			symbolAddresses[numSymbols] += highestAbsoluteAddressPointer;
			printf("%s=%d\n", symbolNames[numSymbols], symbolAddresses[numSymbols]);
			numSymbols++;
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

		for(j = 0; j < numInstructions; j ++){
			int inst;
			scanf("%d", &inst);
			int addressType = inst % 10;
			if(addressType == 1 || addressType == 2){
				memoryModel[j + highestAbsoluteAddressPointer] = inst / 10;
			}
			else if(addressType == 3){
				memoryModel[j + highestAbsoluteAddressPointer] = (inst / 10) + highestAbsoluteAddressPointer;
			}
			else if(addressType == 4){
				memoryModel[j + highestAbsoluteAddressPointer] = (inst / 1000) * 100;
			}
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
		int useAddress;
		int j;

		for(j = 0; j < numSymbols; j ++){
			//printf("symbol name %d: %s\tuse name: %s\n", j, symbolNames[0], cur->name);
			//printf("%d\n", strcmp(symbolNames[j], cur->name) == 0);
			if(strcmp(symbolNames[j], cur->name) == 0){
				useAddress = symbolAddresses[j];
				j = numSymbols;
			}
		}
		
		for(j = 0; j < cur->numUses; j ++){
			memoryModel[(cur->uses[j])] += useAddress;
		}
	}

	for(i = 0; i < highestAbsoluteAddressPointer; i ++){
		printf("%d:\t%d\n", i, memoryModel[i]);
	}

	return 0;
}

