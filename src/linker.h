#ifndef LINKER_H
#define LINKER_H

typedef struct definition {
	char name[100];
	int absoluteAddress;

	struct definition * next;
} definition;

int addToBack(definition ** headPointer, definition * toAdd);

#endif
