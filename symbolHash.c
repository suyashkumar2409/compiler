#include <stdio.h>
#include <stdlib.h>
#include "symbolTableDef.h"


idNode** initHashFunc(int size)
{
	idNode** hashTable = (idNode**)malloc(sizeof(idNode*) * size);

	int i;
	for(i = 0;i<size;i++)
	{
		hashTable[i] = (idNode*)malloc(sizeof(idNode));
		hashTable[i]->next = NULL;
		hashTable[i]->prev = NULL;
	}

	return hashTable;
}

int getHash(char * inp, int size)
{
	int len = strlen(inp);

	int hash = 7%size;
	for(int i = 0;i<len;i++)
	{
		hash = hash*31 + (int)(inp[i]);
		hash = hash%size;
	}

	return hash;
}

void insert(idNode** hashTable, idNode* id, int size)
{
	int hash = getHash(id->ID, size);

	if(hashTable[hash]->next == NULL)
	{
		hashTable[hash]->next = id;
		id->prev = hashTable[hash];
		id->next = NULL;
	}
	else
	{
		id->next = hashTable[hash]->next;
		hashTable[hash]->next->prev = id;

		hashTable[hash]->next = id;
		id->prev = hashTable[hash];
	}
}

//NULL means not found
idNode* retrieve(idNode** hashTable, char* id, int scope, int size)
{
	int hash = getHash(id->ID, size);
	idNode* ptr = hashTable[hash]->next;

	while(ptr!=NULL)
	{
		if(strcmp(ptr->ID, id)!=0 && ptr->scope == scope)
			break;

		ptr = ptr->next;
	}

	return ptr;
}
