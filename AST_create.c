#include "symbolTable.c"

int isUseful(TreeNode* tn)
{
	switch(tn->allenum)
	{
		case INTEGER:return 1;
		case REAL:return 1;
		case BOOLEAN:return 1;
		case TRUE:return 1;
		case FALSE:return 1;
		case AND:return 1;
		case OR:return 1;
		case PLUS:return 1;
		case MINUS:return 1;
		case MUL:return 1;
		case DIV:return 1;
		case LT:return 1;
		case LE:return 1;
		case GE:return 1;
		case GT:return 1;
		case EQ:return 1;
		case NE:return 1;
		case NUM:return 1;
		case RNUM:return 1;
		case ID: return 1;
		case FOR: return 1;
		case WHILE:
		case DEFAULT: return 1;
		default : return 0;
	}
}

int isOperator(TreeNode* t)
{
	switch(t->allenum)
	{
		case AND:
		case OR:
		case PLUS:
		case MINUS:
		case DIV:
		case MUL:
		case LT:
		case LE:
		case GT:
		case GE:
		case EQ:
		case NE: return 1;
		default : return 0;
	}
}

int isCollapsible(TreeNode* t)
{

}
void removeDuplicateNodes(TreeNode* t)
{
	TreeNode* child = t->childListStart;
	TreeNode* whereToResumeIteration = child;
	while(whereToResumeIteration!=NULL)
	{
		if(whereToResumeIteration->allenum == t->allenum)
		{
			int noOfChildrenToAdd = whereToResumeIteration->childrenNum;
			if(noOfChildrenToAdd != 0)
			{
				t->childrenNum+=(noOfChildrenToAdd-1);
				if(whereToResumeIteration->childListStart!=NULL)
				{
					whereToResumeIteration->childListStart->siblingPrev = whereToResumeIteration->siblingPrev;
				}
				
				if(whereToResumeIteration->siblingPrev != NULL)
				{
					whereToResumeIteration->siblingPrev->siblingNext = whereToResumeIteration->childListStart;
				}
				else
				{
					t->childListStart = whereToResumeIteration->childListStart;
				}
				if(whereToResumeIteration->childListEnd!=NULL)
					whereToResumeIteration->childListEnd->siblingNext = whereToResumeIteration->siblingNext;
				
				if(whereToResumeIteration->siblingNext != NULL)
				{
					whereToResumeIteration->siblingNext->siblingPrev = whereToResumeIteration->childListEnd;
				}
				else
				{
					t->childListEnd = whereToResumeIteration->childListEnd;
				}
				TreeNode* dummy = whereToResumeIteration->childListStart;
				while(dummy!=NULL)
				{
					dummy->parent = t;
					dummy = dummy->siblingNext;
				}
				whereToResumeIteration = whereToResumeIteration->childListStart;
			}
			else
			{
				TreeNode* prev = whereToResumeIteration->siblingPrev;
				TreeNode* next = whereToResumeIteration->siblingNext;
				if(prev!=NULL)
				{
					prev->siblingNext = next;
				}
				else
				{
					whereToResumeIteration->parent->childListStart = next;
				}
				if(next!=NULL)
				{
					next->siblingPrev = prev;
				}
				else
				{
					whereToResumeIteration->childListEnd = prev;
				}
				whereToResumeIteration->parent->childrenNum--;
				whereToResumeIteration = next;
			}
		}
		else 
		{
			
			whereToResumeIteration = whereToResumeIteration->siblingNext;
			
		}
	}
}


TreeNode* removeSingleChildNodes(TreeNode* t)
{
	if(t->childListStart == NULL || (t->childListStart != t->childListEnd))
		return t;

	return removeSingleChildNodes(t->childListStart);
	
}



void traversal_remove_useless(TreeNode* t)
{
	if(t->childListStart == NULL)
	{
		
		if(!isUseful(t))
		{
			//printf("%d\n", t->allenum);
			TreeNode* t1 = t->siblingPrev;
			TreeNode* t2 = t->siblingNext;
			if(t1!=NULL)
				t1->siblingNext = t2;
			else
				t->parent->childListStart = t->siblingNext;
			if(t2!=NULL)
				t2->siblingPrev = t1;
			else
				t->parent->childListEnd = t->siblingPrev;

			t->parent->childrenNum--;
			free(t);
			return ;
		}
			
	}
	
	TreeNode* child = t->childListStart;
	while(child!=NULL)
	{
		traversal_remove_useless(child);
		child = child->siblingNext;
	}
}

void traversal_remove_duplicate(TreeNode* t)
{
	
	TreeNode* child = t->childListStart;
	while(child!=NULL)
	{
		traversal_remove_duplicate(child);
		child = child->siblingNext;
	}
	removeDuplicateNodes(t);

}

void traversal_remove_single(TreeNode* t)
{
	TreeNode* toSubstitute = removeSingleChildNodes(t);
	if(toSubstitute->allenum != t->allenum)
	{
		//printf("%s\n", TerminalsAndNonTerminalsList[toSubstitute->allenum]);
		TreeNode* t1 = t->siblingPrev;
		TreeNode* t2 = t->siblingNext;

		if(t1!=NULL)
		{
			t1->siblingNext = toSubstitute;
			toSubstitute->siblingPrev = t1;
		}	
		else
		{
			t->parent->childListStart = toSubstitute;
			toSubstitute->siblingPrev = NULL;
		}	
		if(t2!=NULL)
		{
			//printf("%s %s\n", TerminalsAndNonTerminalsList[t->allenum], TerminalsAndNonTerminalsList[toSubstitute->allenum]);
			t2->siblingPrev = toSubstitute;
			toSubstitute->siblingNext = t2;
		}	
		else
		{
			t->parent->childListEnd = toSubstitute;
			toSubstitute->siblingNext = NULL;
		}

	}
	
	TreeNode* child = t->childListStart;
	while(child!=NULL)
	{
		traversal_remove_single(child);
		child = child->siblingNext;
	}
}
void preOrder(TreeNode* t)
{
	if(t!=NULL)
	{
		printf("%s\n", TerminalsAndNonTerminalsList[t->allenum]);
		TreeNode* child = t->childListStart;
		while(child!=NULL)
		{
			preOrder(child);
			child = child->siblingNext;
		}
	}
}

void removeNullNodesTraversal(TreeNode* t)
{
	
	TreeNode* child = t->childListStart;
	while(child!=NULL)
	{
		removeNullNodesTraversal(child);
		child = child->siblingNext;
	}
	if(t->childListStart == NULL)
	{
		TreeNode* t1 = t->siblingPrev;
			TreeNode* t2 = t->siblingNext;
			if(t1!=NULL)
				t1->siblingNext = t2;
			else
				t->parent->childListStart = t->siblingNext;
			if(t2!=NULL)
				t2->siblingPrev = t1;
			else
				t->parent->childListEnd = t->siblingPrev;

			t->parent->childrenNum--;
			return ;
	}
}


void mainOfASTcreate()
{
	traversal_remove_useless(parserTree);
	// traversal_remove_single(parserTree);
	// traversal_remove_duplicate(parserTree);
	// removeNullNodesTraversal(parserTree);

	// printf("\n");
	//  preOrder(parserTree);
}

