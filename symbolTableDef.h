#include "actualParsing.c"

// typedef enum{
// 	defined, assigned
// } statusEnum

struct variableStruct{
	allEnum type;
};

struct arrayStruct{
	allEnum type;
	int rangeStart;
	int rangeEnd;
};

typedef struct {
	struct variableStruct variable;
	struct arrayStruct array;
	int isVariable;
	int width;
	int result;
	int isDeclared;
	int isAssigned;
}idVar;


struct scopeNodeStruct{
	int scope;
	int isSwitch;
	int isLoop;
	struct scopeNodeStruct * siblingNext;
	struct scopeNodeStruct * siblingPrev;

	struct scopeNodeStruct * parent;
	struct scopeNodeStruct * childListStart;
	struct scopeNodeStruct * childListEnd;
};

typedef struct scopeNodeStruct scopeNode;


struct argumentNode{
	struct variableStruct variable;
	struct arrayStruct array;
	int isVariable;

	struct argumentNode* next;
	struct argumentNode* prev;

	char ID[8];
};


typedef struct argumentNode argument;


typedef struct 
{
	int isUsed;
	int isDeclared;
	int isDefined;
	argument* inputArgument;
	argument* outputArgument;
}idFunction;
	
	
struct idNodeStruct
{
	//common fields
	char ID[8];
	int scope;
	int lineNo;
	// statusEnum status;


	int isVar;
	idVar var;
	idFunction fun;	
	struct idNodeStruct * next;
	struct idNodeStruct * prev;
};

typedef struct idNodeStruct idNode;