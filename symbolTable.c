#include "symbolHash.c"
#include "symbolTableDef.h"
#include "actualParsing.c"

#define hashIdSize 1017
#define hashFunctionSize 1017

idNode** symbolId;
idNode** symbolFunction;

int global_scope = 0;
int current_scope = global_scope;

scopeNode* scopeTree = NULL;
	int scope;
	int isSwitch;
	int isLoop;
	struct scopeNodeStruct * siblingNext;
	struct scopeNodeStruct * siblingPrev;

	struct scopeNodeStruct * parent;
	struct scopeNodeStruct * childStart;
	struct scopeNodeStruct * childEnd;

scopeNode* newScopeNode(int scope, int isSwitch, int isLoop)
{
	scopeNode* scopeTree = (scopeNode*)malloc(sizeof(scopeNode));

	scopeTree->scope = global_scope;
	scopeTree->isSwitch = 0;
	scopeTree->isLoop = 0;

	return scopeTree;
}

void addChild(scopeNode* parent, scopeNode* child)
{
	child->parent = parent;

	if(parent->childEnd == NULL)
	{
		child->siblingPrev = NULL;
		child->siblingNext = NULL;
		parent->childStart = child;
		parent->childEnd = child;
	}
	else
	{
		child->siblingPrev = parent->childEnd;
		child->siblingNext = NULL;
		parent->childEnd = child;
	}
}


void createScopeTree()
{
	scopeTree = newScopeNode(global_scope, 0, 0);	
}

void createHashTables()
{
	symbolId = initHashFunc(hashIdSize);
	symbolFunction = initHashFunc(hashFunctionSize);
}

idVar createNewID();            //Implement this;

idFunction createNewDeclaredFunction()
{
	idFunction newFunc;
	newFunc.isDeclared = 1;
	newFunc.isDefined = 0;
	newFunc.isUsed = 0;

	newFunc.argumentHead = NULL;
	newFunc.argumentTail = NULL;

	return newFunc;
}

struct idNodeStruct
{
	//common fields
	char ID[10];
	int scope;
	int lineNo;
	// statusEnum status;


	int isVar;
	idVar var;
	idFunction fun;	
	struct idNodeStruct * next;
	struct idNodeStruct * prev;
};

idNode* createNewIDNode(char* ID, int scope, int lineNo, int isVar, int isDeclared)
{
	idNode* newNode = (idNode*)malloc(sizeof(idNode));

	strcpy(newNode->ID, ID);
	newNode->scope = scope;
	newNode->lineNo = lineNo;


	if(isVar)
	{
		// do for variables
	}
	else
	{
		if(isDeclared)
		{
			idFunction newFunction = createNewDeclaredFunction();
			newNode->fun = newFunction;
		}
		else
		{
			// do for defined functions
		}
	}

	return newNode;
}

void populateSymbolTable(TreeNode* root)
{
	if(root!=NULL)
	{
		switch(root->allenum)
		{

			//Function Declaration
			case nt_moduleDeclaration:
				
				//get name
				char funcName[10];

				TreeNode* funcIdNode = root->childStart->siblingNext->siblingNext;

				strcpy(funcName, funcIdNode->tokenInfo->identifier);
				
				//get scope
				int scope = current_scope;

				//get linenumber
				int lineNo = funcIdNode->tokenInfo->lineNo;

				//get symbol table node
				idNode* newSymbolNode = createNewIDNode(funcName, scope,lineNo, 0, 1);
				insert(symbolFunction, newSymbolNode);

				break;
		}

		TreeNode* ptr = root->childListStart;

		while(ptr!=NULL)
		{
			populateSymbolTable(ptr);
			ptr = ptr->siblingNext;
		}
	}
}

void mainOfSymbolTable()
{
	createHashTables();
	populateSymbolTable(parserTree);
	createScopeTree();
}