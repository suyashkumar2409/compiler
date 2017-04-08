#include "lexer.c"

typedef enum{
	defined, assigned
} statusEnum

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
}idVar;


struct scopeNodeStruct{
	int scope;
	int isSwitch;
	int isLoop;
	struct scopeNodeStruct * siblingNext;
	struct scopeNodeStruct * siblingPrev;

	struct scopeNodeStruct * parent;
	struct scopeNodeStruct * childStart;
	struct scopeNodeStruct * childEnd;
};

typedef struct scopeNodeStruct scopeNode;


struct argumentNode{
	struct variableStruct variable;
	struct arrayStruct array;
	int isVariable;

	struct argumentNode* next;
};


typedef struct argumentNode argument;


typedef struct 
{
	int used;
	argument* argumentHead;
	argument* argumentTail;
}idFunction;
	
	
struct idNodeStruct
{
	//common fields
	char ID[10];
	int scope;
	int lineNo;
	statusEnum status;


	int isVar;
	idVar var;
	idFunction fun;	
	struct idNodeStruct * next;
	struct idNodeStruct * prev;
};

typedef struct idNodeStruct idNode;