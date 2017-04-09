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
	struct scopeNodeStruct * childListStart;
	struct scopeNodeStruct * childListEnd;

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

	if(parent->childListEnd == NULL)
	{
		child->siblingPrev = NULL;
		child->siblingNext = NULL;
		parent->childListStart = child;
		parent->childListEnd = child;
	}
	else
	{
		child->siblingPrev = parent->childListEnd;
		child->siblingNext = NULL;
		parent->childListEnd = child;
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

	newFunc.inputArgument = NULL;
	newFunc.outputArgument = NULL;

	return newFunc;
}

idFunction createNewDefinitionFunction(argument* ip, argument* op)
{
	idFunction newFunc;
	newFunc.isDeclared = 0;
	newFunc.isDefined = 1;
	newFunc.isUsed = 0;

	newFunc.inputArgument = ip;
	newFunc.outputArgument = op;

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


// idNode* createNewIDNode(char* ID, int scope, int lineNo, int isVar, int isDeclared)
// {
// 	idNode* newNode = (idNode*)malloc(sizeof(idNode));

// 	strcpy(newNode->ID, ID);
// 	newNode->scope = scope;
// 	newNode->lineNo = lineNo;


// 	if(isVar)
// 	{
// 		// do for variables
// 	}
// 	else
// 	{
// 		if(isDeclared)
// 		{
// 			idFunction newFunction = createNewDeclaredFunction();
// 			newNode->fun = newFunction;
// 		}
// 		else
// 		{
// 			// do for defined functions
// 		}
// 	}

// 	return newNode;
// }
idNode* createNewFunctionDefinition(char* ID, int scope, int lineNo, argument* inputList, argument* outputList)
{
	idNode* newNode = (idNode*)malloc(sizeof(idNode));

	strcpy(newNode->ID, ID);
	newNode->scope = scope;
	newNode->lineNo = lineNo;

	idFunction newFunction = createNewDefinitionFunction(inputList, outputList);
	newNode->fun = newFunction;

	return newNode;
}

idNode* createNewFunctionDeclaration(char* ID, int scope, int lineNo)
{
	idNode* newNode = (idNode*)malloc(sizeof(idNode));

	strcpy(newNode->ID, ID);
	newNode->scope = scope;
	newNode->lineNo = lineNo;

	idFunction newFunction = createNewDeclaredFunction();
	newNode->fun = newFunction;
	
	return newNode;
}

argument* newArgumentNode()
{
	argument* temp =  (argument*)malloc(sizeof(argument));

	temp->next = NULL;
	temp->prev = NULL;

	return temp;
}

void fillArgument(argument* tempNode, TreeNode* id, TreeNode* datatype)
{
	if(datatype->childListStart->allenum == ARRAY)
	{
		tempNode->isVariable = 0;
		struct arrayStruct array;

		TreeNode* range = datatype->childListStart->siblingNext->siblingNext;
		TreeNode* type = datatype->childListEnd;

		int startRange = range->childListStart->tokenInfo->integer;
		int endRange = range->childListEnd->tokenInfo->integer;

		array.rangeStart = startRange;
		array.rangeEnd = endRange;

		array.type = type->childListStart->allenum;

		tempNode->array = array;

		strcpy(tempNode->ID, id->tokenInfo->identifier);

	}	
	else
	{
		tempNode->isVariable = 1;
		struct variableStruct var;
		var.allenum = datatype->childListStart->allenum;

		tempNode->variable = var;
		strcpy(tempNode->ID, id->tokenInfo->identifier);
	}
}

argument* analyseInput(TreeNode* inputPlist)
{
	argument* argumentHead = newArgumentNode();
	argument* argumentTail = argumentHead;

	//takes ID and datatype and fills argument
	fillArgument(argumentHead, inputPlist->childListStart, inputPlist->childListStart->siblingNext->siblingNext);

	TreeNode* ptr = inputPlist->childListEnd;

	//till end of input_plist2 is not found
	while(ptr->childListStart->allenum != EPSILON)
	{
		argument* tempNode= newArgumentNode();
		fillArgument(tempNode, ptr->childListStart->siblingNext, ptr->childListStart->siblingNext->siblingNext->siblingNext);

		tempNode->prev = argumentTail;
		argumentTail->next = tempNode;
		argumentTail = tempNode;

		ptr = ptr->childListEnd;
	}

	return argumentHead;
}

void analyseIO(TreeNode* list)
{
	return analyseInput(list);
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

				TreeNode* funcIdNode = root->childListStart->siblingNext->siblingNext;

				strcpy(funcName, funcIdNode->tokenInfo->identifier);
				
				//get scope
				int scope = current_scope;

				//get linenumber
				int lineNo = funcIdNode->tokenInfo->lineNo;

				//get symbol table node
				idNode* newSymbolNode = createNewFunctionDeclaration(funcName, scope,lineNo, 0, 1);
				
// ************* SEMANTIC ANALYSIS - CHECK WHETHER THIS DECLARATION IS ALREADY PRESENT 
				insert(symbolFunction, newSymbolNode);



				break;

			case nt_module: //********** IMPORTANT - ID names integrity has not been checked(same arguments), do in ID analysis


				//get name
				char funcName[10];

				TreeNode* funcIdNode = root->childListStart->siblingNext->siblingNext;

				strcpy(funcName, funcIdNode->tokenInfo->identifier);


				//get scope
				int scope = current_scope;

				//get linenumber
				int lineNo = funcIdNode->tokenInfo->lineNo;

				//analyses input_plist
				argument* inputArgument = analyseIO(funcIdNode->siblingNext->siblingNext->siblingNext->siblingNext->siblingNext);
				
				TreeNode* ret = root->childListEnd->siblingPrev;

				argument* outputArgument = analyseIO(ret->childListStart->siblingNext->siblingNext);

				idNode* newSymbolNode = createNewFunctionDefinition(funcName, scope, lineNo, inputArgument, outputArgument);

// ************** SEMANTIC ANALYSIS - CHECK WHETHER THIS DEFINITION IS ALREADY PRESENT 
				insert(symbolFunction, newSymbolNode);

				//for moduleDef
				populateSymbolTable(root->childListEnd);
				break;

			// ******* Add case for moduledef and check for function invocations

		}

		TreeNode* ptr = root->childListStart;

// should this really be global instead of default?
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