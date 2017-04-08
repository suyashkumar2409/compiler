#include "symbolHash.c"
#include "symbolTableDef.h"
#include "actualParsing.c"

#define hashIdSize 1017
#define hashFunctionSize 1017

idNode** hashId;
idNode** hashFunction;

void createHashTables()
{
	hashId = initHashFunc(hashIdSize);
	hashFunction = initHashFunc(hashFunctionSize);
}