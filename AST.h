// Batch Number 83
// Suyash Kumar 2014A7PS053P
// Siddharth Agarwal 2014A7PS025P
// #include "parser.h"

typedef struct treenode{
	rule_node ruleNode;
	TokenInfo tokenInfo;
	struct treenode* childListStart;
	struct treenode* childListEnd;
	struct treenode* siblingNext;
	struct treenode* siblingPrev;
	int childrenNum; 
	struct treenode* parent;
} Treenode;

Treenode* newNodeTree(rule_node ruleNode);
void addChild(Treenode* parent, Treenode* child);
void printInorder(Treenode* parent, FILE* filePtrTree);
void printRule(rule_node ruleNode, TokenInfo tokeninfo, Treenode* parent, FILE* filePtrTree);

rule_node newRuleTerminal(allEnum t);
rule_node newRuleNonTerminal(allEnum nt);
