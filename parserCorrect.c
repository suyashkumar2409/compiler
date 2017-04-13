// Batch Number 83
// Suyash Kumar 2014A7PS053P
// Siddharth Agarwal 2014A7PS025P
#include <stdio.h>
#include "lexer.c"
#define no_term 58
#define no_nonterm 51
#define no_rules 95

int siddharthRan = 0;
// typedef enum{
// 	sAND, sARRAY, sBOOLEAN, sBREAK, sCASE, sDECLARE, sDEFAULT, sDRIVER, sEND, sFOR, sGET_VALUE,
// 	sIN, sINPUT, sINTEGER, sMODULE, sOF, sOR, sPARAMETERS, sPRINT, sPROGRAM, sREAL, sRETURNS, sSTART,
// 	sSWITCH, sTAKES, sUSE, sWHILE, sWITH, sFALSE, sTRUE, sPLUS, sMINUS, sMUL, sDIV, sLT, sLE, sGE, sGT, sEQ, sNE,
// 	sDRIVERDEF, sDEF, sDRIVERENDDEF,sENDDEF, sCOLON, sRANGEOP, sSEMICOL, sCOMMA, sASSIGNOP, sSQBO, sSQBC, sBO, sBC, sEPSILON, s$, sID, sNUM, sRNUM
// } Terminal;

// typedef enum
// {
// 	snt_program,snt_moduleDeclaration,snt_moduleDeclarations,snt_otherModules,snt_driverModule,snt_module,snt_ret,snt_input_plist,
// 	snt_IP_List,snt_output_plist,snt_OP_List,snt_dataType,snt_type,snt_moduleDef,snt_statements,snt_statement,snt_ioStmt,
// 	snt_var,snt_whichId,snt_simpleStmt,snt_assignmentStmt,snt_whichStmt,snt_lvalueIDStmt,snt_lvalueARRStmt,snt_index,
// 	snt_moduleReuseStmt,snt_optional,snt_idList,snt_N3,snt_expression,snt_EXP,snt_arithmeticOrBooleanExpr,snt_N7,snt_AnyTerm,snt_N8,snt_arithmeticExpr,
// 	snt_N4,snt_term,snt_N5,snt_factor,snt_op1,snt_op2,snt_logicalOp,snt_relationalOp,snt_declareStmt,snt_conditionalStmt,
// 	snt_caseStmts,snt_N9,snt_value,snt_default,snt_iterativeStmt,snt_range
// } NonTerminal;

const char* non_term_array[] = {
	"program","moduleDeclaration","moduleDeclarations","otherModules","driverModule","module","ret","input_plist",
	"IP_List","output_plist","OP_List","dataType","type","moduleDef","statements","statement","ioStmt",
	"var","whichId","simpleStmt","assignmentStmt","whichStmt","lvalueIDStmt","lvalueARRStmt","index",
	"moduleReuseStmt","optional","idList","N3","expression","arithmeticOrBooleanExpr","N7","AnyTerm","N8",
	"arithmeticExpr","N4","term","N5","factor","op1","op2","logicalOp","relationalOp","declareStmt",
	"conditionalStmt", "caseStmts","N9","value","default","iterativeStmt","range",};

// const char* TerminalsListCaps[] = 
// {
// 	"AND", "ARRAY", "BOOLEAN", "BREAK", "CASE", "DECLARE", "DEFAULT", "DRIVER", "END", "FOR", "GET_VALUE",
// 	"IN", "INPUT", "INTEGER", "MODULE", "OF", "OR", "PARAMETERS", "PRINT", "PROGRAM", "REAL", "RETURNS", "START",
// 	"SWITCH", "TAKES", "USE", "WHILE", "WITH", "FALSE", "TRUE", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE",
// 	"DRIVERDEF", "DEF","DRIVERENDDEF", "ENDDEF", "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "EPSILON","$", "ID", "NUM", "RNUM",
// };	 

struct rnode
{
	allEnum allenum;
	struct rnode* next;
	struct rnode* prev;
	int isterminal;
};
typedef struct rnode RuleNode;

struct rhead
{
	RuleNode* first;
	RuleNode* last;
};
typedef struct rhead RuleHead;

struct fnode
{
	struct fnode* next;
	allEnum allenum;
};
typedef struct fnode FirstNode;

struct fhead
{
	FirstNode* first;
	FirstNode* last;
	// int isEmpty;
};
typedef struct fhead FirstHead;

struct folnode
{
	struct folnode* next;
	allEnum allenum;
};
typedef struct folnode FollowNode;

struct folhead
{
	FollowNode* first;
	FollowNode* last;

};
typedef struct folhead FollowHead;



RuleHead table[no_nonterm][no_term];

FirstHead* firstArray;
FollowHead* followArray;
RuleHead* G;

void add_follow(allEnum nt, allEnum t)
{
	int index = (int)(nt - ntOffset);											///getting the corresponding index of a nonterminal
	FollowHead* ft=&followArray[index];
	FollowNode* add = (FollowNode*)malloc(sizeof(FollowNode));
	add->next = NULL;
	add->allenum = t;
	if(ft->first == NULL)
	{
		ft->first=add;
		ft->last = add;
	}
	else
	{
		FollowNode* temp = ft->last;
		temp->next = add;
		ft->last = add;
	}
	// FollowNode* dummy = ft->first;
	// while(dummy->next!=NULL)
	// {
	// 	dummy=dummy->next;
	// }
	// dummy->next = add;
}

void add_first(allEnum nt, allEnum t)
{
	int index = (int)(nt - ntOffset);
	//printf("%d %d\n", nt,t);											///getting the corresponding index of a nonterminal
	FirstHead* ft=&firstArray[index];
	FirstNode* add = (FirstNode*)malloc(sizeof(FirstNode));
	add->next = NULL;
	add->allenum = t;
	if(ft->first == NULL)
	{
		ft->first=add;
		ft->last = add;
		//printf("%d\n", ft->first->t);
	}
	else
	{
		FirstNode* temp = ft->last;
		temp->next = add;
		ft->last = add;
	}
	// FirstNode* dummy = ft->first;
	// while(dummy->next!=NULL)
	// {
	// 	dummy=dummy->next;
	// }
	// dummy->next = add;
	//printf("%d\n",add->t);

}

void print_rule(RuleHead rh)
{
	RuleNode* rn = rh.first;
	if(rn==NULL)
		printf("NULL");
	while(rn!=NULL)
	{
		if(rn->isterminal)
			printf("%s ", TerminalsListCaps[rn->allenum]);
		else 
			printf("%s ", non_term_array[rn->allenum - ntOffset]);
		rn = rn->next;
	}
}

void add_to_table(allEnum nt,allEnum t, RuleHead rh)
{
	table[nt - ntOffset][t] = rh;
}

int first_contains_e(FirstHead* fh)
{
	FirstNode* fn = fh->first;
	while(fn!=NULL)
	{
		//printf("%s\n", TerminalsListCaps[fn->t]);
		if(fn->allenum == EPSILON)
		{
			return 1;
		}
		fn = fn->next;
	}
	return 0;
}

void add_follow_to_table(RuleHead *rh)
{
	allEnum nt = rh->first->allenum;
	FollowHead ft = followArray[nt - ntOffset];																	/////enum to int/////////////////////
	FollowNode* fn = ft.first;
	while(fn!=NULL)
	{
		 // if(nt == snt_moduleDeclarations)
		 // {
		 // 	print_rule(*rh);
		 // }
		allEnum t = fn->allenum;
		
		add_to_table(nt,t,*rh);
		fn = fn->next ;
	}
}

// int add_first_to_table(RuleHead* rh)
// {
//     RuleNode* rf = rh->first;
// 	RuleNode* rn = rh->first->next;
// 	if(rn->isterminal)
// 	{
// 		if(rn->t != sEPSILON)
// 			add_to_table(rf->nt,rn->t,*rh);
// 		else
// 			add_follow_to_table(rh);
// 		return 0;
// 	}
// 	FirstHead fh = firstArray[rn->nt];
// 	int flag =0;
// 	while(rn!=NULL && (rn->isterminal==0))													/////////enum to int////
// 	{
// 		fh = firstArray[rn->nt];
// 		FirstNode* fn = fh.first;
// 		while(fn!=NULL)
// 		{
// 			flag = 0;
// 			if(fn->t != EPSILON)
// 			{
// 				// printf("SDFHJKL");
// 				// printf("%s %s\n",non_term_array[rh->first->nt],TerminalsListCaps[fn->t]);
// 				add_to_table(rh->first->nt,fn->t,*rh);
// 			}
// 			else
// 				flag = 1;
// 			fn = fn->next ;
// 		}
// 		if(flag = 0)
// 			return 0;
// 		rn = rn->next;
// 	}
// 	if(rn == NULL)
// 	{
// 		return flag;
// 	}
// 	if(rn->isterminal == 1)
// 	{
// 		//printf("%s %s\n",non_term_array[rh->first->nt],TerminalsListCaps[rn->t]);
// 		if(rn->t!=EPSILON)
// 			add_to_table(rh->first->nt,rn->t,*rh);
// 	}

// 	while(rn!=NULL && (rn->isterminal==0))													/////////enum to int////
// 	{
// 		fh = firstArray[rn->nt];
// 		FirstNode* fn = fh.first;
// 		while(fn!=NULL)
// 		{
// 			flag = 0;
// 			if(fn->t != sEPSILON)
// 			{
// 				add_to_table(rh->first->nt,fn->t,*rh);
// 			}
// 			fn = fn->next ;
// 		}
// 		if(!first_contains_e(&firstArray[rn->nt]))
// 			return 0;
// 		rn = rn->next;
// 	}
// 	//if(first_contains_e(&firstArray[rh->first->nt]))
// 	//{
// 	if(rn == NULL)
// 		add_follow_to_table(rh);
// 	if(rn->isterminal)
// 	{
// 		add_to_table(rh->first->nt,rn->t,*rh);
// 	}
// 	//}
// 	return 0;
// }




/******** getindex(non-terminal/terminal) gets the index of the same in which to store in the parse table ************/
/******** FirstHead get_first_head GETS  the head of the linked list that contains the first of the element ************/
/******** firstArray contins the nodes of the linked list which has the firt of that non terminal ************/

void populate_table()	/////////define G as an array of rules 'rule'
{
	int i=0;
	for(i=0;i<no_rules;i++)
	{
		RuleHead r=G[i];
		//print_rule(r);
		//printf("\n");
		RuleNode* X = r.first;
		RuleNode* rn= X->next;
		int flag=0;
		while(rn != NULL)
		{
			if(rn->isterminal)
			{
				if(rn->allenum == EPSILON)
				{
					rn = rn->next;
					continue;
				}
				else
					add_to_table(X->allenum,rn->allenum,r);
				flag = 1;
				break;
			}
			else
			{
				FirstHead fh = firstArray[rn->allenum - ntOffset];
				FirstNode* fn = fh.first;
				while(fn!=NULL)
				{
					if(fn->allenum != EPSILON)
					{
						add_to_table(X->allenum,fn->allenum,r);
					}
					fn = fn->next;
				}
				if(first_contains_e(&fh)==0)
				{
					flag =1;
					break;
				}
				rn = rn->next;
			}
		}
		if(flag==0)
		{
		// 	if(r.first->nt == nt_moduleDeclarations)
		// printf("%s\n", TerminalsListCaps[r.first->next->t]);	
		add_follow_to_table(&r);
		}
	}
}


//shitty code here, correct it
void insertIntoRule(RuleHead* r, allEnum allenum, int isterminal)
{
	RuleNode* rn = (RuleNode*)malloc(sizeof(RuleNode));
	rn->next = NULL;
	rn->prev = NULL;
	rn ->allenum = allenum;
	rn->isterminal=isterminal;
	
	if(r->first == NULL)
	{
		r->first = rn;
		r->last = rn;
	}
	else
	{
		RuleNode* temp = r->last;
		temp->next = rn;
		rn->prev = temp;
		r->last = rn;
	}
}


void insert_into_rule_terminal(RuleHead* r, allEnum t)
{
	insertIntoRule(r, t, 1);
}


void insert_into_rule_nonterminal(RuleHead* r, allEnum nt)
{
	insertIntoRule(r, nt, 0);
}

int nonterm_contains(char* str)
{
	int i=0;
	for(;i<no_nonterm;i++)
	{
		if(!strcmp(str,non_term_array[i]))
			return i + ntOffset;
	}
	return -1;
}

int term_contains(char* str)
{
	int i=0;
	for(;i<no_term;i++)
	{
		if(!strcmp(str,TerminalsListCaps[i]))
			return i;
	}
	return -1;
}

void get_grammar()
{
	FILE *gram_file = fopen("grammar.txt", "r");
	int i = 0;
	char* str = (char*)malloc(100 * sizeof(char));
	while(!feof(gram_file))
	{
		
		fscanf(gram_file,"%s",str);
		//printf("%s ", str);	
		if(strcmp(str,".") == 0)
		{
			//printf("\n");
			i++;
		}
		else
		{

			int a =term_contains(str);
			int b = nonterm_contains(str);
			if(a!=-1)
			{
				insert_into_rule_terminal(&G[i],(allEnum)a);
			}
			else if(b!=-1)
			{
				insert_into_rule_nonterminal(&G[i],(allEnum)b);
			}
			else
			{
				printf("%s\n", str);
				printf("ERROR");
			}	
		}

	}

}

/*void generate_first(allEnum nt)
{

	if(firstArray[n1].first!=NULL)
		return;

	int i=0;
	for(;i<94;i++)
	{
		RuleNode* nt_node = G[i].first;
		if(nt_node->nt == nt)
		{
			RuleNode* rn1 = nt_node->next;				/// taking the first right node
			if(rn1->isterminal==1)
			{
				if(rn1->t ==EPSILON)
					firstArray[rn1->nt].isEmpty = 1;
				else
					add_first_to_table(nt,rn1->t);
			}



			generate_first(rn1->nt);
			FirstHead fh=firstArray[nt];
			if(fh.first == NULL)
			{
				fh.first = firstArray[rn1->nt].first;
			}
			else
			{
				FirstNode* fn = fh.first;
				while(fn->next!=NULL)
				{
					fn=fn->next;
				}
				fn->next = firstArray[rn1->nt].first;
			}
			while(1)
			{

			}
		}
	}
}*/

void get_first()
{
	FILE* fil = fopen("first3.txt","r");
	char str[100];
	while(!feof(fil))
	{
		fscanf(fil,"%s",str);
		//printf("%s ", str);
		int a = nonterm_contains(str);
		while(1)
		{
			fscanf(fil,"%s",str);
			//printf("%s ", str);
			if(strcmp(str,".")==0)
			{
				//printf("\n");
				break;
			}
			else
			{
				int b = term_contains(str);
				add_first((allEnum)a,(allEnum)b);
			}
		}
	}
}

void get_follow()
{
	FILE* fil = fopen("follow3.txt","r");
	char str[100];
	while(!feof(fil))
	{
		fscanf(fil,"%s",str);
		//printf("%s ", str);
		int a = nonterm_contains(str);
		while(1)
		{
			fscanf(fil,"%s",str);
		//	printf("%s ", str);
			if(strcmp(str,".")==0)
			{
		//		printf("\n");
				break;
			}
			else
			{
				int b = term_contains(str);
				add_follow((allEnum)a,(allEnum)b);
			}
		}
	}
}

// void markEmpty()
// {
// 	int i=0;
// 	for(;i<94;i++)
// 	{
// 		if( G[i].first->next->isterminal==1 && G[i].first->next->t == sEPSILON)
// 			firstArray[G[i].first->nt].isEmpty = 1 ;
// 	}
// }

// void get_Last()
// {
// 	int i=0;
// 	for(;i<94;i++)
// 	{
// 		RuleHead r = G[i];
// 		RuleNode* rn = G[i].first;
// 		while(rn->next!=NULL)
// 		{
// 			rn = rn->next;
// 		}
// 		r.last = rn;
// 	}
// }

void parserSiddharth()
{

	G = (RuleHead*)malloc(96 * sizeof(RuleHead));
	firstArray = (FirstHead*)malloc(sizeof(FirstHead)* no_nonterm);

	followArray = (FollowHead*)malloc(sizeof(FollowHead)* no_nonterm);
	int i=0;
	for(i=0;i<96;i++)
	{
		G[i].first = NULL;
	}
	for(i=0;i<no_nonterm;i++)
	{
		firstArray[i].first = NULL;
		followArray[i].first = NULL;

	}
    get_grammar();
    // get_Last();  // this isnt needed - this will happen when you correct the shitty insert into terminal code
    // markEmpty(); // this isnt needed

  get_first();          // simple enough - do a complete rewrite
     get_follow();

    int j;
    int k;
    for(j=0;j<no_nonterm;j++)
    {
    	for(k=0;k<no_term;k++)
    	{
    		table[j][k].first=NULL;
    	}
    }
    populate_table();        
    // printf("%s",TerminalsAndNonTerminalsList[followArray[3].first->next->allenum]);
    // print_rule(table[nt_program - ntOffset][DEF]);

    // siddharthRan = 1;
}

// int main()
// {
// 	parserSiddharth();
// }