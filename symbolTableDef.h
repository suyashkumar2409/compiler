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

	char scopeName[10];
	int scopeStart;
	int scopeEnd;

	int nesting;
};

typedef struct scopeNodeStruct scopeNode;


struct argumentNode{
	struct variableStruct variable;
	struct arrayStruct array;
	int isVariable;

	struct argumentNode* next;
	struct argumentNode* prev;

	char ID[10];
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
	char ID[10];
	int scope;
	int lineNo;
	// statusEnum status;
	char scopeName[10];
	int scopeStart;
	int scopeEnd;

	int nesting;
	int offset;

	int valueAssigned;

	int isVar;
	idVar var;
	idFunction fun;	
	struct idNodeStruct * next;
	struct idNodeStruct * prev;

	struct idNodeStruct * nextList;
};

typedef struct idNodeStruct idNode;