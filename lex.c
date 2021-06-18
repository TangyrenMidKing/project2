// Shibo Ding
// Jiahao Zhu
// Cop3402 Summer project2

/*
	This is the lex skeleton for the UCF Summer 2021 Systems Software Project
	Implement the function lexanalyzer, add as many functions and global
	variables as desired, but do not alter printerror or printtokens.
	Include your name (and your partner's name) in this comment in order to
	earn the points for compiling
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_LEXEME 500
#define MAX_IDENT 11
#define MAX_DIGITS 5
#define UNDEFINED -1

lexeme *list;
int lex_index;
char symbols[14] =  {'=', '>' , '<' , '%' , '*' , '/' , '+' , '-' , '(' , ')' ,
              ',' , '.' , ';' ,  ':'};
char reserved[14][10] = {"const", "var", "procedure", "call", "if", "then", "else", "while",
						"do", "begin", "end", "read", "write", "odd"};



void printerror(int type);
void printtokens();
int token_function(char* str);
char* left_string(char* str, char* split_point);
int isReserved(char* str);
int isSymbols(char c);

lexeme *lexanalyzer(char *input)
{
	list = malloc(MAX_LEXEME * sizeof(lexeme));
	lex_index = 0;
	char* ptr = input;

	while (ptr[0] != '\0')
	{
			// Start with white space
			if (iscntrl(ptr[0]) == 0)
			{
				ptr++;
			}

			// Start with number.
			else if (isdigit(ptr[0]))
			{
        char* p = ptr;
        while (isdigit (p[0]))
          p++;
        char* num = left_string(ptr,p);
        // Name too long.
        if (strlen(num) > MAX_DIGITS)
        {
          printerror(3);
          free(list);
          return NULL;
        }

        // If a number is followed by an identifier with no whitespace,
        // it is an invalid identifier error.
        if ( strlen(p) > 0 & isdigit(p[1]) )
        {
          printerror(2);
          free(list);
          return NULL;
        }

        strcpy(list[lex_index].name , num);
        list[lex_index].value = atoi(num);
        list[lex_index].type = numbersym;

        ptr = p;
			}

			// Start with char
			else if (isalpha(ptr[0]))
			{
        char* p = ptr;
        while (isalpha(p[0]) | isdigit(p[0]))
          p++;
        char* str = left_string(ptr,p);
        // Name too long.
        if (strlen(str) > MAX_IDENT)
        {
          printerror(4);
          free(list);
          return NULL;
        }
        // reserved
        if (isReserved(str))
        {
          strcpy(list[lex_index].name , str);
          list[lex_index].value = UNDEFINED;
          list[lex_index].type = token_function(str);
        }
        // identifier
        else
        {
          strcpy(list[lex_index].name , str);
          list[lex_index].value = UNDEFINED;
          list[lex_index].type = identsym;
        }

        ptr = p;
			}

			// Start with special symbols
			else if (isSymbols(ptr[0]))
			{
        // Maybe Comment Symbol.
        if (ptr[0] == '/')
        {
          if (strlen(ptr) > 1 && ptr[1] == '*')
          {
            while (ptr[1] != '\0' & (ptr[0] != '*' | ptr[1] != '/'))
              ptr++;

            // Neverending comment
            if (strlen(ptr) == 0)
            {
              printerror(5);
              free(list);
              return NULL;
            }
            // Comment!
            else
              ptr++;
          }
          // slash symbol
          else
          {
            strcpy(list[lex_index].name , "/");
            list[lex_index].value = UNDEFINED;
            list[lex_index].type = slashsym;
            ptr++;
          }
        }
        // Non Comment Symbol.
        else
        {
          if (ptr[0] == '<')
          {
            ptr++;
            // <>
            if (ptr[0] == '>')
            {
              strcpy(list[lex_index].name , "<>");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = neqsym;
              ptr++;
            }
            // <=
            else if (ptr[0] == '=')
            {
              strcpy(list[lex_index].name , "<=");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = leqsym;
              ptr++;
            }
            // <
            else
            {
              strcpy(list[lex_index].name , "<");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = lessym;
              ptr++;
            }
          }
          else if (ptr[0] == '>')
          {
            ptr++;
            // >=
            if (ptr[0] == '=')
            {
              strcpy(list[lex_index].name , ">=");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = geqsym;
              ptr++;
            }
            // >
            else
            {
              strcpy(list[lex_index].name , ">");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = gtrsym;
              ptr++;
            }
          }
          else if (ptr[0] == ':')
          {
            ptr++;
            // :=
            if (ptr[0] == '=')
            {
              strcpy(list[lex_index].name , ":=");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = becomessym;
              ptr++;
            }
            // :
            else
            {
              strcpy(list[lex_index].name , ":");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = semicolonsym;
              ptr++;
            }
          }
          else
          {
            if (ptr[0] == '%')
            {
              strcpy(list[lex_index].name , "%");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = modsym;
              ptr++;
            }
            else if (ptr[0] == '*')
            {
              strcpy(list[lex_index].name , "*");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = multsym;
              ptr++;
            }
            else if (ptr[0] == '+')
            {
              strcpy(list[lex_index].name , "+");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = plussym;
              ptr++;
            }
            else if (ptr[0] == '-')
            {
              strcpy(list[lex_index].name , "-");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = minussym;
              ptr++;
            }
            else if (ptr[0] == ')')
            {
              strcpy(list[lex_index].name , ")");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = rparentsym;
              ptr++;
            }
            else if (ptr[0] == '(')
            {
              strcpy(list[lex_index].name , "(");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = lparentsym;
              ptr++;
            }
            else if (ptr[0] == ',')
            {
              strcpy(list[lex_index].name , ",");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = commasym;
              ptr++;
            }
            else if (ptr[0] == '.')
            {
              strcpy(list[lex_index].name , ".");
              list[lex_index].value = UNDEFINED;
              list[lex_index].type = periodsym;
              ptr++;
            }
            else
            {
              printerror(1);
              free(list);
              return NULL;
            }
          }
        }
			}
			// Unrecognized Sysmbels.
			else
			{
        printerror(0);
        free(list);
        return NULL;
			}

      lex_index++;
	}

	return list;
}

char* left_string(char* str, char* split_point)
{
  int len = strlen(str);
  int right = strlen(split_point);
  int left = len - right;

  char* res = malloc(sizeof(char)*left);
  strncpy(res,str, left);

  return res;
}

int token_function(char* str)
{
  if (strcmp(str, "const") == 0)
    return constsym;
  else if (strcmp(str, "var") == 0)
    return varsym;
  else if (strcmp(str, "procedure") == 0)
    return procsym;
  else if (strcmp(str, "call") == 0)
    return callsym;
  else if (strcmp(str, "if") == 0)
    return ifsym;
  else if (strcmp(str, "then") == 0)
    return thensym;
  else if (strcmp(str, "while") == 0)
    return whilesym;
  else if (strcmp(str, "do") == 0)
    return dosym;
  else if (strcmp(str, "begin") == 0)
    return beginsym;
  else if (strcmp(str, "end") == 0)
    return endsym;
  else if (strcmp(str, "read") == 0)
    return readsym;
  else if (strcmp(str, "write") == 0)
    return writesym;
  else if (strcmp(str, "odd") == 0)
    return oddsym;
  // Error
  else if (strcmp(str, "read") == 0)
    return 0;
}

int isReserved(char* str)
{
  for (int i = 0; i < 14; i++)
  {
    if (strcmp(str, reserved[i]) == 0)
      return 1;
  }
  return 0;
}

int isSymbols(char c)
{
  for (int i = 0; i < 14; i++)
  {
    if (c == symbols[i])
      return 1;
  }
  return 0;
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "<>", neqsym);
				break;
			case lessym:
				printf("%11s\t%d", "<", lessym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case slashsym:
				printf("%11s\t%d", "/", slashsym);
				break;
			case plussym:
				printf("%11s\t%d", "+", plussym);
				break;
			case minussym:
				printf("%11s\t%d", "-", minussym);
				break;
			case lparentsym:
				printf("%11s\t%d", "(", lparentsym);
				break;
			case rparentsym:
				printf("%11s\t%d", ")", rparentsym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case becomessym:
				printf("%11s\t%d", ":=", becomessym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
	list[lex_index++].type = -1;
}

void printerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else if (type == 5)
		printf("Lexical Analyzer Error: Neverending Comment\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");

	free(list);
	return;
}
