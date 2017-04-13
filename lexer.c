// Batch Number 83
// Suyash Kumar 2014A7PS053P
// Siddharth Agarwal 2014A7PS025P
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "lexerDef.h"
#define streamSize 100

int line_num = 1;
	

//file pointer to the code
FILE* fileptr; //bla bla

//the function to move the pointer back if the sentence isnt finished in the 100 sized buffer

TokenInfo * headPtr;
TokenInfo * tailPtr;
TokenInfo * currPtr;

char * uppercase(char * str)
{
	int len = strlen(str) + 1;
	// if(!strcmp(str,"ASSIGNOP"))
		// printf("*************%d****************\n",len );
	char * tmpUpper = (char*)malloc(sizeof(char)*len);

	int i = 0;
	for(i = 0;i<len;i++)
	{
		tmpUpper[i] = toupper(str[i]);
	}
	tmpUpper[len] = '\0';
	return tmpUpper;
}


/****************** List of keywords ***************************/

int findKeyword(char * compare)
{
	int len = keywordUniqueNum;
	int i;
	for(i = 0;i<len;i++)
	{
		if(strcmp(keywordsList[i], compare)==0)
			return i;
	}
	return -1;
}
/***************** Initialise functions ************************************/
void initialise(char * filename)
{
	line_num = 1;
	//************ Token list initialisation code
	headPtr = NULL;
	tailPtr = NULL;
	currPtr = NULL;


	//************ File Opening **********************************
	fileptr = fopen(filename,"r"); //bla bla
}

/************* Reser pointer to start of tokens ******************/
void resetPtr()
{
	currPtr = headPtr;
}

/********************** FUNCTIONS RELATED TO TOKEN LINKED LIST *****************************/
//barebase add Token function for token linked list
void addToken(TokenInfo* newToken)
{
	if(headPtr == NULL)
	{
		newToken->next = NULL;
		newToken->prev = NULL;
		headPtr = newToken;
		tailPtr = newToken;
		currPtr = headPtr;
	}
	else
	{
		tailPtr->next = newToken;
		newToken->prev = tailPtr;
		tailPtr = newToken;
	}

	// free(newToken);
}

void addTokenInteger(int value, int lineNo)
{
	TokenInfo * newToken= (TokenInfo*)malloc(sizeof(TokenInfo));
	
	newToken->type = 2;
	newToken->integer = value;
	newToken->lineNo = lineNo;
	newToken->allenum = (allEnum)NUM;

	addToken(newToken);

	newToken = NULL;
}

void addTokenReal(float value, int lineNo)
{	
	TokenInfo * newToken= (TokenInfo*)malloc(sizeof(TokenInfo));
	
	newToken->type = 3;
	newToken->rnum = value;
	newToken->lineNo = lineNo;
	newToken->allenum = (allEnum)RNUM;

	addToken(newToken);
	newToken = NULL;

}

void addTokenID(char * identifier, int lineNo)
{
	// if(!strcmp(identifier, "start"))
	// 	printf("faa");
	TokenInfo * newToken= (TokenInfo*)malloc(sizeof(TokenInfo));
	
	newToken->type = 1;
	strcpy(newToken->identifier,identifier);
	newToken->lineNo = lineNo;
	newToken->allenum = (allEnum)ID;

	addToken(newToken);
	newToken = NULL;

}

void addTokenKeyword(allEnum keyword, int lineNo)
{
	// if(keyword==22)
	// 	printf("hap");
	TokenInfo * newToken= (TokenInfo*)malloc(sizeof(TokenInfo));
	
	newToken->type = 0;
	newToken->allenum = keyword;
	newToken->lineNo = lineNo;

	addToken(newToken);	
	newToken = NULL;

}

void addTokenSymbol(allEnum keyword, int lineNo)
{
	TokenInfo * newToken= (TokenInfo*)malloc(sizeof(TokenInfo));
	
	newToken->type = 4;
	newToken->allenum = keyword;
	newToken->lineNo = lineNo;

	addToken(newToken);	
	newToken = NULL;

}

/////////////// This is a file read operation and may have error in new compiler
/////////////// I have added something new here on 18/3
void readLess(char * str)
{
	// int last = streamSize - 1;
	// if(str[last]=='\0')
	// 	last--;

	int last = strlen(str);
	while(last > 0 && (!(str[last]==' '||str[last]=='\t')))
	{
		last = last - 1;
	}
	last++;
	str[last]='\0';
	fseek(fileptr,last - streamSize,SEEK_CUR);	
}

/*********************** Functions for getting Tokens from linked list ************************************/
TokenInfo * getNextToken()
{

	// why am i checking currPtr->next = NULL? see again
	if(currPtr == NULL) //|| currPtr->next ==NULL)
	{
		return NULL;
	}
	else
	{
		TokenInfo * temp = currPtr;
		currPtr = currPtr->next;
		return temp;

// ************** This was some jugaad 
		// if(temp->type==2 && (!strcmp(temp->identifier, "start")))
		// {
		// 	TokenInfo* bla = (TokenInfo*)malloc(sizeof(TokenInfo));
		// 	bla->type = 0;
		// 	bla->allenum = START;
		// 	bla->next = temp->next;
		// 	bla->prev = temp->prev;

		// 	return bla;
		// }
	}
}


/*********************** Functions related to getting input and removing comments **************************/
// function to get the string of length <=100 for processing

//************ This is a function related to file handling and may have errors in new compiler
char * getOptimumString()
{
	// printf("working");
	
	char * string = (char*)(malloc(sizeof(char)*streamSize*2));

	if(fgets(string, streamSize, fileptr) != NULL)
	{
		if(strlen(string) == streamSize)
			readLess(string);

		// printf("%s\n",string);
	}
	else
	{
		return NULL;
	}
	
	return string;
}

void removeCommentsPartially(char *output)
{
	// fileptr = fopen(input , "r");

	FILE *fclean = fopen(output , "w");
	
	char buffer[streamSize];

	strcpy(buffer,getOptimumString());

	int isComment = 0;
	


	while(strcmp(buffer,"")!=0)
	{
			int i=0, len = strlen(buffer);
			while(i < len)
			{
				//inComment corresponds to flag
				if (isComment==1)
				{	
					//check for ending comments
					while(i<len)
					{
						if(buffer[i]=='\n')
						{
							fprintf(fclean ,"\n");
						}

						if(buffer[i]=='*')
						{
							if(i+1<len && buffer[i+1]=='*')
								break;
						}
						i++;
					}
				}
				// insert nonComment lines
				while(i<len && ! (buffer[i]=='*' && i+1 < len && buffer[i+1]=='*'))
				{
					fprintf(fclean ,"%c", buffer[i++]);
				}			
				// if true, last two are comments
				if(i!=len)
				{
					i = i+2;
					isComment = !isComment;
				}
			}
			char * temp = getOptimumString();
			if(temp == NULL)
				break;
			strcpy(buffer, temp);
			free(temp);
	}
	fclose(fclean);

}

//takes the input code file and removes comments, then puts in a temporary output file
void removeCommentsCompletely(char *output)
{
	// fileptr = fopen(input , "r");

	FILE *fclean = fopen(output , "w");
	
	char buffer[streamSize];

	strcpy(buffer,getOptimumString());

	int isComment = 0;
	


	while(strcmp(buffer,"")!=0)
	{
			int i=0, len = strlen(buffer);
			while(i < len)
			{
				//inComment corresponds to flag
				if (isComment==1)
				{	
					//check for ending comments
					while(i<len)
					{
						if(buffer[i]=='*')
						{
							if(i+1<len && buffer[i+1]=='*')
								break;
						}
						i++;
					}
				}
				// insert nonComment lines
				while(i<len && ! (buffer[i]=='*' && i+1 < len && buffer[i+1]=='*'))
				{
					fprintf(fclean ,"%c", buffer[i++]);
				}			
				// if true, last two are comments
				if(i!=len)
				{
					i = i+2;
					isComment = !isComment;
				}
			}
			char * temp = getOptimumString();
			if(temp == NULL)
				break;
			strcpy(buffer, temp);
			free(temp);
	}
	fclose(fclean);
}

void computePower(char* buffer, int * idx, int * po)
{
	while(buffer[*idx]>='0'&&buffer[*idx]<='9')
	{
		(*idx) = (*idx) + 1;
		*po=((*po)*10)+(buffer[*idx]-'0');
	}
}


int checkIDalpha(char character)
{
	return ((character >= 'A' && character <= 'Z')||(character >= 'a' && character <= 'z')||(character >= '0' && character <= '9')||(character == '_'))&& (character != '\n' || character != '\r');
}

int charToNum(char a)
{
	if(a>='0' && a <='9')
		return a - '0';
	else
		return -1;
}

/***************************** Functions for error reporting **********************************/
void tooLongIdentifier(int line_num)
{
	printf("\nERROR_1 : Identifier at line %d is longer than the prescribed length \n", line_num);
}

void unknownPattern(char * lexeme, int line_num)
{
	printf("\nERROR_3: Unknown pattern %s at line %d \n",lexeme, line_num);

	free(lexeme);
}

void unknownSymbolerror(char symbol, int line_num)
{
	if(!(symbol=='\n' || symbol == '\r'))
	printf("\nERROR_2: Unknown Symbol %c at line %d\n", symbol, line_num);
}



/***************************** Function for getting correct Token and adding it ***************************/
void tokenise(char* buffer)
{
	int idx = 0;
	int len = strlen(buffer);
	int state = 1;
	char lookAhead;
	while(idx < len)
	{
		// while(idx < len && (buffer[idx]==' ' || buffer[idx] == '\t'))
		// {
		// 	idx ++;
		// }

		// if(idx < len && (buffer[idx] == '\n' || buffer[idx] == '\r'))
		// 	break;
		lookAhead = buffer[idx];
			// printf("%d\n",idx);
			//State 2
			if(lookAhead == '+')
			{
				// state = 2;
				allEnum keyword = PLUS;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			//State 3
			else if(lookAhead == '-')
			{
				// state = 3;
				allEnum keyword = MINUS;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			else if(lookAhead == '*')
			{
				allEnum keyword = MUL;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;	
			}
			else if(lookAhead == '/')
			{
				allEnum keyword = DIV;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			else if(lookAhead == '<')
			{
				idx++;
				lookAhead = buffer[idx];

				allEnum keyword;
				if(lookAhead == '=')
				{
					keyword = LE;
					idx++;
				}
				else if(lookAhead == '<')
				{
					idx++;
					lookAhead = buffer[idx];

					if(lookAhead == '<')
					{
						keyword = DRIVERDEF;
						idx++;
					}
					else
					{
						keyword = DEF;
					}
				}
				else
				{
					keyword = LT;
				}

				addTokenSymbol(keyword, line_num);
			}
			else if(lookAhead == '>')
			{
				idx++;
				lookAhead = buffer[idx];

				allEnum keyword;
				if(lookAhead == '=')
				{
					keyword = GE;
					idx++;
				}
				else if(lookAhead == '>')
				{
					idx++;
					lookAhead = buffer[idx];

					if(lookAhead == '>')
					{
						keyword = DRIVERENDDEF;
						idx++;
					}
					else
					{
						keyword = ENDDEF;
					}
				}
				else
				{
					keyword = GT;
				}

				addTokenSymbol(keyword, line_num);
			}
			else if(lookAhead == '=')
			{
				//Whats with the unknown lexeme?
				char* unknownLexeme = (char*)(malloc(sizeof(char)*2));

				unknownLexeme[0] = lookAhead;
				allEnum keyword;

				idx++;
				lookAhead = buffer[idx];

				if(lookAhead == '=')
				{
					keyword = EQ;
					idx++;
					addTokenSymbol(keyword, line_num);
				}
				else
				{
					unknownLexeme[1] = lookAhead;
					unknownPattern(unknownLexeme, line_num);          //************** new error
				}
			
			}
			else if(lookAhead == '!')
			{
				allEnum keyword;

				char* unknownLexeme = (char*)(malloc(sizeof(char)*2));

				unknownLexeme[0] = lookAhead;

				idx++;
				lookAhead = buffer[idx];

				if(lookAhead == '=')
				{
					keyword = NE;
					idx++;
					addTokenSymbol(keyword, line_num);
				}
				else
				{
					unknownLexeme[1] = lookAhead;
					unknownPattern(unknownLexeme, line_num);          //************** new error
				}
			
			}
			else if(lookAhead == ':')
			{
				allEnum keyword;

				idx++;
				lookAhead = buffer[idx];

				if(lookAhead == '=')
				{
					keyword = ASSIGNOP;
					idx++;
				}
				else
				{
					keyword = COLON;
				}
			
				addTokenSymbol(keyword, line_num);
			}
			else if(lookAhead == '.')
			{
				allEnum keyword;

				char* unknownLexeme = (char*)(malloc(sizeof(char)*2));

				unknownLexeme[0] = lookAhead;
				idx++;
				lookAhead = buffer[idx];

				if(lookAhead == '.')
				{
					keyword = RANGEOP;
					idx++;
					addTokenSymbol(keyword, line_num);
				}
				else
				{
					unknownLexeme[1] = lookAhead;
					unknownPattern(unknownLexeme, line_num);          //************** new error
				}
			}
			else if(lookAhead == ';')
			{
				// state = 3;
				allEnum keyword = SEMICOL;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			else if(lookAhead == ',')
			{
				// state = 3;
				allEnum keyword = COMMA;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			else if(lookAhead == '[')
			{
				// state = 3;
				allEnum keyword = SQBO;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			else if(lookAhead == ']')
			{
				// state = 3;
				allEnum keyword = SQBC;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			else if(lookAhead == '(')
			{
				// state = 3;
				allEnum keyword = BO;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			else if(lookAhead == ')')
			{
				// state = 3;
				allEnum keyword = BC;
				addTokenSymbol(keyword, line_num);
				// state = 1;
				idx++;
			}
			else if((lookAhead >= 'A' && lookAhead <= 'Z') || (lookAhead >= 'a' && lookAhead <= 'z'))
			{
				char * possStr = (char *)(malloc(sizeof(char)*20));
				int idxPoss = 0;
				possStr[idxPoss++] = lookAhead;
				idx++;

				while(idx < len && checkIDalpha(buffer[idx]))
				{
					possStr[idxPoss] = buffer[idx];
					idxPoss++;
					idx++;
				}
				possStr[idxPoss] = '\0';

				//*********** check for plag

				// printf("wdwmfd");
				
				int isKeyword = findKeyword(possStr);         // hash function find
				// if(strlen(possStr)==6)
				// printf("%d %c\n",possStr[5],possStr[5]);
				// if(!strcmp(possStr,"start"))
				// 	printf("***ad%d",isKeyword);
				if (isKeyword == -1)
				{
					// 	if(!strcmp(possStr,"start"))
					// printf("Puttugkubkuing");
				
					if (strlen(possStr)<=idLenMax) 				
					{				
						addTokenID(possStr, line_num);
					}
					else
					{
						tooLongIdentifier(line_num);          //*****************new error
					}
				}
				else
				{
					// if(!strcmp(possStr,"start"))
					// printf("Putting");
					// int j;
					// ********** Understand this
					// for(j=0;j<strlen(r.k);j++)				
					// fprintf(fp,"%c",toupper(r.k[j]));
					// printf("***ad%d",isKeyword);

					addTokenKeyword((allEnum)isKeyword, line_num);
				}
			// idx--;

			//****************** end plag
			}
			else if(lookAhead >= '0' && lookAhead <= '9')         //Handle numbers
			{
				int decNum = 0;
				int mantissa = charToNum(lookAhead), fraction = 0;
				int power = 0;

				while(idx + 1 < len && (buffer[idx + 1] >= '0' && buffer[idx + 1] <= '9'))
				{
					mantissa *= 10;
					mantissa += buffer[idx+1]- '0';
					idx++;
				}

				if(idx + 1 < len && buffer[idx + 1] == '.')
				{
					if(idx + 2 < len && buffer[idx + 2] == '.')
					{
						//its a range operator;
						addTokenInteger(mantissa, line_num);
						idx++;
					}
					else
					{
						idx ++;
						if(idx + 1 < len && (buffer[idx + 1]<'0' && buffer[idx + 1] > '9'))
						{
							//some random stuff after decimal point
							printf("Invalid real number\n");
						}
						else
						{
							while(idx + 1 < len && (buffer[idx + 1]>='0' && buffer[idx + 1] <= '9'))
							{
								idx ++ ;
								decNum ++ ;
								fraction *= 10;
								fraction += buffer[idx] - '0';	
							}

							if(idx + 1 < len && (buffer[idx + 1] == 'e' || buffer[idx + 1]=='E'))
							{
								idx += 2;
								
								if(idx < len && ((buffer[idx] == '+')))
								{
									computePower(buffer, &idx, &power);
								}
								else if(idx < len && ((buffer[idx] == '-'))) 
								{
									computePower(buffer, &idx, &power);
									power *= -1;
								}
								else if(idx < len && ((buffer[idx]) >= '0' && buffer[idx] <= '9'))
								{
									idx--;
									computePower(buffer, &idx, &power);
									//*********** Some slight change is here - hehe - lookout
								}
								else
								{
									printf("Invalid real number\n");
								}
							}
							else
							{
								power = 0;
							}

							float num = ((float)mantissa + (float)(fraction/pow(10, decNum))) * pow(10,power);
							addTokenReal(num, line_num);
							idx++;
						}
					}
				}
				else
				{
					//no . found
					addTokenInteger(mantissa, line_num);
						idx++;

				}

			}
			else if(buffer[idx]=='\n')
			{
				// printf("jdfowf\n");
				line_num++;
				idx++;
			}
			else if(buffer[idx]==' ' || buffer[idx]=='\t')
				idx++;
			else//|| buffer[idx]=='\n'))
			{
				// printf("%d %d\n", idx, (int)buffer[idx]);
				unknownSymbolerror(buffer[idx], line_num);
				idx++;
			}

			// while(idx < len && (buffer[idx]==' ' || buffer[idx]=='\t'))//||buffer[idx]=='\n' || buffer[idx]=='\r'))
			// 	idx++;
	}


}
	


void printToken(TokenInfo * bla)
{
	char* tempstr = NULL;
	// if(bla->allenum == 22)
	// 	printf("here %d \n",bla->type);
	if(bla != NULL)
	{
		switch(bla->type)
		{
			case 0:
				// printf("%d",(int)(bla->token));
				// tempstr = uppercase(keywordsList[(int)(bla->allenum)]);
				printf("%s : %s : Line %d\n",TerminalsListCaps[(int)(bla->allenum)], keywordsList[(int)(bla->allenum)], bla->lineNo);
				break;
			case 1:
				printf("ID : %s : Line %d\n",bla->identifier, bla->lineNo);
				break;
			case 2:
				printf("NUM : %d : Line %d\n",bla->integer, bla->lineNo);
				break;
			case 3:
				printf("RNUM : %f : Line %d\n",bla->rnum, bla->lineNo);
				break;	
			case 4:
				// tempstr = uppercase(TokenList[(int)(bla->allenum) - tokenOffset]);
				printf("%s : %s : Line %d\n",TokenList[(int)(bla->allenum) - tokenOffset],TokenList[(int)(bla->allenum) - tokenOffset], bla->lineNo);
				break;

		}
		// if(tempstr != NULL)
		// free(tempstr);
	}
}

void printLinkedList()
{
	TokenInfo * bla = headPtr;
	if(bla!=NULL)
	{
		while(bla->next !=NULL)
		{
			printToken(bla);
			bla = bla->next;
		}
		printToken(bla);
	}
}

void printProgramWithoutComments(char * filename)
{
	initialise(filename);
	removeCommentsCompletely("intermediate");
	fclose(fileptr);
	fileptr = fopen("intermediate","r");

	while(1)
	{
		char* temp = getOptimumString();
		if(temp == NULL)
		{
			break;
		}
		else
		{
			printf("%s",temp);
		}
	}

	fclose(fileptr);
}

void runTokenizer(char * filename)
{
	initialise(filename);
	removeCommentsPartially("intermediate");
	// printf("working\n");

	fclose(fileptr);
	fileptr = fopen("intermediate","r");

	while(1)
	{
		char* temp = getOptimumString();
		if(temp == NULL)
		{
			break;
		}
		else
		{
			tokenise(temp);
		}
	}

	fclose(fileptr);

	// addTokenSymbol($,line_num);
}

void mainOfLexer(char* filename)
{
	// initialise(filename);
	runTokenizer(filename);
	// printLinkedList();
	
}

// int main()
// {
// 	mainOfLexer("testcase3.txt");
// 	printLinkedList();
// }
// int main()
// {
// 	// printf("%d",findKeyword("start"));
// 	initialise("testcase2.txt");
// 	runTokenizer("testcase2.txt");
// 	printLinkedList();
// 	while(1)
// 	{
// 		TokenInfo * bla = getNextToken();
// 		if(bla!=NULL)
// 			printToken(bla);
// 		else
// 			break;	
// 	}
// }