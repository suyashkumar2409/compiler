#include "symbolTable.c"


// tn->allenum == BOOLEAN ||tn->allenum==ID ||tn->allenum==RNUM || 
// 	tn->allenum==NUM || tn->allenum==FALSE || tn->allenum==TRUE || 
// 	tn->allenum == PLUS || tn->allenum == MINUS || tn->allenum == MUL 
// 	|| tn->allenum == DIV || tn->allenum == LT || tn->allenum == LE || 
// 	tn->allenum == GE || tn->allenum == GT || tn->allenum == EQ || tn->allenum == NE;
int isUseful(TreeNode* tn)
{
	return tn->allenum == BOOLEAN ||tn->allenum==ID ||tn->allenum==RNUM || tn->allenum==NUM || tn->allenum==FALSE || tn->allenum==TRUE || tn->allenum == PLUS || tn->allenum == MINUS || tn->allenum == MUL || tn->allenum == DIV || tn->allenum == LT || tn->allenum == LE || tn->allenum == GE || tn->allenum == GT || tn->allenum == EQ || tn->allenum == NE;		
}

void removeDublicateNodes(TreeNode* t)
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
					whereToResumeIteration->childListStart->siblingPrev = whereToResumeIteration->siblingPrev;
				
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
				TreeNode* dummy = whereToResumeIteration->childListStart;
				while(dummy!=NULL)
				{
					dummy->parent = t;
				}
				TreeNode* dum1 = whereToResumeIteration->siblingPrev;
				free(whereToResumeIteration);
				whereToResumeIteration = dum1->siblingNext;	
				continue;		
			}
		}
		else 
		{
			whereToResumeIteration = whereToResumeIteration->siblingNext;
			continue;
		}
	}
}

TreeNode* removeSingleChildNodes(TreeNode* t)
{
	if(t->childListStart != t->childListEnd || t->childListStart==NULL)
		return t;

	return removeSingleChildNodes(t->childListStart);
	
}



void traversal_remove_useless(TreeNode* t)
{
	if(t->childListStart==NULL)
	{
		
		if(!isUseful(t))
		{
			// printf("%s\n", TerminalsAndNonTerminalsList[t->allenum]);
			TreeNode* t1 = t->siblingPrev;
			TreeNode* t2 = t->siblingNext;
			if(t1!=NULL)
				t1->siblingNext = t2;
			else
				t->parent->childListStart = t2;
			if(t2!=NULL)
				t2->siblingPrev = t1;
			else
				t->parent->childListEnd = t->siblingPrev;

			t->parent->childrenNum = t->parent->childrenNum - 1;
			free(t);
			return ;
		}
			
	}
	
	TreeNode* child = t->childListStart;
	while(child!=NULL)
	{
		TreeNode* temp = child;
		child = child->siblingNext;
		traversal_remove_useless(temp);
	}
}

void traversal_remove_duplicate(TreeNode* t)
{
	
	TreeNode* child = t->childListStart;
	while(child!=NULL)
	{
		//traversal_remove_duplicate(child);
		child = child->siblingNext;
	}
	//removeDublicateNodes(t);

}

void traversal_remove_single(TreeNode* t)
{
	TreeNode* toSubstitute = removeSingleChildNodes(t);
	
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
		printf("\n");
	}
}



int main()
{
	mainOfLexer("testcase1.txt");
	parsing();
	mainOfSymbolTable();

	traversal_remove_useless(parserTree);
	// traversal_remove_single(parserTree);
	
	// traversal_remove_duplicate(parserTree);
	printf("\n");
	preOrder(parserTree);
	
}