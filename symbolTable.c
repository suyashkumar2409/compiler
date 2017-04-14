#include "symbolHash.c"
#define hashIdSize 1017
#define hashFunctionSize 1017

//convert single big symbol table to hierarchial table
//no need to do above, just add scope into search function
idNode** symbolId;
idNode** symbolFunction;

int global_scope = 0;
int current_scope = 0;

int global_offset = 0;
char currentScopeName[10];
// strcpy(currentScopeName, "global");

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

scopeNode* newScopeNode(int scope, int isSwitch, int isLoop, int startLineNo)
{
	scopeNode* scopeTree = (scopeNode*)malloc(sizeof(scopeNode));

	scopeTree->scope = scope;
	scopeTree->isSwitch = isSwitch;
	scopeTree->isLoop = isLoop;

	strcpy(scopeTree->scopeName, currentScopeName);

	scopeTree->scopeStart= startLineNo;


	return scopeTree;
}

void addChildScope(scopeNode* parent, scopeNode* child)
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

	child->nesting = parent->nesting + 1;
}


void createScopeTree()
{
	scopeTree = newScopeNode(global_scope, 0, 0, 0);	
	currentScopeNode = scopeTree;
}

void createHashTables()
{
	symbolId = initHashFunc(hashIdSize);
	symbolFunction = initHashFunc(hashFunctionSize);
}



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

		int startRange = range->childListStart->tokenInfo.integer;
		int endRange = range->childListEnd->tokenInfo.integer;

		array.rangeStart = startRange;
		array.rangeEnd = endRange;

		array.type = type->childListStart->allenum;

		tempNode->array = array;

		strcpy(tempNode->ID, id->tokenInfo.identifier);

	}	
	else
	{
		tempNode->isVariable = 1;
		struct variableStruct var;
		var.type = datatype->childListStart->allenum;

		tempNode->variable = var;
		strcpy(tempNode->ID, id->tokenInfo.identifier);
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

argument * analyseIO(TreeNode* list)
{
	return analyseInput(list);
}

idNode* checkIdInScope(idNode** hashtable, char * identifier, scopeNode* currentScopeNode)
{
	while(currentScopeNode!=NULL)
	{
		idNode* getVal = retrieve(hashtable, identifier, currentScopeNode->scope, hashFunctionSize);

		if(getVal != NULL)
		{
			return getVal;
		}
		else
		{
			currentScopeNode = currentScopeNode->parent;
		}
	}

	return NULL;
}

int checkExistenceIDLIST(idNode** hashtable, TreeNode* list, int scope)
{
	TreeNode* first = list->childListStart;
	TreeNode* n3  = list->childListEnd;

	int allCorrect = 1;

	idNode* temp = retrieve(hashtable, first->tokenInfo.identifier, scope, hashFunctionSize);

	if(temp != NULL)
	{
		allCorrect = 0;
		printf("Error at line %d: %s has been redeclared, originally defined at line %d:\n",first->tokenInfo.lineNo, first->tokenInfo.identifier, temp->lineNo);
	}

	while(n3->childListStart->allenum != EPSILON)
	{
		first = n3->childListStart->siblingNext;

		temp = retrieve(hashtable, first->tokenInfo.identifier, scope, hashFunctionSize);
		if(temp != NULL)
		{
			allCorrect = 0;
			printf("Error at line %d: %s has been redeclared, originally defined at line %d:\n",first->tokenInfo.lineNo, first->tokenInfo.identifier, temp->lineNo);
		}
		n3 = n3->childListEnd;
	}

	return allCorrect;
}

idNode* createNewIDVar(char * identifier,scopeNode* currScope , int lineNo, allEnum datatype)
{
	int scope = currScope->scope;
	idNode* newNode = (idNode*)malloc(sizeof(idNode));
	strcpy(newNode->ID, identifier);

	newNode->scope = scope;
	newNode->lineNo = lineNo;

	newNode->isVar = 1;

	idVar var;

	var.variable.type = datatype;
	var.isVariable = 1;
	var.isDeclared  =1;
	var.isAssigned = 0;


	switch(datatype)
	{
		case INTEGER:
			var.width = 2;
			break;
		case REAL:
			var.width = 4;
			break;
		case BOOLEAN:
			var.width = 1;
			break;
	}

	strcpy(newNode->scopeName, currScope->scopeName);
	newNode->scopeStart = currScope->scopeStart;
	newNode->scopeEnd = currScope->scopeEnd;
	newNode->nesting = currScope->nesting;

	newNode->offset = global_offset;
	global_offset = global_offset + var.width;

	newNode->var = var;


	return newNode;
}

idNode* createNewIDArray(char * identifier, scopeNode* currScope, int lineNo, TreeNode* datatype)
{

	int scope = currScope->scope;
	idNode* newNode = (idNode*)malloc(sizeof(idNode));
	strcpy(newNode->ID, identifier);

	newNode->scope = scope;
	newNode->lineNo = lineNo;

	newNode->isVar = 0;

	idVar var;

	TreeNode* range = datatype->childListStart->siblingNext->siblingNext;

	var.array.type = datatype->childListEnd->childListStart->tokenInfo.allenum;
	var.array.rangeStart = range->childListStart->tokenInfo.integer;
	var.array.rangeEnd = range->childListEnd->tokenInfo.integer;

	int numLen = var.array.rangeEnd - var.array.rangeStart + 1;

	var.isVariable = 0;
	var.isDeclared  =1;
	var.isAssigned = 0;


	switch(var.array.type)
	{
		case INTEGER:
			var.width = 2;
			break;
		case REAL:
			var.width = 4;
			break;
		case BOOLEAN:
			var.width = 1;
			break;
	}	

	var.width = var.width * numLen;


	strcpy(newNode->scopeName, currScope->scopeName);
	newNode->scopeStart = currScope->scopeStart;
	newNode->scopeEnd = currScope->scopeEnd;
	newNode->nesting = currScope->nesting;

	newNode->offset = global_offset;
	global_offset = global_offset + var.width;

	newNode->var = var;


	return newNode;
}

void insertSingleIDHash(idNode** hashtable,TreeNode* id,TreeNode* datatype, scopeNode* currScope)
{
	idNode* newIdNode;
	int scope = currScope->scope;
	if(datatype->childListStart->allenum == ARRAY)
	{
		newIdNode = createNewIDArray(id->tokenInfo.identifier, currScope, id->tokenInfo.lineNo, datatype);	
	}
	else
	{
		newIdNode = createNewIDVar(id->tokenInfo.identifier, currScope, id->tokenInfo.lineNo, datatype->tokenInfo.allenum);
	}

	
	insert(hashtable, newIdNode, hashFunctionSize);	

}

// for each ID in idList, insert in hashtable using datatype - handle both arrays and normal nums (is bound checking done?)
void insertIDLISTHash(idNode** hashtable,TreeNode* list,TreeNode* datatype, scopeNode* currScope)
{
	int scope = currScope->scope;
	TreeNode* first = list->childListStart;
	TreeNode* n3  = list->childListEnd;

	insertSingleIDHash(hashtable, first, datatype, currScope);

	while(n3->childListStart->allenum != EPSILON)
	{
		first = n3->childListStart->siblingNext;
		insertSingleIDHash(hashtable, first, datatype, currScope);
		n3 = n3->childListEnd;
	}
}	

void checkAndAdd(idNode** hashtable, TreeNode* id, TreeNode* datatype, scopeNode* currScope)
{
	if(checkIdInScope(hashtable, id->tokenInfo.identifier, currentScopeNode) != NULL)
		printf("Error at line %d: %s input parameter is being redefined\n",id->tokenInfo.lineNo, id->tokenInfo.identifier);
	else
	{
		idNode* newNode = createNewIDVar(id->tokenInfo.identifier, currScope, id->tokenInfo.lineNo, datatype->allenum);
		insert(symbolId , newNode, hashFunctionSize);		
	}
}

void checkAndAddInputList(idNode** symbolId,TreeNode* ipList, scopeNode* currScope)
{
	TreeNode* ptr = ipList->childListStart;

	checkAndAdd(symbolId, ptr, ptr->siblingNext->siblingNext, currScope);

	ptr = ipList->childListEnd;
	while(ptr->childListStart->allenum != EPSILON)
	{
		checkAndAdd(symbolId, ptr->childListStart->siblingNext, ptr->childListStart->siblingNext->siblingNext->siblingNext,  currScope);		
		ptr = ptr->childListEnd;
	}
}

void checkOPID(idNode** hashtable, TreeNode* id, scopeNode* currScope)
{
	if(checkIdInScope(hashtable, id->tokenInfo.identifier, currentScopeNode) == NULL)
		printf("Error at line %d: %s output parameter has not been defined\n",id->tokenInfo.lineNo, id->tokenInfo.identifier);
}

void checkOutputParameters(idNode** hashtable, TreeNode* list, scopeNode* currScope)
{
	TreeNode* ptr = list->childListStart;

	checkOPID(hashtable, ptr, currScope);

	ptr = list->childListEnd;
	while(ptr->childListStart->allenum != EPSILON)
	{
		checkOPID(hashtable, ptr->childListStart->siblingNext, currScope);
		ptr = ptr->childListEnd;
	}
}


void populateSymbolTableID(TreeNode* root)
{

	if(root!=NULL)

	{
		// printf("%s\n",TerminalsAndNonTerminalsList[root->allenum]);
		switch(root->allenum)
		{
			case START:
			{

				current_scope = current_scope + 1;
				scopeNode* newNode = newScopeNode(current_scope, 0, 0, root->tokenInfo.lineNo);
				addChildScope(currentScopeNode, newNode);
				currentScopeNode = newNode;
				break;
			}

			case END:
			{
				currentScopeNode->scopeEnd = root->tokenInfo.lineNo;
				currentScopeNode = currentScopeNode->parent;

				break;
			}
			// Function Declaration
			case nt_moduleDeclaration:
				
			{
				//get name

				char funcName[10];

				TreeNode* funcIdNode = root->childListStart->siblingNext->siblingNext;

				strcpy(funcName, funcIdNode->tokenInfo.identifier);
				
				//get scope
				int scope = current_scope;

				//get linenumber
				int lineNo = funcIdNode->tokenInfo.lineNo;

				//get symbol table node
				idNode* newSymbolNode = createNewFunctionDeclaration(funcName, scope,lineNo);

				//only defined
				newSymbolNode->fun.isDeclared = 0;
				newSymbolNode->fun.isDefined = 1;
				newSymbolNode->fun.isUsed = 0;
				
// ************* SEMANTIC ANALYSIS - CHECK WHETHER THIS DECLARATION IS ALREADY PRESENT 
				insert(symbolFunction, newSymbolNode, hashFunctionSize);



				break;

			}

			case nt_module: //********** IMPORTANT - ID names integrity has not been checked(same arguments), do in ID analysis

			{

				//get name
				char funcName[10];

				TreeNode* funcIdNode = root->childListStart->siblingNext->siblingNext;

				strcpy(funcName, funcIdNode->tokenInfo.identifier);
				strcpy(currentScopeName , funcName);


				//get scope
				int scope = current_scope;

				//get linenumber
				int lineNo = funcIdNode->tokenInfo.lineNo;

				//analyses input_plist
				argument* inputArgument = analyseIO(funcIdNode->siblingNext->siblingNext->siblingNext->siblingNext->siblingNext);
				
				TreeNode* ret = root->childListEnd->siblingPrev;

				argument* outputArgument = NULL;
				if(ret->childListStart->allenum != EPSILON)
					outputArgument = analyseIO(ret->childListStart->siblingNext->siblingNext);

				idNode* newSymbolNode = createNewFunctionDefinition(funcName, scope, lineNo, inputArgument, outputArgument);

// ******************* THERE IS A PROBLEM HERE - DONT LET START AND END REACH RECURSION, increase scope on INPUT 
// ************** SEMANTIC ANALYSIS - CHECK WHETHER THIS DEFINITION IS ALREADY PRESENT or if already has been declared
				//1. Check if definition is already there, if yes - throw error
				//2. insert input list into current scope + 1 (without incrementing scope - this is essential)
				//3. recursively analyse def
				//4. check output params

				TreeNode* idVal = root->childListStart->siblingNext->siblingNext;

				//check if it has been defined or declared

				idNode* statusCheck = checkIdInScope(symbolFunction, idVal->tokenInfo.identifier, currentScopeNode);
				if(statusCheck!=NULL)
				{
					// it is being redeclared
					if(statusCheck->fun.isDeclared == 1)
						printf("Error at line %d: %s function is being redefined\n",idVal->tokenInfo.lineNo, idVal->tokenInfo.identifier );
					else//it has only been defined
					{
						// function has already been defined - entry exists in symbol table, just add argument info
						statusCheck->fun.isDeclared = 1;
						statusCheck->fun.inputArgument = inputArgument;
						statusCheck->fun.outputArgument = outputArgument;

					}
				}
				else
				{	//neither declared nor defined
					newSymbolNode->fun.isDeclared = 1;
					insert(symbolFunction, newSymbolNode, hashFunctionSize);
				}


				current_scope = current_scope + 1;
				scopeNode* newNode = newScopeNode(current_scope, 0, 0, root->tokenInfo.lineNo);
				addChildScope(currentScopeNode, newNode);
				currentScopeNode = newNode;

				checkAndAddInputList(symbolId, idVal->siblingNext->siblingNext->siblingNext->siblingNext->siblingNext, currentScopeNode);

				if(ret->childListStart->allenum != EPSILON)
				{
					//current scope + 1 because these are defined in the function def
					checkAndAddInputList(symbolId, ret->childListStart->siblingNext->siblingNext, currentScopeNode);
				}

				populateSymbolTableID(root->childListEnd->childListStart->siblingNext);

				currentScopeNode->scopeEnd = root->tokenInfo.lineNo;
				currentScopeNode = currentScopeNode->parent;

				

				break;
			}
// 			// ******* Add case for moduledef and check for function invocations

			case nt_driverModule:
			{
				strcpy(currentScopeName, "driver");
				populateSymbolTableID(root->childListEnd);
				break;
			}



			/***************** ID Related Symbol Table Operations *****************/
			//**** moduleDef no longer needed
			/*************** IO statement ************************/
			case nt_ioStmt:
				{
				//check if GET_VALUE - check whether ID is present in scope
				if(root->childListStart->allenum == GET_VALUE)
				{
					TreeNode* idVal = root->childListStart->siblingNext->siblingNext;
					
					if(checkIdInScope(symbolId, idVal->tokenInfo.identifier, currentScopeNode)==NULL)
						printf("Error at line %d: %s is not in scope\n",idVal->tokenInfo.lineNo, idVal->tokenInfo.identifier );
				
				}

				//check if PRINT - run recursively for var
				else
				{
					populateSymbolTableID(root->childListStart->siblingNext->siblingNext);
				}


				break;

				}
			// NT VAR HAS BEEN DONE LATER
			// WHICH ID HAS BEEN DONE LATER
			/*************** Simple statement **********************/
			case nt_assignmentStmt:
				{
				//check ID
				TreeNode* firstChild = root->childListStart;
				if(checkIdInScope(symbolId, firstChild->tokenInfo.identifier, currentScopeNode)==NULL)
					printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo.lineNo, firstChild->tokenInfo.identifier );

				//recursively run which stmt				
				populateSymbolTableID(firstChild->siblingNext);
				
				break;
				}
			case nt_lvalueIDStmt:
				{
				//recursively run expression
				populateSymbolTableID(root->childListStart->siblingNext);

				break;
				}
			case nt_lvalueARRStmt:
				{
				//recursively run index and expression
				//index
				populateSymbolTableID(root->childListStart->siblingNext);

				//expression
				populateSymbolTableID(root->childListStart->siblingNext->siblingNext->siblingNext->siblingNext);

				break;

				}
			case nt_index:
				{
				//if num, no problem, if ID, check ID
				TreeNode* firstChild = root->childListStart;

				if(firstChild->allenum == NUM)
					;
				else
				//check ID
				{
					if(checkIdInScope(symbolId, firstChild->tokenInfo.identifier, currentScopeNode)==NULL)
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo.lineNo, firstChild->tokenInfo.identifier );
					
				}

				break;

				}
			case nt_expression:
				{
				/****************** THIS IS LONG DO IT LATER *************************/
				if(root->childListStart->allenum == nt_arithmeticOrBooleanExpr)
					populateSymbolTableID(root->childListStart);
				else if(root->childListStart->siblingNext->allenum == BO)
					populateSymbolTableID(root->childListStart->siblingNext->siblingNext);
				else
					populateSymbolTableID(root->childListStart->siblingNext);


				break;

				}

			//AnyTerm(no need) N7(no need) arithmeticExpr(no need) N8(no need) term(no need) factor(no need) N5(no need) N4(no need) var - check for ID, whichId - check for ID
			case nt_var:

				{
				//if num or rnum no problem
				if(root->childListStart->allenum == NUM||root->childListStart->allenum == RNUM)
					;
				else
				//if ID, check
				{
					TreeNode* firstChild = root->childListStart;
					if(checkIdInScope(symbolId, firstChild->tokenInfo.identifier, currentScopeNode)==NULL)
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo.lineNo, firstChild->tokenInfo.identifier );
					
					populateSymbolTableID(firstChild->siblingNext);
				}

				break;

				}

			case nt_whichId:

				{
				TreeNode* firstChild = root->childListStart;
				// if epsilon, no problem
				if(firstChild->allenum == EPSILON)
					;
				else
				// if sqbo check id
				{
					firstChild = firstChild->siblingNext;
					if(checkIdInScope(symbolId, firstChild->tokenInfo.identifier, currentScopeNode)==NULL)
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo.lineNo, firstChild->tokenInfo.identifier );
				}
				break;

				}	


			//*********** ALSO ANALYSE MODULE REUSE STATEMENT ***************
			
			case nt_moduleReuseStmt:
				{

				TreeNode * firstChild = root->childListStart;
				//recursively add optional
				populateSymbolTableID(firstChild);
				//check ID
				firstChild = firstChild->siblingNext->siblingNext->siblingNext;

				if(checkIdInScope(symbolFunction, firstChild->tokenInfo.identifier, currentScopeNode)==NULL)
					printf("Error at line %d: %s function has not been defined\n",firstChild->tokenInfo.lineNo, firstChild->tokenInfo.identifier );
				
				//recursively add IDLIST

				firstChild = firstChild->siblingNext->siblingNext->siblingNext;
				populateSymbolTableID(firstChild);

				break;

				}



			case nt_optional:
				{

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

				}

			case nt_idList:
				{

				//check ID
				TreeNode* firstChild = root->childListStart;
				if(checkIdInScope(symbolId, firstChild->tokenInfo.identifier, currentScopeNode)==NULL)
					printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo.lineNo, firstChild->tokenInfo.identifier );

				//recursively add N3				
				firstChild = firstChild->siblingNext;
				populateSymbolTableID(firstChild);

				break;

				}

			case nt_N3:
				{

				TreeNode* firstChild = root->childListStart;
				//if epsilon no problem
				if(firstChild->allenum == EPSILON)
					;
				else
				//else check ID recursively add N3
				{
					firstChild = firstChild->siblingNext;
					if(checkIdInScope(symbolId, firstChild->tokenInfo.identifier, currentScopeNode)==NULL)
						printf("Error at line %d: %s is not in scope\n",firstChild->tokenInfo.lineNo, firstChild->tokenInfo.identifier );

					//recursively add N3				
					firstChild = firstChild->siblingNext;
					populateSymbolTableID(firstChild);
				}

				break;

				}
			/******************* declare statement ***********************/
			case nt_declareStmt:

				{
				// instead for each ID in idlist, first check whether it already exists
				TreeNode * firstChild = root->childListStart;
				firstChild = firstChild->siblingNext;

				if(checkExistenceIDLIST(symbolId, firstChild, currentScopeNode->scope))  //check only in current scope
				{
				// for each ID in idList, insert in hashtable using datatype - handle both arrays and normal nums (is bound checking done?)
					insertIDLISTHash(symbolId, firstChild, firstChild->siblingNext->siblingNext, currentScopeNode);					
				}

				break;
				}


			/******************* conditional statement ********************/
			case nt_conditionalStmt:

				{
				// check ID in switch
				TreeNode* idVal = root->childListStart->siblingNext->siblingNext;
				if(checkIdInScope(symbolId, idVal->tokenInfo.identifier, currentScopeNode)==NULL)
					printf("Error at line %d: %s is not in scope\n",idVal->tokenInfo.lineNo, idVal->tokenInfo.identifier );
					//START
				populateSymbolTableID(idVal->siblingNext->siblingNext);
				//recursively analyse case statements 
				populateSymbolTableID(idVal->siblingNext->siblingNext->siblingNext);

				//and default
				populateSymbolTableID(root->childListEnd->siblingPrev);


					//END
				populateSymbolTableID(root->childListEnd);

				break;

				}

			case nt_caseStmts:
				
				{
				//increase scope
				
				//recursively for statements
				populateSymbolTableID(root->childListStart->siblingNext->siblingNext->siblingNext);


				//recursively add statements


				//exit scope

				//recursively add N9
				populateSymbolTableID(root->childListEnd);

				break;

				}

			case nt_N9: //multicase

				{

				if(root->childListStart->allenum == EPSILON)
					;
				else
				{

				//recursively for statements
					populateSymbolTableID(root->childListStart->siblingNext->siblingNext->siblingNext);

				//exit scope
				//recursively add N9
				populateSymbolTableID(root->childListEnd);

				}
				
				break;

				}
			case nt_default:

				{
				//increase scope if not epsilon
				//recursively add statements if not epsilon
				if(root->childListStart->allenum == EPSILON)
					;
				else
				{

				//recursively for statements
					populateSymbolTableID(root->childListStart->siblingNext->siblingNext);
				}
				
				break;

				}

//************ Code up Iterative
				case nt_iterativeStmt:
				{
					TreeNode* firstChild = root->childListStart;

					if(firstChild->allenum == FOR)
					{
						TreeNode* idVal = firstChild->siblingNext->siblingNext;
						if(checkIdInScope(symbolId, idVal->tokenInfo.identifier, currentScopeNode)==NULL)
							printf("Error at line %d: %s is not in scope\n",idVal->tokenInfo.lineNo, idVal->tokenInfo.identifier );

						//START
						populateSymbolTableID(idVal->siblingNext->siblingNext->siblingNext->siblingNext);
				
						//recursively for statements
						populateSymbolTableID(root->childListEnd->siblingPrev);

						//exit scope
						populateSymbolTableID(root->childListEnd);

					}
					else
					{
						populateSymbolTableID(root->childListStart->siblingNext->siblingNext);

						//START
						populateSymbolTableID(root->childListStart->siblingNext->siblingNext->siblingNext->siblingNext);

						//recursively for statements
						populateSymbolTableID(root->childListEnd->siblingPrev);

						//exit scope
						populateSymbolTableID(root->childListEnd);
					}

					break;

				}

			default:
				{

				TreeNode* ptr = root->childListStart;

				while(ptr!=NULL)
				{
					populateSymbolTableID(ptr);
					ptr = ptr->siblingNext;
				}

				break;

				}
		}

		
	}


}


void mainOfSymbolTable()
{
	createHashTables();
	createScopeTree();

	populateSymbolTableID(parserTree);
	// populateSymbolTableFunction(parserTree);
}

int main()
{
	mainOfLexer("t2.txt");
	parsing();

	//  FILE* fopenParseTree;
	// fopenParseTree = fopen("outfile.txt", "w+");
	// printInorder(parserTree, fopenParseTree);
	mainOfSymbolTable();

	// idNode* temp = retrieve(symbolId, "bak", 1, hashFunctionSize);

	// if(temp==NULL)
	// 	printf("found");
	// else
	// 	printf("not found");
}