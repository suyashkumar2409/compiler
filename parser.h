// Batch Number 83
// Suyash Kumar 2014A7PS053P
// Siddharth Agarwal 2014A7PS025P
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#define no_term 58
#define no_nonterm 51
#define no_rules 94

typedef Token Terminal;

typedef enum
{
	nt_program,nt_moduleDeclaration,nt_moduleDeclarations,nt_otherModules,nt_driverModule,nt_module,nt_ret,nt_input_plist,
	nt_IP_List,nt_output_plist,nt_OP_List,nt_dataType,nt_type,nt_moduleDef,nt_statements,nt_statement,nt_ioStmt,
	nt_var,nt_whichId,nt_simpleStmt,nt_assignmentStmt,nt_whichStmt,nt_lvalueIDStmt,nt_lvalueARRStmt,nt_index,
	nt_moduleReuseStmt,nt_optional,nt_idList,nt_N3,nt_expression,nt_EXP,nt_arithmeticOrBooleanExpr,nt_N7,nt_AnyTerm,nt_N8,nt_arithmeticExpr,
	nt_N4,nt_term,nt_N5,nt_factor,nt_op1,nt_op2,nt_logicalOp,nt_relationalOp,nt_declareStmt,nt_conditionalStmt,
	nt_caseStmts,nt_N9,nt_value,nt_default,nt_iterativeStmt,nt_range
} Nonterminal;

struct rnode
{
	struct rnode* next;
	struct rnode* prev;
	Terminal t;
	Nonterminal nt;
	int isterminal;
};
typedef struct rnode rule_node;

struct rhead
{
	rule_node* first;
};
typedef struct rhead rule_head;

struct fnode
{
	struct fnode* next;
	Terminal t;
};
typedef struct fnode first_node;

struct fhead
{
	first_node* first;
	int isEmpty;
};
typedef struct fhead first_head;

struct folnode
{
	struct folnode* next;
	Terminal t;
};
typedef struct folnode follow_node;

struct folhead
{
	follow_node* first;
};
typedef struct folhead follow_head;



rule_head table[no_nonterm][no_term];

first_head* first_array;
follow_head* follow_array;
rule_head* G;

const char* non_term_array[] = {
	"program","moduleDeclaration","moduleDeclarations","otherModules","driverModule","module","ret","input_plist",
	"IP_List","output_plist","OP_List","dataType","type","moduleDef","statements","statement","ioStmt",
	"var","whichId","simpleStmt","assignmentStmt","whichStmt","lvalueIDStmt","lvalueARRStmt","index",
	"moduleReuseStmt","optional","idList","N3","expression","arithmeticOrBooleanExpr","N7","AnyTerm","N8",
	"arithmeticExpr","N4","term","N5","factor","op1","op2","logicalOp","relationalOp","declareStmt",
	"conditionalStmt", "caseStmts","N9","value","default","iterativeStmt","range",};

const char* term_array[] = 
{
	"AND", "ARRAY", "BOOLEAN", "BREAK", "CASE", "DECLARE", "DEFAULT", "DRIVER","DRIVERDEF", "DRIVERENDDEF", "END", "FOR", "GET_VALUE",
	"IN", "INPUT", "INTEGER", "MODULE", "OF", "OR", "PARAMETERS", "PRINT", "PROGRAM", "REAL", "RETURNS", "START",
	"SWITCH", "TAKES", "USE", "WHILE", "WITH", "FALSE", "TRUE", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE",
	"DEF", "ENDDEF", "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "EPSILON","$", "ID", "NUM", "RNUM",
};																																					/////////////SHOULD WE INCLSE COMMENTMARK////////////
/*first_head flist[1000];
follow_head follist[1000];*/
