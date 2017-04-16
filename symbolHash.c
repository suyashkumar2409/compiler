#include <stdio.h>
#include <stdlib.h>
#include "actualParsing.c"


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


idNode* insert(idNode** hashTable, idNode* id, int size, TreeNode* idTree)
{
	int hash = getHash(id->ID, size);
	// printf("\n Trying to nInsert %s scope: %d",id->ID,id->scope);

	// printf("\nInsert Hash is %d\n",hash);

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

	id->valueAssigned = 0;

	idTree->entry = id;


}

idNode* insert2(idNode** hashTable, idNode* id, int size, idNode* endOfHash, TreeNode* idTree)
{
	insert(hashTable, id, size, idTree);
	if(endOfHash != NULL)
	{
		endOfHash->nextList = id;
	}

	idTree->entry = id;

	return id;
}


//NULL means not found
idNode* retrieve(idNode** hashTable, char* id, int scope, int size, TreeNode* idTree)
{
	int hash = getHash(id, size);
	idNode* ptr = hashTable[hash]->next;

	// printf("\n Trying to retrieve %s scope : %d",id, scope);
	// printf("\nRetrieve Hash is %d\n",hash);

	while(ptr!=NULL)
	{
		
		if(strcmp(ptr->ID, id)==0 && ptr->scope == scope)
			break;

		ptr = ptr->next;
	}

	if(ptr!=NULL)
	{
		idTree->entry = ptr;
	}

	return ptr;
}
