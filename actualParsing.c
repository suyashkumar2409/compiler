// Batch Number 83
// Suyash Kumar 2014A7PS053P
// Siddharth Agarwal 2014A7PS025P
#include <stdio.h>
#include <stdlib.h>
// #include "stack.c"
#include "AST.c"
#define parsingRuleLength 1000
// #include "parser.h"

TreeNode* parserStack = NULL;
TreeNode * parserTree = NULL;

TreeNode* helperStack = NULL;

void correctPrintingFinalTree()
{

}

void initialiseParserSuyash()
{
    // if(!siddharthRan) 
	parserSiddharth();
	// printf("%d")
	
	//adds start symbol to both stack and ast, also adds $ to stack

	// while(top(parserStack)!=NULL)
	// {
	// 	parserStack = pop(parserStack);
	// }

	parserTree = NULL;
	parserStack = NULL;


	//Stack $
	RuleNode ruleNodeEmpty;
	ruleNodeEmpty.allenum = $;
	TokenInfo tokenInfoDollar;
	tokenInfoDollar.allenum = $;

	TreeNode* newNodeDollar = newNodeTree(ruleNodeEmpty, tokenInfoDollar);


	parserStack = push(parserStack, newNodeDollar);

	//Stack S
	RuleNode ruleNodeProgram = newRuleNonTerminal(nt_program);
	TokenInfo tokenInfoEmpty;
	TreeNode* newNodeProgram = newNodeTree(ruleNodeProgram, tokenInfoEmpty);

	parserStack = push(parserStack, newNodeProgram);
}

// void printParseTree(char* filename)
// {
// 	FILE * filePtrTree =  fopen(filename, "w+");
// 	printInorder(parserTree, filePtrTree);
// }

void printStack()
{
	TreeNode* bla = parserStack;
	while(bla!=NULL)
	{
		// printf("stack %s\n",TerminalsAndNonTerminalsList[bla->ruleNode.allenum]);
		bla = bla->next;
	}
}
void parsing()
{
	// printLinkedList();
	initialiseParserSuyash();
	resetPtr();
	TokenInfo * lookAhead = getNextToken();
	// lookAhead = getNextToken();

	// int iter = 0;	
	while(1)	
	{
		TreeNode* topNode = (top(parserStack));
		RuleNode topRule = topNode->ruleNode;

		// printStack();printf
		printf("yo%s\n",TerminalsAndNonTerminalsList[topRule.allenum]);
		// TokenInfo* lookAhead = topNode->tokenInfo;

		//if stack shows $
		if((topRule.isterminal) && (topRule.allenum == (allEnum)$))
		{
			break;
		}

		if(lookAhead == NULL)
		{
			int isError = 0;
			while(1)
			{

				topRule = (top(parserStack))->ruleNode;
		// printf("yow%s\n",TerminalsAndNonTerminalsList[topRule.allenum]);

				//if stack shows $
				if((topRule.isterminal) && (topRule.allenum == (allEnum)$))
				{
					break;
				}	
				
				// if(topRule.tnt == 8)
				// {
				// 	parserStack = pop(parserStack);
				// }
				// else
				//if the stack contains a terminal, when lookahead is over
				if(topRule.allenum < ntOffset)
				{
					isError = 1;
					break;
				}
				else if(first_contains_e(&firstArray[topRule.allenum - ntOffset]))
				{
					TreeNode* makeEpsilon = top(parserStack);
					parserStack = pop(parserStack);


					RuleNode emptyRule;
					TokenInfo epsilonTokenInfo;

					epsilonTokenInfo.allenum = EPSILON;
					emptyRule.allenum = EPSILON;
					epsilonTokenInfo.type = 5;
					TreeNode* epsilonNode = newNodeTree(emptyRule, epsilonTokenInfo);

					// printf("%s",TerminalsAndNonTerminalsList[makeEpsilon->ruleNode.allenum]);

					addChild(makeEpsilon, epsilonNode);
				}
				else
				{
					isError = 1;
					break;
				}

			}

			if(isError)
				printf("**HERE Error because program ended before parsing completed\n");
			else
				break;
		}
		// iter++;

		if(topRule.isterminal)
		{
			//stack top is terminal
			
			//******************need to handle epsilon
			//*************** check out at end
			if(topRule.allenum == EPSILON)
			{
				//Simply add , no checking required
				parserStack = pop(parserStack);
				topNode->isLeaf = 1;
			}
			
			else
			{

				//checking required
				if(lookAhead->allenum == topRule.allenum)
				{
					parserStack = pop(parserStack);
					// find a way to insert it

					topNode->tokenInfo = *lookAhead;
					topNode->isLeaf = 1;
				}
				else
				{
					// printf("Error because terminal doesnt match\n");

					if(lookAhead->type == 1) //lookahead is ID
					{
						printf("ERROR_5: The token ID for lexeme %s does not match at line %d.The expected token here is %s", lookAhead->identifier, 
						 lookAhead->lineNo, TerminalsList[topRule.allenum]);
					}
					else if(lookAhead->type == 2) //lookahead is NUM
					{
						printf("ERROR_5: The token NUM for lexeme %d does not match at line %d.The expected token here is %s", lookAhead->integer, 
						 lookAhead->lineNo, TerminalsList[topRule.allenum]);
					}
					else if(lookAhead->type == 3) //lookahead is RNUM
					{
						printf("ERROR_5: The token RNUM for lexeme %f does not match at line %d.The expected token here is %s", lookAhead->rnum, 
						 lookAhead->lineNo, TerminalsList[topRule.allenum]);
					}
					else if(lookAhead->type == 4 || lookAhead->type==0) //lookahead is token or keyword
					{
						char * tmpUpper = uppercase(TerminalsList[lookAhead->allenum]);
						printf("ERROR_5: The token %s for lexeme %s does not match at line %d.The expected token here is %s", tmpUpper,TerminalsList[lookAhead->allenum], 
						 lookAhead->lineNo, TerminalsList[topRule.allenum]);

						free(tmpUpper);
					}

					printf("\n Thus, parsing is unsuccessful\n");
					break;

				}

				lookAhead = getNextToken();
				// if(lookAhead!=NULL)
				// printf("\nkak%s\n",TerminalsAndNonTerminalsList[lookAhead->allenum]);
			}
		}
		//top is non terminal
		else
		{
			//lookAhead is keyword and rule exists
			if(table[topRule.allenum - ntOffset][lookAhead->allenum].first != NULL)
			{

				//remove top non-terminal

				//My Stack has the nodes of the tree
				TreeNode* parentPopped = top(parserStack); 

				if(parentPopped->parent == NULL)
				{
					//TOP was Start symbol program
					parserTree = parentPopped;
				}
				// printf("%s\n",TerminalsAndNonTerminalsList[topRule.allenum]);

				parserStack = pop(parserStack);
				RuleHead myRule = table[topRule.allenum - ntOffset][lookAhead->allenum];
				// print_rule(myRule);

				//Pushing to stack in reverse order
				RuleNode* head = myRule.first->next;
				RuleNode* tail = myRule.last;

				RuleNode* ptr = tail;



				//Adding to tree
				TokenInfo tokenInfoEmpty;
				while(ptr!=head->prev)
				{

					TreeNode * newPushedNode = newNodeTree(*ptr, tokenInfoEmpty);
					newPushedNode->isLeaf = 0;
					addChildEnd(parentPopped, newPushedNode);
					parserStack = push(parserStack, newPushedNode);
					ptr = ptr->prev;

					// printf("%s\n",TerminalsAndNonTerminalsList[top(helperStack)->ruleNode.allenum]);

				}
// printf("hoolah\n");
				//adding to stack
// while(!isEmptyStack(helperStack))
// 				{
// 					printf("%s\n",TerminalsAndNonTerminalsList[top(helperStack)->ruleNode.allenum]);
// 					TreeNode temp = top(helperStack);

// 					parserStack = push(parserStack, top(helperStack));
// 					printf("%s\n",TerminalsAndNonTerminalsList[top(helperStack)->ruleNode.allenum]);

// 					helperStack = pop(helperStack);
// 				}

				// parserStack = transferStack(parserStack, helperStack);

				// while(!isEmptyStack(helperStack))
				// {
				// 	// TreeNode* tempNode = top(helperStack);
				// 	// TreeNode* toBePushed = (TreeNode*)malloc(sizeof(TreeNode));
				// 	// TreeNode* toBePopped = top(helperStack);

				// 	// copy(toBePushed, toBePopped);

				// 	parserStack = push(parserStack, top(helperStack));
				// 	// printf("%s\n",TerminalsAndNonTerminalsList[tempNode->ruleNode.allenum]);
				// 	helperStack = pop(helperStack);

				// 	// printf("%s\n",TerminalsAndNonTerminalsList[top(parserStack)->ruleNode.allenum]);

				// }

			}
			else /////////******** This is error handling, do at the very last
			{
				// printf("%d %d\n",topRule.tnt, lookAhead->allenum);
				// ************** Do proper error handling
				// printf("Error because rule isnt present\n");
				if(lookAhead->type == 1) //lookahead is ID
				{
					printf("ERROR_5: The token ID for lexeme %s does not match at line %d.The expected token here is %s", lookAhead->identifier, 
					 lookAhead->lineNo, TerminalsList[topRule.allenum]);
				}
				else if(lookAhead->type == 2) //lookahead is NUM
				{
					printf("ERROR_5: The token NUM for lexeme %d does not match at line %d.The expected token here is %s", lookAhead->integer, 
					lookAhead->lineNo, TerminalsList[topRule.allenum]);
				}
				else if(lookAhead->type == 3) //lookahead is RNUM
				{
					printf("ERROR_5: The token RNUM for lexeme %f does not match at line %d.The expected token here is %s", lookAhead->rnum, 
					lookAhead->lineNo, TerminalsList[topRule.allenum]);
				}
				else if(lookAhead->type == 4 || lookAhead->type==0) //lookahead is token or keyword
				{
					char * tmpUpper = uppercase(TerminalsList[lookAhead->allenum]);
					printf("***ERROR_5: The token %s for lexeme %s does not match at line %d.The expected token here is %s", tmpUpper,TerminalsList[lookAhead->allenum], 
					lookAhead->lineNo, TerminalsList[topRule.allenum]);
					free(tmpUpper);
				}

					printf("\n Thus, parsing is unsuccessful\n");
					break;

			}
		}
	}


	// lookAhead = getNextToken();

	if(lookAhead != NULL)
	{
		printf("Error because parsing completed before program ended\n");
	}
	else
	{
		printf("Compilation Successful!");
	}	
}

void printRule2(RuleNode ruleNode, TokenInfo tokenInfo,TreeNode* parent, FILE * filePtrTree,int isLeaf)
{
	if(isLeaf)
	{
		if(ruleNode.allenum == EPSILON)
		{
			printf("EPSILON  leaf\n");
		}
		else
		{
			if(tokenInfo.type == 1)
				printf("ID %s leaf\n", tokenInfo.identifier);
			else if(tokenInfo.type == 2)
				printf("NUM %d leaf\n", tokenInfo.integer);
			else if(tokenInfo.type == 3)
				printf("RNUM %f leaf\n", tokenInfo.rnum);
			else if(tokenInfo.type == 0 || tokenInfo.type == 4)
			{
				char * tempUpper = uppercase(TerminalsList[tokenInfo.allenum]);
				printf("Keyword %s leaf\n",	TerminalsList[tokenInfo.allenum]);

				free(tempUpper);
			}
		}
		
	}
	else
	{
		printf("%s node\n",TerminalsAndNonTerminalsList[ruleNode.allenum]);
	}
}

void printInorder(TreeNode* parent, FILE * filePtrTree)
{
	if(parent != NULL)
	{
		int i = 0;
		TreeNode* child;
		child = parent->childListStart;
		printInorder(child, filePtrTree);
		printRule2(parent->ruleNode, parent->tokenInfo, parent->parent, filePtrTree, parent->isLeaf);

		if(child!=NULL)
			child = child->siblingNext;
		
		while(child!=NULL)
		{
			printInorder(child, filePtrTree);
			child = child->siblingNext;
		}

	}
	// else
	// {
	// 	printf("\n\n\n\n***************** BEHOLD *************************\n\n\n\n\n");
	// }
}

// int main()
// {
// 	mainOfLexer("testcase1.txt");
// 	parsing()	;
// 	FILE* fopenParseTree;
// 	fopenParseTree = fopen("outfile.txt", "w+");
// 	printInorder(parserTree, fopenParseTree);


// }