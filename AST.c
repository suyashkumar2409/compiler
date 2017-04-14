// Batch Number 83
// Suyash Kumar 2014A7PS053P
// Siddharth Agarwal 2014A7PS025P
#include <stdio.h>
#include <stdlib.h>
#include "parserCorrect.c"
// #include "AST.h"
// typedef struct treenode{
// 	rule_node ruleNode;
// 	struct treenode* childListStart;
// 	struct treenode* childListEnd;
// 	struct treenode* siblingNext;
// 	struct treenode* siblingPrev;
// 	int childrenNum; 
// 	struct treenode* parent;
// } TreeNode;

// typedef struct rnodesk
// {
// 	struct rnode* next;
// 	struct rnode* prev;
// 	allEnum tnt;
// 	int isterminal;
// } rule_node_sk;


// typedef struct stacknode{
// 	rule_node_sk ruleNode;
// 	struct stacknode* next;
// } TreeNode;


//usage- stackptr = push(stackptr)


typedef struct treenode{
	int isLeaf;

	TokenInfo tokenInfo;

	allEnum allenum;
	RuleNode ruleNode;


	struct treenode* childListStart;
	struct treenode* childListEnd;
	struct treenode* siblingNext;
	struct treenode* siblingPrev;
	int childrenNum; 
	struct treenode* parent;

	struct treenode* next;
} TreeNode;

void copy(TreeNode* a, TreeNode* b)
{
	a->isLeaf = b->isLeaf;

	a->tokenInfo = b->tokenInfo;

	a->allenum = b->allenum;

	a->ruleNode = b->ruleNode;

	a->childListStart = b->childListStart;

	a->childListEnd = b->childListEnd;

	a->siblingNext = b->siblingNext;

	a->siblingPrev = b->siblingPrev;

	a->childrenNum = b->childrenNum;

	a->parent = b->parent;

	a->next = b->next;	
}


TreeNode* top(TreeNode* root)
{
	return root;
}

int isEmptyStack(TreeNode * root)
{
	return root == NULL;
}
TreeNode* pop(TreeNode* root)
{
	if(root != NULL)
	{
		TreeNode * temp = root;
		root = root->next;
		// free(temp);
		temp = NULL;
	}

	return root;
}

// TreeNode* popWithoutFree(TreeNode* root)
// {
// 	if(root != NULL)
// 	{
// 		TreeNode * temp = root;
// 		root = root->next;
// 	}

// 	return root;
// }

TreeNode* push(TreeNode* root, TreeNode* newNode)
{
	newNode->next = root;
	root = newNode;

	return root;
}

// TreeNode* newNode(RuleNode rule)
// {
// 	TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));

// 	rule_node_sk ruleNodesk;


// 	ruleNodesk.next = ruleNode.next;
// 	ruleNodesk.prev = ruleNode.prev;
// 	ruleNodesk.isterminal = ruleNode.isterminal;

// 	if(ruleNode.isterminal)
// 	{
// 		ruleNodesk.tnt = ruleNode.t;
// 	}
// 	else
// 	{
// 		ruleNodesk.tnt = ruleNode.nt + ntOffset;
// 	}


// 	temp->ruleNode = ruleNodesk;
// 	temp->next = NULL;
// 	return temp;
// }

TreeNode* newNodeTree(RuleNode ruleNode, TokenInfo tokenInfo)
{
	TreeNode* node  = (TreeNode*)malloc(sizeof(TreeNode));
	
	node->ruleNode = ruleNode;
	node->tokenInfo = tokenInfo;
	node->childListStart = NULL;
	node->childListEnd = NULL;
	node->siblingNext = NULL;
	node->siblingPrev = NULL;
	node->childrenNum = 0;
	node->parent = NULL;

	return node;
}


// TreeNode* newNodeTreeSK(rule_node_sk ruleNode, TokenInfo tokenInfo)
// {
// 	TreeNode* node  = (TreeNode*)malloc(sizeof(TreeNode));
	

// 	node->ruleNode = ruleNode;
// 	node->tokenInfo = tokenInfo;
// 	node->childListStart = NULL;
// 	node->childListEnd = NULL;
// 	node->siblingNext = NULL;
// 	node->siblingPrev = NULL;
// 	node->childrenNum = 0;
// 	node->parent = NULL;

// }
void addChild(TreeNode* parent, TreeNode* child)
{
	parent->childrenNum = parent->childrenNum + 1;
	if(parent->childListStart == NULL)
	{
		parent->childListStart = child;
		parent->childListEnd = child;
		child->parent = parent;
	}
	else
	{
		parent->childListEnd->siblingNext = child;
		child->siblingPrev = parent->childListEnd;
		child->parent = parent;
		parent->childListEnd = child;
	}
}

void addChildEnd(TreeNode* parent, TreeNode* child)
{
	parent->childrenNum = parent->childrenNum + 1;
	if(parent->childListEnd == NULL)
	{
		parent->childListStart = child;
		parent->childListEnd = child;
		child->parent = parent;
	}
	else
	{
		parent->childListStart->siblingPrev = child;
		child->siblingNext = parent->childListStart;
		child->parent = parent;
		parent->childListStart = child;
	}
}

 // lexemeCurrentNode     lineno   token   valueIfNumber   parentNodeSymbol   isLeafNode(yes/no)    NodeSymbol


void printRule(RuleNode ruleNode, TokenInfo tokenInfo,TreeNode* parent, FILE * filePtrTree,int isLeaf)
{
	char parentSymbol[100];

	if(parent!=NULL)
		strcpy(parentSymbol ,TerminalsAndNonTerminalsList[((parent->tokenInfo).allenum)]);
	else
		strcpy(parentSymbol ,"ROOT");

	if(isLeaf)
	{
		if(tokenInfo.type == 1)
			fprintf(filePtrTree, "%s %d %s %s yes\n", tokenInfo.identifier, tokenInfo.lineNo,
			 TerminalsList[tokenInfo.allenum], parentSymbol);
		else if(tokenInfo.type == 2)
			fprintf(filePtrTree, "%d %d NUM %d %s yes\n", tokenInfo.integer, tokenInfo.lineNo,
			 tokenInfo.integer, parentSymbol);
		else if(tokenInfo.type == 3)
			fprintf(filePtrTree, "%f %d RNUM %f %s yes\n", tokenInfo.rnum, tokenInfo.lineNo,
			 tokenInfo.rnum, parentSymbol);
		else if(tokenInfo.type == 0 || tokenInfo.type == 4)
		{
			char * tempUpper = uppercase(TerminalsList[tokenInfo.allenum]);
			fprintf(filePtrTree, "%s %d %s %s yes\n", tempUpper, tokenInfo.lineNo, 
				TerminalsList[tokenInfo.allenum], parentSymbol);

			free(tempUpper);
		}
		else if(tokenInfo.type == 5)
		{
			fprintf(filePtrTree, "EPSILON  %s yes\n", parentSymbol);
		}
	}
	else
	{
		// confirm - what does token mean for non terminals
		fprintf(filePtrTree,"---- %d %s %s no %s\n", tokenInfo.lineNo, 
			TerminalsAndNonTerminalsList[tokenInfo.allenum], parentSymbol, 
			TerminalsAndNonTerminalsList[tokenInfo.allenum]);
	}
}




// RuleNode newRuleTerminal(allenum t)
// {
// 	RuleNode rule;
// 	rule.allenum = t;
// 	rule.next = NULL;
// 	rule.isterminal = 1;

// 	return rule;
// }
RuleNode newRuleNonTerminal(allEnum nt)
{

	RuleNode rule;
	rule.allenum = nt;
	rule.next = NULL;
	rule.isterminal = 0;

	return rule;	
}

TreeNode* transferStack(TreeNode* a, TreeNode* b)
{
	TreeNode* temp = b->next;
	b->next = a;
	a = b;
	b = temp;
	while(temp!=NULL)
	{
		temp = b->next;
		b->next = a;
		a = b;
		b = temp;
	}

	return a;

}