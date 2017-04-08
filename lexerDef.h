// Batch Number 83
// Suyash Kumar 2014A7PS053P
// Siddharth Agarwal 2014A7PS025P
#include <stdio.h>
#define idLenMax 8
#define keywordLenMax 30
#define keywordUniqueNum 30

#define keywordOffset 0
#define tokenOffset 30
#define epsilonOffset 53
#define IDOffset 55
#define ntOffset 58
typedef enum{
	/**************** Terminals ************************/
	//All keywords
	//Start with 0
	AND, ARRAY, BOOLEAN, BREAK, CASE, DECLARE, DEFAULT, DRIVER, END, FOR, GET_VALUE, 
	IN, INPUT, INTEGER, MODULE, OF, OR, PARAMETERS, PRINT, PROGRAM, REAL, RETURNS, START, 
	SWITCH, TAKES, USE, WHILE, WITH, FALSE, TRUE, 
	//All tokens
	//Start with 30
	PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, 
	DRIVERDEF, DEF, DRIVERENDDEF, ENDDEF,  COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, BO, BC, 
	//Epsilon
	//Start with 53
	EPSILON,$,
	//ID, integer, Real
	//Start with 55
	ID, NUM, RNUM,
	//Start with 58
	/***************** Non Terminals ********************/
	nt_program,nt_moduleDeclaration,nt_moduleDeclarations,nt_otherModules,nt_driverModule,nt_module,nt_ret,nt_input_plist,
	nt_IP_List,nt_output_plist,nt_OP_List,nt_dataType,nt_type,nt_moduleDef,nt_statements,nt_statement,nt_ioStmt,
	nt_var,nt_whichId,nt_simpleStmt,nt_assignmentStmt,nt_whichStmt,nt_lvalueIDStmt,nt_lvalueARRStmt,nt_index,
	nt_moduleReuseStmt,nt_optional,nt_idList,nt_N3,nt_expression,nt_arithmeticOrBooleanExpr,nt_N7,nt_AnyTerm,nt_N8,nt_arithmeticExpr,
	nt_N4,nt_term,nt_N5,nt_factor,nt_op1,nt_op2,nt_logicalOp,nt_relationalOp,nt_declareStmt,nt_conditionalStmt,
	nt_caseStmts,nt_N9,nt_value,nt_default,nt_iterativeStmt,nt_range

} allEnum;


struct tempTokenInfo{
	//0 - keyword, 1 - identifier, 2 - integer, 3 - real num, 4 - symbol, 5 - epsilon
	int type;
	allEnum allenum;
	char identifier[8];
	int integer;
	float rnum;
	struct tempTokenInfo* next;
	struct tempTokenInfo* prev;

	int lineNo;
};

typedef struct tempTokenInfo TokenInfo;


char keywordsList [][keywordLenMax] = {
		"AND", "array", "boolean", "break", "case", "declare", "default", "driver", "end", "for", "get_value", 
	"in", "input", "integer", "module", "of", "OR", "parameters", "print", "program", "real", "returns", "start", 
	"switch", "takes", "use", "while", "with", "false", "true"
};

char TokenList [][keywordLenMax] = {
	 "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", 
	"DRIVERDEF", "DEF", "DRIVERENDDEF", "ENDDEF", "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "EPSILON"
};

char TerminalsList [][keywordLenMax] = {
	"AND", "array", "boolean", "break", "case", "declare", "default", "driver", "end", "for", "get_value", 
	"in", "input", "integer", "module", "of", "OR", "parameters", "print", "program", "real", "returns", "start", 
	"switch", "takes", "use", "while", "with", "false", "true", 

	"PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", 
	"DRIVERDEF", "DEF", "DRIVERENDDEF", "ENDDEF",  "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", 

	"EPSILON","$",

	"ID", "NUM", "RNUM" 
};

char TerminalsListCaps [][keywordLenMax] = {
	"AND", "ARRAY", "BOOLEAN", "BREAK", "CASE", "DECLARE", "DEFAULT", "DRIVER", "END", "FOR", "GET_VALUE", 
	"IN", "INPUT", "INTEGER", "MODULE", "OF", "OR", "PARAMETERS", "PRINT", "PROGRAM", "REAL", "RETURNS", "START", 
	"SWITCH", "TAKES", "USE", "WHILE", "WITH", "FALSE", "TRUE", 

	"PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", 
	"DRIVERDEF", "DEF", "DRIVERENDDEF", "ENDDEF",  "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", 

	"EPSILON","$",

	"ID", "NUM", "RNUM" 
};

char TerminalsAndNonTerminalsList [][keywordLenMax] = {
	/******* All terminals and non terminals **********/
	"AND", "array", "boolean", "break", "case", "declare", "default", "driver", "end", "FOR", "GET_VALUE", 
	"in", "input", "integer", "module", "of", "OR", "parameters", "print", "program", "real", "returns", "start", 
	"switch", "takes", "use", "while", "with", "FALSE", "TRUE", 

	"PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", 
	"DRIVERDEF", "DEF", "DRIVERENDDEF", "ENDDEF", "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", 

	"EPSILON","$",

	"ID", "NUM", "RNUM" ,
	"program","moduleDeclaration","moduleDeclarations","otherModules","driverModule","module","ret","input_plist",
	"IP_List","output_plist","OP_List","dataType","type","moduleDef","statements","statement","ioStmt",
	"var","whichId","simpleStmt","assignmentStmt","whichStmt","lvalueIDStmt","lvalueARRStmt","index",
	"moduleReuseStmt","optional","idList","N3","expression","arithmeticOrBooleanExpr","N7","AnyTerm","N8",
	"arithmeticExpr","N4","term","N5","factor","op1","op2","logicalOp","relationalOp","declareStmt",
	"conditionalStmt", "caseStmts","N9","value","default","iterativeStmt","range"
};