#include "symbolHash.c"
#include "symbolTableDef.h"
#include "actualParsing.c"

#define hashIdSize 1017
#define hashFunctionSize 1017

//convert single big symbol table to hierarchial table
//no need to do above, just add scope into search function
idNode** symbolId;
idNode** symbolFunction;

int global_scope = 0;
int current_scope = global_scope;

scopeNode* scopeTree = NULL;
scopeNode* currentScopeNode = NULL;
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

	scopeTree->scope = scope;
	scopeTree->isSwitch = isSwitch;
	scopeTree->isLoop = isLoop;

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
	currentScopeNode = scopeTree;
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

void populateSymbolTableFunction(TreeNode* root)
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
				// populateSymbolTable(root->childListEnd);
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

int checkIdInScope(idNode** hashtable, char * identifier, scopeNode* currentScopeNode)
{
	while(currentScopeNode!=NULL)
	{
		idNode* getVal = retrieve(hashtable, identifier, currentScopeNode->scope, hashFunctionSize);

		if(getVal != NULL)
		{
			return 1;
		}
		else
		{
			currentScopeNode = currentScopeNode->parent;
		}
	}

	return 0;
}

int checkExistenceIDLIST(idNode** hashtable, TreeNode* list, int scope)
{
	TreeNode* first = list->childListStart;
	TreeNode* n3  = list->childListEnd;

	int allCorrect = 1;

retrieve(idNode** hashTable, char* id, int scope, int size)
	if(retrieve)
}

void populateSymbolTableID(TreeNode* root)
{
	if(root!=NULL)
	{
		switch(root->allenum)
		{
			case nt_moduleDef:
				//increase scope, set not in loop or switch
				//populate with statements node
				//exit scope

				//scope increased
				current_scope = current_scope + 1;
				scopeNode* newNode = newScopeNode(current_scope, 0, 0);
				addChild(currentScopeNode, scopeNode);
				currentScopeNode = scopeNode;

				//recursively for statements
				populateSymbolTableID(root->childListStart->siblingNext);

				//exit scope
				currentScopeNode = currentScopeNode->parent;


				break;
			/*************** IO statement ************************/
			case nt_ioStmt:
				//check if GET_VALUE - check whether ID is present in scope
				if(root->childListStart->allenum == GET_VALUE)
				{
					TreeNode* idVal = root->childListStart->siblingNext->siblingNext;
					
					if(!checkIdInScope(symbolId, idVal->tokenInfo->identifier, currentScopeNode))
						printf("Error at line %d: %s is not in scope\n",idVal->tokenInfo->lineNo, idVal->tokenInfo->identifier );
				
				}

				//check if PRINT - run recursively for var
				else
				{
					populateSymbolTableID(root->childListStart->siblingNext->siblingNext);
				}


				break;


			case nt_var:
				TreeNode* firstChild = root->childListStart;
				// if NUM or RNUM, no problem
				if(firstChild->allenum == NUM || firstChild->allenum == RNUM)
					; //no problem

				//if ID, check ID, run recursively for whichID
				else
				{
					if(!checkIdInScope(symbolId, firstChild->tokenInfo->identifier, currentScopeNode))
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo->lineNo, firstChild->tokenInfo->identifier );
				
					populateSymbolTableID(firstChild->siblingNext);
				}

				break;

			case nt_whichID:
				//if e, no problem
				TreeNode* firstChild = root->childListStart;

				if(firstChild->allenum == EPSILON)
					;
				else
				//if sqbo, check ID
				{
					if(!checkIdInScope(symbolId, firstChild->siblingNext->tokenInfo->identifier, currentScopeNode))
						printf("Error at line %d: %s is not in scope\n",firstChild->siblingNext->tokenInfo->lineNo, firstChild->siblingNext->tokenInfo->identifier );
					
				}

				break;

			/*************** Simple statement **********************/
			case nt_assignmentStmt:
				//check ID
				TreeNode* firstChild = root->childListStart;
				if(!checkIdInScope(symbolId, firstChild->tokenInfo->identifier, currentScopeNode))
					printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo->lineNo, firstChild->tokenInfo->identifier );

				//recursively run which stmt				
				populateSymbolTableID(firstChild->siblingNext);
				
				break;

			case nt_lvalueIDStmt:
				//recursively run expression
				populateSymbolTableID(root->childListStart->siblingNext);

				break;

			case nt_lvalueARRStmt:
				//recursively run index and expression
				//index
				populateSymbolTableID(root->childListStart->siblingNext);

				//expression
				populateSymbolTableID(root->childListStart->siblingNext->siblingNext->siblingNext->siblingNext);

				break;
			case nt_index:
				//if num, no problem, if ID, check ID
				TreeNode* firstChild = root->childListStart;

				if(firstChild->allenum == NUM)
					;
				else
				//check ID
				{
					if(!checkIdInScope(symbolId, firstChild->tokenInfo->identifier, currentScopeNode))
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo->lineNo, firstChild->tokenInfo->identifier );
					
				}

				break;

			case nt_expression:
				/****************** THIS IS LONG DO IT LATER *************************/
				if(root->childListStart->allenum == nt_arithmeticOrBooleanExpr)
					populateSymbolTableID(root->childListStart);
				else
					populateSymbolTableID(root->childListStart->siblingNext->siblingNext);

				break;

			//AnyTerm(no need) N7(no need) arithmeticExpr(no need) N8(no need) term(no need) factor(no need) N5(no need) N4(no need) var - check for ID, whichId - check for ID
			case nt_var:
				//if num or rnum no problem
				if(root->childListStart->allenum == NUM||root->childListStart->allenum == RNUM)
					;
				else
				//if ID, check
				{
					TreeNode* firstChild = root->childListStart;
					if(!checkIdInScope(symbolId, firstChild->tokenInfo->identifier, currentScopeNode))
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo->lineNo, firstChild->tokenInfo->identifier );
					
					populateSymbolTableID(firstChild->siblingNext);
				}

				break;

			case nt_whichID:
				TreeNode* firstChild = root->childListStart;
				// if epsilon, no problem
				if(firstChild->allenum == EPSILON)
					;
				else
				// if sqbo check id
				{
					firstChild = firstChild->siblingNext;
					if(!checkIdInScope(symbolId, firstChild->tokenInfo->identifier, currentScopeNode))
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo->lineNo, firstChild->tokenInfo->identifier );
				}
				break;


			//*********** ALSO ANALYSE MODULE REUSE STATEMENT ***************
			
			case nt_moduleReuseStmt:
				TreeNode * firstChild = root->childListStart;
				//recursively add optional
				populateSymbolTableID(firstChild);
				//check ID
				firstChild = firstChild->siblingNext->siblingNext->siblingNext;

				if(!checkIdInScope(symbolId, firstChild->tokenInfo->identifier, currentScopeNode))
					printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo->lineNo, firstChild->tokenInfo->identifier );
				
				//recursively add IDLIST

				firstChild = firstChild->siblingNext->siblingNext->siblingNext;
				populateSymbolTableID(firstChild);

				break;


			case nt_optional:
				TreeNode * firstChild = root->childListStart;

				//if epsilon no problem
				if(firstChild->allenum == EPSILON)
					;
				else
				//else recursively add idList
				{
					firstChild = firstChild->siblingNext;
					populateSymbolTableID(firstChild);
				}

				break;

			case nt_idList:
				//check ID
				TreeNode* firstChild = root->childListStart;
				if(!checkIdInScope(symbolId, firstChild->tokenInfo->identifier, currentScopeNode))
					printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo->lineNo, firstChild->tokenInfo->identifier );

				//recursively add N3				
				firstChild = firstChild->siblingNext;
				populateSymbolTableID(firstChild)

			case N3:
				TreeNode* firstChild = root->childListStart;
				//if epsilon no problem
				if(firstChild->allenum == EPSILON)
					;
				else
				//else check ID recursively add N3
				{
					firstChild = firstChild->siblingNext;
					if(!checkIdInScope(symbolId, firstChild->tokenInfo->identifier, currentScopeNode))
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo->lineNo, firstChild->tokenInfo->identifier );

					//recursively add N3				
					firstChild = firstChild->siblingNext;
					populateSymbolTableID(firstChild)
				}
			/******************* declare statement ***********************/
			case nt_declareStmt:
				// instead for each ID in idlist, first check whether it already exists
				TreeNode * firstChild = root->childListStart;
				firstChild = firstChild->siblingNext;

				if(checkExistenceIDLIST(symbolId, firstChild, currentScopeNode->scope))  //check only in current scope
				{
				// for each ID in idList, insert in hashtable using datatype - handle both arrays and normal nums (is bound checking done?)
					insertIDLISTHash(symbolId, firstChild, firstChild->siblingNext->siblingNext, currentScopeNode);					
				}


			/******************* conditional statement ********************/
			case nt_conditionalStmt:
				// check ID in switch
				TreeNode* idVal = root->childListStart->siblingNext->siblingNext;
				if(!checkIdInScope(symbolId, idVal->tokenInfo->identifier, currentScopeNode))
					printf("Error at line %d: %s is not in scope\n",idVal->tokenInfo->lineNo, idVal->tokenInfo->identifier );
					
				//recursively analyse case statements 
				populateSymbolTableID(idVal->siblingNext->siblingNext->siblingNext);

				//and default
				populateSymbolTableID(root->childListEnd->siblingPrev);

				break;

			case nt_caseStmts:
				//increase scope
				current_scope = current_scope + 1;
				scopeNode* newNode = newScopeNode(current_scope, 1, 0);
				addChild(currentScopeNode, scopeNode);
				currentScopeNode = scopeNode;

				//recursively for statements
				populateSymbolTableID(root->childListStart->siblingNext->siblingNext->siblingNext);


				//recursively add statements


				//exit scope
				currentScopeNode = currentScopeNode->parent;

				//recursively add N9
				populateSymbolTableID(root->childListEnd);

				break;

			case nt_N9: //multicase
				if(root->childListStart->allenum == EPSILON)
					;
				else
				{
				//increase scope if not epsilon
					current_scope = current_scope + 1;
					scopeNode* newNode = newScopeNode(current_scope, 1, 0);
					addChild(currentScopeNode, scopeNode);
					currentScopeNode = scopeNode;

				//recursively for statements
					populateSymbolTableID(root->childListStart->siblingNext->siblingNext->siblingNext);

				//exit scope
				currentScopeNode = currentScopeNode->parent;

				//recursively add N9
				populateSymbolTableID(root->childListEnd);

				}
				
				break;
			case nt_default:
				//increase scope if not epsilon
				//recursively add statements if not epsilon
				if(root->childListStart->allenum == EPSILON)
					;
				else
				{
				//increase scope if not epsilon
					current_scope = current_scope + 1;
					scopeNode* newNode = newScopeNode(current_scope, 1, 0);
					addChild(currentScopeNode, scopeNode);
					currentScopeNode = scopeNode;

				//recursively for statements
					populateSymbolTableID(root->childListStart->siblingNext->siblingNext);

				//exit scope
				currentScopeNode = currentScopeNode->parent;
				}
				
				break;


			default:
				TreeNode* ptr = root->childListStart;

				while(ptr!=NULL)
				{
					populateSymbolTable(ptr);
					ptr = ptr->siblingNext;
				}

				break;
		}

		
	}


}


void mainOfSymbolTable()
{
	createHashTables();
	createScopeTree();

	populateSymbolTableID(parserTree);
	populateSymbolTableFunction(parserTree);
}