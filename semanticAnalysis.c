#include "AST_create.c"

void compareIDListWithExpected(TreeNode* idList, argument* argumentList, int lineNo, char* funcName)
{
	argument* currArg = argumentList;
	while(idList->childListStart != NULL && currArg != NULL)
	{
		TreeNode* compareId = idList->childListStart;

		//compareId and currArg
		allEnum type1;
		allEnum type2;

		if(compareId->entry->var.isVariable)
		{
			type1 = compareId->entry->var.variable.type;
			if(currArg->isVariable)
			{
				type2 = currArg->variable.type;

				if(type1 != type2)
					printf("Error at line %d: arguments of the invoked function %s do not match with definition\n", lineNo, funcName);
			}
			else
			{
				printf("Error at line %d: arguments of the invoked function %s do not match with definition\n", lineNo, funcName);
			}
		}
		else
		{
			type1 = compareId->entry->var.array.type;
			if(currArg->isVariable)
			{
				printf("Error at line %d: arguments of the invoked function %s do not match with definition\n", lineNo, funcName);
			}
			else
			{
				type2 = currArg->array.type;

				if(type1 != type2 || compareId->entry->var.array.rangeStart != currArg->array.rangeStart || compareId->entry->var.array.rangeEnd != currArg->array.rangeEnd)
				{
					printf("Error at line %d: arguments of the invoked function %s do not match with definition\n", lineNo, funcName);
				}
			}
		}

		idList = idList->childListEnd;
		currArg = currArg->next;
	}	

	if(idList->childListStart!=NULL)
	{
		printf("Error at line %d: arguments of the invoked function %s do not match with definition\n", lineNo, funcName);
	}

	if(currArg != NULL)
	{
		printf("Error at line %d: arguments of the invoked function %s do not match with definition\n", lineNo, funcName);
	}
}
void semanticAnalysis(TreeNode* root)
{
	if(root!=NULL)
	{
		switch(root->allenum)
		{
			//DONT FORGET THAT U ARE USING REDUCED AST NOT PARSE TREE
			case nt_ret:
			{
				//for each output list check whether its assigned
				if(root->childListStart != NULL)
				{
					TreeNode* outputList = root->childListStart;
					idNode* idEnt = outputList->childListStart->entry;
					if(idEnt->valueAssigned == 0)
					{
						printf("Error at line %d: return variable %s has not been assigned a value\n",idEnt->lineNo, idEnt->ID);
					}

					TreeNode * temp = outputList->childListEnd;
					while(temp->childListStart != NULL)
					{
						idEnt = temp->childListStart->entry;

						if(idEnt->valueAssigned == 0)
						{
							printf("Error at line %d: return variable %s has not been assigned a value\n",idEnt->lineNo, idEnt->ID);
						}
						
						temp = temp->childListEnd;
					}
				}
				break;
			}
			case nt_lvalueIDStmt:
			{
				//CALL TYPE CHECKER FOR EXPRESSION HERE
				break;
			}
			case nt_lvalueARRStmt:
			{
				//CALL TYPE CHECKER FOR EXPRESSION HERE
				break;	
			}
			case nt_moduleReuseStmt:
			{
				//compare expected output list with values
				TreeNode* optional = root->childListStart;
				idNode* idEnt = root->childListStart->siblingNext->entry; // ID
				if(optional->childListStart == NULL)
				{
					if(idEnt->fun.outputArgument!=NULL)
					{
						printf("Error at line %d: invoked function %s's output parameters don't match with definition\n",idEnt->lineNo, idEnt->ID);
					}
				}
				else
				{
					compareIDListWithExpected(optional->childListStart, idEnt->fun.outputArgument,idEnt->lineNo, idEnt->ID);
				}
				//compare expected input list with assigned idList

				compareIDListWithExpected(root->childListEnd, idEnt->fun.inputArgument,idEnt->lineNo, idEnt->ID);
				break;
			}
			case nt_conditionalStmt:
			{
				allEnum type;
				idNode* idEnt = root->childListStart->entry;

				if(idEnt->var.isVariable)
				{
					switch(idEnt->var.variable.type)
					{
						case INTEGER:
							break;
						case REAL:
							printf("Error at line %d: variable %s is of type REAL, can't used as argument in switch statement\n",idEnt->lineNo, idEnt->ID);
							break;
						case BOOLEAN:
							if(root->childListEnd->childListStart!=NULL)
							{
								printf("Error at line %d: Default cannot be used when switch argument is Boolean\n",root->childListEnd->childListStart->tokenInfo.lineNo);
							}
							break;
					}
				//If ID is of type boolean - ensure default does not occur
				//If ID is of type real - it is an error

				//recursively take each case statement and check type

					allEnum switchType = idEnt->var.variable.type;
					TreeNode* caseStmt = root->childListStart->siblingNext;

					while(caseStmt->childListStart!=NULL)
					{
						TreeNode* value = caseStmt->childListStart;

						switch(value->childListStart->tokenInfo.allenum)
						{
							case NUM:
								{
									switch(switchType)
									{
										case INTEGER:
											break;
										case REAL:
										case BOOLEAN:
											printf("Error at line %d: type of case does not match with type of switch\n",value->childListStart->tokenInfo.lineNo);
											break;
									}
									break;
								}
							case TRUE:
							case FALSE:
								{
									switch(switchType)
									{
										case INTEGER:
										case REAL:
											printf("Error at line %d: type of case does not match with type of switch\n",value->childListStart->tokenInfo.lineNo);
											break;
										case BOOLEAN:
											break;
									}
									break;
								}
						}

						caseStmt = caseStmt->childListEnd;
					}

				}
				else
				{
					printf("Error at line %d: array variable %s has been used as argument in switch statement\n",idEnt->lineNo, idEnt->ID);
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

int mainOfSemanticAnalysis()
{
	semanticAnalysis(parserTree);
}
int main()
{
	mainOfLexer("t1.txt");
	parsing();
	mainOfSymbolTable();
	mainOfASTcreate();
	mainOfSemanticAnalysis();
	// printSymbolTable(parserTree);
}