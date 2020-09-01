#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin, *yyout;

typedef enum eTOKENS
{
	TOKEN_KEYWORD_RETURN,
	TOKEN_KEYWORD_VOID,
	TOKEN_KEYWORD_IF,
	TOKEN_KEYWORD_INT,
	TOKEN_KEYWORD_FLOAT,
	TOKEN_ID,
	TOKEN_LESS_THAN,
	TOKEN_GREATER_THAN,
	TOKEN_GREATER_THAN_OR_EQUAL_TO,
	TOKEN_LESS_THAN_OR_EQUAL_TO,
	TOKEN_IS_NOT_EQUAL_TO,
	TOKEN_DOUBLE_EQUAL,
	TOKEN_MULTIPLICATION,
	TOKEN_PLUS,
	TOKEN_OP_EQUAL,
	TOKEN_COMMA,
	TOKEN_COLON,
    TOKEN_SEMICOLON,
	TOKEN_INTEGER_NUMBER,
	TOKEN_FLOAT_NUMBER,
	TOKEN_LEFT_PARENTHESIS,
	TOKEN_RIEHT_PARENTHESIS,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,
	TOKEN_LEFT_CURLY_BRACES,
	TOKEN_RIGHT_CURLY_BRACES,
	TOKEN_EOF
	/*
		Examples of tokens, add/change according to your needs.
		
		Alternative way to define tokens:
		#define TOKEN_INTEGER 1
		#define TOKEN_IF 2
		...........
	*/
}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token *next_token();
void back_token();

void freeList();
Token *getFirst();
#endif