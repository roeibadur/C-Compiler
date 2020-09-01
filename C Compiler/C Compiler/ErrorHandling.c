
#include "ErrorHandling.h"

extern FILE  *yyoutP;
 int size = 0;
 int sizeFirst = 0;

const char* getKindName(enum eTOKENS Kind)
{
	switch (Kind)
	{
	case TOKEN_KEYWORD_VOID: return " TOKEN_KEYWORD_VOID";
	case TOKEN_KEYWORD_RETURN: return "TOKEN_KEYWORD_RETURN";
	case TOKEN_KEYWORD_IF: return "TOKEN_KEYWORD_IF";
	case TOKEN_KEYWORD_INT: return "TOKEN_KEYWORD_INT";
	case TOKEN_KEYWORD_FLOAT: return "TOKEN_KEYWORD_FLOAT";
	case TOKEN_ID: return "TOKEN_ID";
	case TOKEN_LESS_THAN: return "TOKEN_LESS_THAN";
	case TOKEN_GREATER_THAN: return "TOKEN_GREATER_THAN";
	case TOKEN_GREATER_THAN_OR_EQUAL_TO: return "TOKEN_GREATER_THAN_OR_EQUAL_TO";
	case TOKEN_LESS_THAN_OR_EQUAL_TO: return "TOKEN_LESS_THAN_OR_EQUAL_TO";
	case TOKEN_IS_NOT_EQUAL_TO: return "TOKEN_IS_NOT_EQUAL_TO";
	case TOKEN_DOUBLE_EQUAL: return "TOKEN_DOUBLE_EQUAL";
	case TOKEN_MULTIPLICATION: return "TOKEN_MULTIPLICATION";
	case TOKEN_PLUS:return "TOKEN_PLUS";
	case TOKEN_OP_EQUAL: return "TOKEN_OP_EQUAL";
	case TOKEN_COMMA: return "TOKEN_COMMA";
	case TOKEN_COLON: return "TOKEN_COLON";
	case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
	case TOKEN_INTEGER_NUMBER: return "TOKEN_INTEGER_NUMBER";
	case TOKEN_FLOAT_NUMBER: return "TOKEN_FLOAT_NUMBER";
	case TOKEN_LEFT_PARENTHESIS: return "TOKEN_LEFT_PARENTHESIS";
	case TOKEN_RIEHT_PARENTHESIS: return "TOKEN_RIEHT_PARENTHESIS";
	case TOKEN_LEFT_BRACKET: return "TOKEN_LEFT_BRACKET";
	case TOKEN_RIGHT_BRACKET: return "TOKEN_RIGHT_BRACKET";
	case TOKEN_LEFT_CURLY_BRACES: return "TOKEN_LEFT_CURLY_BRACES";
	case TOKEN_RIGHT_CURLY_BRACES: return "TOKEN_RIGHT_CURLY_BRACES";
	case TOKEN_EOF: return "TOKEN_EOF";
	default:return "error";
	}
}

void error(Token *actual, eTOKENS expected)
{
	eTOKENS actualToken = actual->kind;
	int lineNumber = actual->lineNumber;
	fprintf(yyoutP, "Line %d : found token of kind{ %s }, expected: { %s } \n", lineNumber, getKindName(actualToken), getKindName(expected));
}

int checkIfFollow(eTOKENS* arrayToken, eTOKENS target)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (arrayToken[i] == target)
			return 1;
	}
	return 0;
}

void errorRecovery(char* parserName, Token *t)
{
	eTOKENS* arrayFollowToken;	
	PrintFirstTokens(parserName,t);
	arrayFollowToken = getFollowTokens(parserName);

	while (t->kind != TOKEN_EOF && !checkIfFollow(arrayFollowToken, t->kind) && t)
	{   
		t = next_token();
	}
	if(checkIfFollow(arrayFollowToken, t->kind) || t->kind == TOKEN_EOF)
       back_token();
}

void printTokensArray(eTOKENS *followArray, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		fprintf(yyoutP, "  %s  ", getKindName(followArray[i]));
	}
}

eTOKENS* getFirstTokens(char* parserName)
{
	eTOKENS *b;
	int i;
	if (!strcmp(parserName, "PROG") || !strcmp(parserName, "GLOBAL_VARS") || !strcmp(parserName, "VAR_DEC") ||
		!strcmp(parserName, "TYPE") || !strcmp(parserName, "PARAMS") || !strcmp(parserName, "PARAM_LIST") || !strcmp(parserName, "PARAM") ||
		!strcmp(parserName, "VAR_DEC_LIST") || !strcmp(parserName, "GLOBAL_VARS_Tag") || !strcmp(parserName, "VAR_DEC_LIST_Tag"))
	{
		eTOKENS a[] = { TOKEN_KEYWORD_INT, TOKEN_KEYWORD_FLOAT};
		sizeFirst = 2;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		for (i = 0; i < sizeFirst; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "FUNC_PREDEFS") || !strcmp(parserName, "FUNC_PROTOTYPE") || !strcmp(parserName, "FUNC_FULL_DEFS") ||
		!strcmp(parserName, "FUNC_WITH_BODY") || !strcmp(parserName, "RETURNED_TYPE") || !strcmp(parserName, "FUNC_PREDEFS_Tag")
		|| !strcmp(parserName, "FUNC_FULL_DEFS_Tag"))
	{
		eTOKENS a[] = { TOKEN_KEYWORD_INT, TOKEN_KEYWORD_FLOAT,	TOKEN_KEYWORD_VOID };
		sizeFirst = 3;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		for (i = 0; i < sizeFirst; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "ARGS") || !strcmp(parserName, "ARG_LIST") || !strcmp(parserName, "EXPR_LIST") ||
		!strcmp(parserName, "CONDITION") || !strcmp(parserName, "EXPR") || !strcmp(parserName, "TERM") || !strcmp(parserName, "FACTOR") ||
		!strcmp(parserName, "RETURN_STMT'") || !strcmp(parserName, "EXPR"))
	{
		eTOKENS a[] = { TOKEN_INTEGER_NUMBER, TOKEN_FLOAT_NUMBER,TOKEN_ID,TOKEN_LEFT_PARENTHESIS };
		sizeFirst = 4;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		for (i = 0; i < sizeFirst; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "STMT_LIST") || !strcmp(parserName, "STMT"))
	{
		eTOKENS a[] = { TOKEN_ID,TOKEN_KEYWORD_RETURN,TOKEN_KEYWORD_IF,TOKEN_LEFT_CURLY_BRACES };
		sizeFirst = 4;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		for (i = 0; i < sizeFirst; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "CALL") || !strcmp(parserName, "VAR"))
	{
		eTOKENS a[] = { TOKEN_ID};
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));		
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "DIM_SIZES"))
	{
		eTOKENS a[] = { TOKEN_INTEGER_NUMBER };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "COMP_STMT"))
	{
		eTOKENS a[] = { TOKEN_LEFT_CURLY_BRACES };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "RETURN_STMT"))
	{
		eTOKENS a[] = { TOKEN_KEYWORD_RETURN };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "IF_STMT"))
	{
		eTOKENS a[] = { TOKEN_KEYWORD_IF };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "DIM_SIZES_Tag") || !strcmp(parserName, "PARAM_LIST_Tag") || !strcmp(parserName, "ARG_LIST_Tag")
		|| !strcmp(parserName, "EXPR_LIST_Tag"))
	{
		eTOKENS a[] = { TOKEN_COMMA };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "VAR_DEC_Tag"))
	{
		eTOKENS a[] = { TOKEN_SEMICOLON ,TOKEN_LEFT_BRACKET };
		sizeFirst = 2;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		b[1] = a[1];
		return b;
	}
	else if (!strcmp(parserName, "PARAM_Tag") || !strcmp(parserName, "VAR_Tag"))
	{
		eTOKENS a[] = { TOKEN_LEFT_BRACKET };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
	    return b;
	}
	else if (!strcmp(parserName, "STMT_LIST_Tag"))
	{
		eTOKENS a[] = { TOKEN_SEMICOLON };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "EXPR_Tag"))
	{
		eTOKENS a[] = { TOKEN_PLUS };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "TERM_Tag"))
	{
		eTOKENS a[] = { TOKEN_MULTIPLICATION };
		sizeFirst = 1;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		return b;
	}
	else if (!strcmp(parserName, "FACTOR_Tag"))
	{
		eTOKENS a[] = { TOKEN_LEFT_CURLY_BRACES ,TOKEN_LEFT_PARENTHESIS };
		sizeFirst = 2;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		b[1] = a[1];
		return b;
	}
	else if (!strcmp(parserName, "STMT_Tag"))
	{
		eTOKENS a[] = { TOKEN_LEFT_CURLY_BRACES ,TOKEN_LEFT_PARENTHESIS,TOKEN_OP_EQUAL };
		sizeFirst = 3;
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		b[0] = a[0];
		b[1] = a[1];
		b[2] = a[2];
		return b;
	}
	else 
	{
		b = (eTOKENS*)malloc(sizeFirst * sizeof(eTOKENS));
		return b;
	}

}

void PrintFirstTokens(char* parserName, Token *token)
{
	eTOKENS *b = getFirstTokens(parserName);
	fprintf(yyoutP, "ERROR-Expected token\\token's: ");
	printTokensArray(b, sizeFirst);
	fprintf(yyoutP, "at line: '%d', Actual token '%s', lexeme: '%s'.\n", token->lineNumber, getKindName(token->kind), token->lexeme);
}

eTOKENS* getFollowTokens(char* parserName)
{
	eTOKENS *b;
	int i;
	if (!strcmp(parserName, "GLOBAL_VARS") || !strcmp(parserName, "FUNC_PREDEFS") || !strcmp(parserName, "GLOBAL_VARS_Tag") ||
		!strcmp(parserName, "FUNC_PREDEFS_Tag") )
	{
		eTOKENS a[] = { TOKEN_KEYWORD_INT, TOKEN_KEYWORD_FLOAT,	TOKEN_KEYWORD_VOID };
		size = 3;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "PROG") || !strcmp(parserName, "FUNC_FULL_DEFS") || !strcmp(parserName, "FUNC_FULL_DEFS_Tag"))
	{
		eTOKENS a[] = { TOKEN_EOF };
		size = 1;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "STMT") || !strcmp(parserName, "IF_STMT") || !strcmp(parserName, "RETURN_STMT") ||
		!strcmp(parserName, "RETURN_STMT_Tag") || !strcmp(parserName, "STMT_Tag"))
	{
		eTOKENS a[] = { TOKEN_SEMICOLON ,TOKEN_RIGHT_CURLY_BRACES };
		size = 2;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "ARG_LIST_Tag") || !strcmp(parserName, "PARAM_LIST_Tag") || !strcmp(parserName, "CONDITION")
		|| !strcmp(parserName, "ARG_LIST") || !strcmp(parserName, "ARGS") || !strcmp(parserName, "PARAM_LIST") || !strcmp(parserName, "PARAMS"))
	{
		eTOKENS a[] = { TOKEN_RIEHT_PARENTHESIS };
		size = 1;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "DIM_SIZES") || !strcmp(parserName, "EXPR_LIST") || !strcmp(parserName, "DIM_SIZES_Tag")
		|| !strcmp(parserName, "EXPR_LIST_Tag"))
	{
		eTOKENS a[] = { TOKEN_RIGHT_BRACKET };
		size = 1;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "TYPE") || !strcmp(parserName, "RETURNED_TYPE"))
	{
		eTOKENS a[] = { TOKEN_ID };
		size = 1;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "PARAM_Tag") || !strcmp(parserName, "PARAM"))
	{
		eTOKENS a[] = { TOKEN_COMMA,TOKEN_RIEHT_PARENTHESIS };
		size = 2;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "VAR_DEC") || !strcmp(parserName, "VAR_DEC_Tag"))
	{
		eTOKENS a[] = { TOKEN_KEYWORD_INT, TOKEN_KEYWORD_FLOAT,	TOKEN_KEYWORD_VOID,TOKEN_LEFT_CURLY_BRACES,TOKEN_ID,TOKEN_KEYWORD_IF,TOKEN_KEYWORD_RETURN };
		size = 7;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "FUNC_PROTOTYPE"))
	{
		eTOKENS a[] = { TOKEN_LEFT_CURLY_BRACES,TOKEN_SEMICOLON };
		size = 2;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "FUNC_WITH_BODY"))
	{
		eTOKENS a[] = { TOKEN_KEYWORD_INT, TOKEN_KEYWORD_FLOAT,	TOKEN_KEYWORD_VOID,TOKEN_EOF };
		size = 3;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "COMP_STMT"))
	{
		eTOKENS a[] = { TOKEN_KEYWORD_INT, TOKEN_KEYWORD_FLOAT,	TOKEN_KEYWORD_VOID,TOKEN_EOF,TOKEN_SEMICOLON ,TOKEN_RIGHT_CURLY_BRACES };
		size = 5;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "VAR_DEC_LIST") || !strcmp(parserName, "VAR_DEC_LIST_Tag"))
	{
		eTOKENS a[] = { TOKEN_LEFT_CURLY_BRACES ,TOKEN_ID , TOKEN_KEYWORD_IF ,TOKEN_KEYWORD_RETURN };
		size = 4;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "STMT_LIST") || !strcmp(parserName, "STMT_LIST_Tag"))
	{
		eTOKENS a[] = { TOKEN_RIGHT_CURLY_BRACES };
		size = 1;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "EXPR") || !strcmp(parserName, "EXPR_Tag"))
	{
		eTOKENS a[] = { TOKEN_COMMA,TOKEN_RIEHT_PARENTHESIS,TOKEN_LESS_THAN,TOKEN_GREATER_THAN,	TOKEN_GREATER_THAN_OR_EQUAL_TO,
		TOKEN_LESS_THAN_OR_EQUAL_TO,TOKEN_IS_NOT_EQUAL_TO,TOKEN_DOUBLE_EQUAL,TOKEN_SEMICOLON,TOKEN_RIGHT_CURLY_BRACES,
		TOKEN_RIGHT_BRACKET };
		size = 11;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "TERM") || !strcmp(parserName, "TERM_Tag"))
	{
		eTOKENS a[] = { TOKEN_COMMA,TOKEN_RIEHT_PARENTHESIS,TOKEN_LESS_THAN,TOKEN_GREATER_THAN,	TOKEN_GREATER_THAN_OR_EQUAL_TO,
		TOKEN_LESS_THAN_OR_EQUAL_TO,TOKEN_IS_NOT_EQUAL_TO,TOKEN_DOUBLE_EQUAL,TOKEN_SEMICOLON,TOKEN_RIGHT_CURLY_BRACES,
		TOKEN_RIGHT_BRACKET,TOKEN_PLUS };
		size = 12;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "FACTOR") || !strcmp(parserName, "FACTOR_Tag"))
	{
		eTOKENS a[] = { TOKEN_COMMA,TOKEN_RIEHT_PARENTHESIS,TOKEN_LESS_THAN,TOKEN_GREATER_THAN,	TOKEN_GREATER_THAN_OR_EQUAL_TO,
		TOKEN_LESS_THAN_OR_EQUAL_TO,TOKEN_IS_NOT_EQUAL_TO,TOKEN_DOUBLE_EQUAL,TOKEN_SEMICOLON,TOKEN_RIGHT_CURLY_BRACES,
		TOKEN_RIGHT_BRACKET,TOKEN_PLUS,TOKEN_MULTIPLICATION };
		size = 13;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else if (!strcmp(parserName, "VAR_Tag"))
	{
		eTOKENS a[] = { TOKEN_COMMA,TOKEN_RIEHT_PARENTHESIS,TOKEN_LESS_THAN,TOKEN_GREATER_THAN,	TOKEN_GREATER_THAN_OR_EQUAL_TO,
		TOKEN_LESS_THAN_OR_EQUAL_TO,TOKEN_IS_NOT_EQUAL_TO,TOKEN_DOUBLE_EQUAL,TOKEN_SEMICOLON,TOKEN_RIGHT_CURLY_BRACES,
		TOKEN_RIGHT_BRACKET,TOKEN_PLUS,TOKEN_MULTIPLICATION,TOKEN_OP_EQUAL };
		size = 14;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		for (i = 0; i < size; i++)
			b[i] = a[i];
		return b;
	}
	else
	{
		size = 0;
		b = (eTOKENS*)malloc(size * sizeof(eTOKENS));
		return b;
	}
}