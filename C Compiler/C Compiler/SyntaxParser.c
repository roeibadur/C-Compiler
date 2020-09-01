#include "SyntaxParser.h"
#include "ErrorHandling.h"
#include"Token.h"

extern FILE  *yyoutP;
FILE  *yyoutS;
int first = 1;
Token* t;
EntryType currFuncReturnType;
int didFunctionReturnValidType = 0;

void parser(int ex)
{
	yyoutS = ex == 1 ? fopen("C:\\temp\\test1_313299505_312350671_semantic.txt", "w+") : fopen("C:\\temp\\test2_313299505_312350671_semantic.txt", "w+");
	parse_PROG();
	match(TOKEN_EOF);
	fclose(yyoutS);
}



void parse_PROG()
{
	make_table();
	fprintf(yyoutP, "Rule(PROG -> GLOBAL_VARS FUNC_PREDEFS FUNC_FULL_DEFS) \n");
	parse_GLOBAL_VARS();
	do {
		TableEntry* function = parse_FUNC_PROTOTYPE(1);
		currFuncReturnType = function->entry_type;
		didFunctionReturnValidType = 0;
		int isValid = checkForDuplicate(function->nextParam);
		if (isValid == 0)
		{
			fprintf(yyoutS, "{Line %d} - func '%s' has duplicate pamras\n", t->lineNumber, function->name);
		}
		t = next_token();
		if (isValid == 1)
		{
			if (t->kind == TOKEN_SEMICOLON)
			{
				insertFunctionToTable(function,0);
			}
			else
			{
				TableEntry* returnFunction = find(function->name);
				if (returnFunction == NULL)
				{
					insertFunctionToTable(function,1);
					
				}
				else
				{
					make_table();
					assertEqualsFunction(returnFunction, function);
					insertParamsToTable(function->nextParam);
				}
			}
		}
	} while (t->kind == TOKEN_SEMICOLON);


	if (t->kind == TOKEN_LEFT_CURLY_BRACES)
	{
		back_token(); 
		parse_COMP_STMT();  
		if (currFuncReturnType != Void && didFunctionReturnValidType == 0)
		{
			fprintf(yyoutS, "{Line %d} - func didn't return valid type\n", t->lineNumber);
		}
		t = next_token();
		if (t->kind != TOKEN_EOF)    						
		{
			back_token();    
			parse_FUNC_FULL_DEFS();
		}
		else
			back_token();     
	}
	else
	{   
		fprintf(yyoutP, "Line %d : found token of kind{ %s }, expected: { TOKEN_LEFT_CURLY_BRACES } { TOKEN_SEMICOLON } \n", t->lineNumber, getKindName(t->kind));
		while (t->kind != TOKEN_EOF) { t = next_token(); }
		 back_token();		
	}
	pop_table(yyoutS);
	pop_table(yyoutS);
}

void parse_GLOBAL_VARS()
{
	fprintf(yyoutP, "Rule(GLOBAL_VARS -> VAR_DEC GLOBAL_VARS_Tag)\n");
	parse_VAR_DEC();
	parse_GLOBAL_VARS_Tag();

}

void parse_GLOBAL_VARS_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_KEYWORD_INT: case TOKEN_KEYWORD_FLOAT:
	{
		t = next_token();
		if (t->kind == TOKEN_ID)
		{   
			t = next_token();
			if (t->kind == TOKEN_LEFT_PARENTHESIS)
			{
				
				back_token();
				back_token();
				back_token();
				fprintf(yyoutP, "Rule(GLOBAL_VARS_Tag -> epsilon)\n");
				
			}
			else if (t->kind == TOKEN_SEMICOLON || t->kind == TOKEN_LEFT_BRACKET)
			{  
				back_token();
				back_token();
				back_token();
				fprintf(yyoutP, "Rule(GLOBAL_VARS_Tag ->VAR_DEC GLOBAL_VARS_Tag)\n");
				parse_VAR_DEC();
				parse_GLOBAL_VARS_Tag();
				break;
				
			}

			else {
				fprintf(yyoutP, "Line %d : expected token of kind{TOKEN_SEMICOLON}, {TOKEN_LEFT_BRACKET} found: { %s } \n", t->lineNumber, getKindName(t->kind));
				back_token();
				back_token();
				back_token();
				parse_VAR_DEC();
				parse_GLOBAL_VARS_Tag();
				break;			
			}
		}
		else {
			back_token();
			back_token();
			fprintf(yyoutP, "Rule(GLOBAL_VARS_Tag -> epsilon)\n");			
		}
		break;
	}
	case TOKEN_KEYWORD_VOID:
	{
		back_token();
		fprintf(yyoutP, "Rule(GLOBAL_VARS_Tag -> epsilon)\n");
		break;
	}
	default: errorRecovery("GLOBAL_VARS_Tag",t);
	}
}

void parse_FUNC_WITH_BODY()
{
	
	fprintf(yyoutP, "Rule(FUNC_WITH_BODY->FUNC_PROTOTYPE COMP_STMT)\n");
	TableEntry* function =  parse_FUNC_PROTOTYPE(0);
	currFuncReturnType = function->entry_type;
	int isValid = checkForDuplicate(function->nextParam);
	if (isValid == 0)
	{
		fprintf(yyoutS, "{Line %d} - func '%s' has duplicate pamras\n", t->lineNumber, function->name);
	}
	if (isValid == 1)
	{
			TableEntry* returnFunction = find(function->name);
			if (returnFunction == NULL)
			{			
				insertFunctionToTable(function,1);
			
			}
			else
			{			
				assertEqualsFunction(returnFunction, function);				
			}
	}
	make_table();
	insertParamsToTable(function->nextParam);
	didFunctionReturnValidType = 0;
	parse_COMP_STMT();
	if (currFuncReturnType != Void && didFunctionReturnValidType == 0)
	{
		fprintf(yyoutS, "{Line %d} - func didn't return valid type\n", t->lineNumber);
	}
	pop_table(yyoutS);
}


void parse_FUNC_FULL_DEFS()
{
	fprintf(yyoutP, "Rule(FUNC_FULL_DEFS->FUNC_WITH_BODY FUNC_FULL_DEFS_Tag)\n");
	parse_FUNC_WITH_BODY();
	parse_FUNC_FULL_DEFS_Tag();
}

void parse_STMT_Tag(char* lexme)
{
	int result = 0;
	TableEntry* entry = find(lexme);
	if (entry == NULL)
	{
		fprintf(yyoutS, "{Line %d} - id '%s' undefined\n", t->lineNumber, lexme);
	}
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_LEFT_PARENTHESIS:
	
					fprintf(yyoutP, "Rule(STMT_Tag->(ARGS)\n");
					result =parse_ARGS();
					match(TOKEN_RIEHT_PARENTHESIS);
					if (entry == NULL)
					{
						return Exception;
					}
					else if (entry->entry_role != Function)
					{
						fprintf(yyoutS, "{Line %d} - id '%s' is not function\n", t->lineNumber, entry->name);
						return Exception;
					}
					else
					{
						int counter = 0;
						TableEntry* ptr = entry->nextParam;
						while (ptr != NULL)
						{
							counter++;
							ptr = ptr->nextParam;
						}
						if (result != counter)
						{
							fprintf(yyoutS, "{Line %d} - invalid function '%s' call params not equal \n", t->lineNumber,entry->name);
							return Exception;
						}
					}
					break;
	case TOKEN_LEFT_BRACKET:
	case TOKEN_OP_EQUAL:
				back_token();
				fprintf(yyoutP, "Rule(STMT_Tag->VAR_Tag = EXPR)\n");
				result = parse_VAR_Tag();
				match(TOKEN_OP_EQUAL);
				EntryType type = parse_EXPR();
				if (entry == NULL)
				{
					return Exception;
				}
				else if (entry->entry_role == Function)
				{
					fprintf(yyoutS, "{Line %d} - id '%s' is function\n", t->lineNumber, entry->name);
					return Exception;
				}
				else if (result == -1)
				{
					return Exception;
				}
				else if (entry->entry_role == Array && result == 0)
				{
					fprintf(yyoutS, "{Line %d} - id '%s' is array\n", t->lineNumber, entry->name);
					return Exception;
				}
				else if (entry->entry_role == Array && result != entry->arrayDimnestions)
				{
					fprintf(yyoutS, "{Line %d} - id '%s' diffrent dimnesions\n", t->lineNumber, entry->name);
					return Exception;

				}
				else if (entry->entry_role == Variable && result > 0 )
				{
					fprintf(yyoutS, "{Line %d} - id '%s' is variable\n", t->lineNumber, entry->name);
					return Exception;
				}
				else if (entry->entry_type == Integer && type == Float)
				{
					fprintf(yyoutS, "{Line %d} - left side is Integer and right side is float - forbidden\n", t->lineNumber, entry->name);
					return Exception;
				}

				entry->isUsed = 1;
				break;
	default: 
			errorRecovery("STMT_Tag",t);
	}
}

EntryType parse_FACTOR_Tag(char* id)
{
	int result = 0;
	TableEntry* entry = find(id);
	if (entry == NULL)
	{
		fprintf(yyoutS, "{Line %d} - id '%s' undefined\n", t->lineNumber,id);
	}
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_LEFT_PARENTHESIS: 	  
				fprintf(yyoutP, "Rule(FACTOR_Tag -> ( ARGS ) )\n");
				result = parse_ARGS(); 
				match(TOKEN_RIEHT_PARENTHESIS);
				if (entry == NULL)
				{
					return Exception;
				}
				else if (entry->entry_role != Function)
				{
					fprintf(yyoutS, "{Line %d} - id '%s' is not function\n", t->lineNumber,entry->name);
					return Exception;
				}
				else
				{
					int counter = 0;
					TableEntry* ptr = entry->nextParam;
					while (ptr != NULL)
					{
						counter++;
						ptr = ptr->nextParam;
					}
					if (result != counter)
					{
						fprintf(yyoutS, "{Line %d} - invalid function call '%s' params not equal \n", t->lineNumber,entry->name);
						return Exception;
					}
				}
				entry->isUsed = 1;
				return entry->entry_type;
	case TOKEN_LEFT_BRACKET: 
				fprintf(yyoutP, "Rule(FACTOR_Tag -> VAR_Tag  )\n");
				back_token();
				result = parse_VAR_Tag();
				if (entry == NULL)
				{
					return Exception;
				}
				else if (entry->entry_role != Array)
				{
					fprintf(yyoutS, "{Line %d} - id '%s' is  not arryc\n", t->lineNumber, entry->name);
					return Exception;
				}
				else if (result == 0 && entry->entry_role == Variable)
				{
					fprintf(yyoutS, "{Line %d} - id '%s' is  variable\n", t->lineNumber, entry->name);
					return Exception;
				}
				else if (result == -1)
				{
					return Exception;
				}
				else if (result != entry->arrayDimnestions)
				{
					fprintf(yyoutS, "{Line %d} - id '%s' diffrent dimnesions\n", t->lineNumber, entry->name);
					return Exception;
				}
				entry->isUsed = 1;
				return entry->entry_type;
	case TOKEN_GREATER_THAN:
	case TOKEN_LESS_THAN:
	case TOKEN_GREATER_THAN_OR_EQUAL_TO:
	case TOKEN_LESS_THAN_OR_EQUAL_TO:
	case TOKEN_IS_NOT_EQUAL_TO:
	case TOKEN_MULTIPLICATION:case TOKEN_PLUS: 
	case TOKEN_DOUBLE_EQUAL:
	case TOKEN_RIGHT_BRACKET: 
	case TOKEN_COMMA:
	case TOKEN_RIEHT_PARENTHESIS:
	case TOKEN_SEMICOLON:
	case TOKEN_RIGHT_CURLY_BRACES:
					fprintf(yyoutP, "Rule(FACTOR_Tag ->  VAR_Tag )\n");
					back_token(); 
					int result = parse_VAR_Tag();
					if (entry == NULL)
					{
						return Exception;
					}
					else if (entry->entry_role != Variable)
					{
						fprintf(yyoutS, "{Line %d} - id '%s' is notvariable\n", t->lineNumber, entry->name);
						return Exception;
					}
					else if (result != 0 && entry->entry_role == Variable)
					{
						fprintf(yyoutS, "{Line %d} - id '%s' is  variable\n", t->lineNumber, entry->name);
						return Exception;
					}
					else if (result == -1)
					{
						return Exception;
					}
					entry->isUsed = 1;
					return entry->entry_type;
									break;
	default:	 
				 errorRecovery("FACTOR_Tag", t);				 
	}
}

EntryType parse_TERM_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_MULTIPLICATION:	
								fprintf(yyoutP, "Rule(TERM_Tag->*FACTOR TERM_Tag)\n");
								EntryType entry_factor_type = parse_FACTOR();
								EntryType entry_term_tag_type = parse_TERM_Tag();
								return calculteEntryType(entry_factor_type, entry_term_tag_type);
	case TOKEN_LESS_THAN:
	case TOKEN_GREATER_THAN:
	case TOKEN_GREATER_THAN_OR_EQUAL_TO:
	case TOKEN_LESS_THAN_OR_EQUAL_TO:
	case TOKEN_IS_NOT_EQUAL_TO:
	case TOKEN_DOUBLE_EQUAL:
	case TOKEN_PLUS: 
	case TOKEN_COMMA:
	case TOKEN_RIEHT_PARENTHESIS:
	case TOKEN_SEMICOLON:
	case TOKEN_RIGHT_BRACKET:
	case TOKEN_RIGHT_CURLY_BRACES: 
									back_token();
									fprintf(yyoutP, "Rule(TERM_Tag -> epsilon)\n");
									return Integer;
	default: 
			errorRecovery("TERM_Tag",t);
	}
}

EntryType parse_EXPR_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_PLUS:	
					fprintf(yyoutP, "Rule(EXPR_Tag -> +TERM EXPR_Tag)\n");
					EntryType entry_term_type = parse_TERM();
					EntryType entry_expr_tag_type = parse_EXPR_Tag();
					return calculteEntryType(entry_term_type, entry_expr_tag_type);
					break;	
	case TOKEN_GREATER_THAN:
	case TOKEN_GREATER_THAN_OR_EQUAL_TO:
	case TOKEN_LESS_THAN_OR_EQUAL_TO: 
	case TOKEN_IS_NOT_EQUAL_TO: 
	case TOKEN_DOUBLE_EQUAL: 
	case TOKEN_COMMA: 
	case TOKEN_RIEHT_PARENTHESIS: 
	case TOKEN_SEMICOLON:
	case TOKEN_RIGHT_BRACKET:
	case TOKEN_RIGHT_CURLY_BRACES: 
									back_token();
									fprintf(yyoutP, "Rule(EXPR_Tag -> epsilon )\n");
									return Integer;
	default: 
			errorRecovery("EXPR_Tag",t);
	}
}

int parse_EXPR_LIST_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_RIGHT_BRACKET: 
								back_token();
								fprintf(yyoutP, "Rule(EXPR_LIST_Tag -> epsilon )\n");
								return 0;
	case TOKEN_COMMA:	
						fprintf(yyoutP, "Rule(EXPR_LIST_Tag -> , EXPR EXPR_LIST_Tag )\n");
						EntryType type = parse_EXPR();
						int result = parse_EXPR_LIST_Tag();
						if (type != Integer)
						{
							fprintf(yyoutS, "{Line %d} - Expr inside array must to be integer\n", t->lineNumber);
							return -1;
						}
						if (result == -1)
						{
							return -1;
						}
						return result + 1;
	
	default: 
			errorRecovery("EXPR_LIST_Tag",t);
	}
}

int parse_VAR_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_LESS_THAN:
	case TOKEN_GREATER_THAN: 
	case TOKEN_GREATER_THAN_OR_EQUAL_TO: 
	case TOKEN_LESS_THAN_OR_EQUAL_TO: 
	case TOKEN_IS_NOT_EQUAL_TO: 
	case TOKEN_DOUBLE_EQUAL:
	case TOKEN_MULTIPLICATION:
	case TOKEN_PLUS: 
	case TOKEN_COMMA: 
	case TOKEN_RIEHT_PARENTHESIS: 
	case TOKEN_OP_EQUAL: 
	case TOKEN_SEMICOLON: 
	case TOKEN_RIGHT_BRACKET: 
	case TOKEN_RIGHT_CURLY_BRACES:
									back_token();
									fprintf(yyoutP, "Rule(VAR_Tag -> epsilon )\n");
									return 0;
	case TOKEN_LEFT_BRACKET:	
							fprintf(yyoutP, "Rule(VAR_Tag -> [ EXPR_LIST ] )\n");
							int result = parse_EXPR_LIST();
							match(TOKEN_RIGHT_BRACKET);
							return result;
	default: 
			errorRecovery("VAR_Tag",t);
	}
}

EntryType parse_RETURN_STMT_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_SEMICOLON: 
	case TOKEN_RIGHT_CURLY_BRACES: 
									back_token();
									fprintf(yyoutP, "Rule(RETURN_STMT_Tag -> epsilon )\n");
									return Void;
	case TOKEN_INTEGER_NUMBER:
	case TOKEN_FLOAT_NUMBER:
	case TOKEN_ID:
	case TOKEN_LEFT_PARENTHESIS:	
									back_token();
									fprintf(yyoutP, "Rule(RETURN_STMT_Tag -> EXPR )\n");
									return parse_EXPR();	
	default: 
			errorRecovery("RETURN_STMT_Tag",t);
	}
}

int parse_ARG_LIST_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_RIEHT_PARENTHESIS: 
									back_token();
									fprintf(yyoutP, "Rule(ARG_LIST_Tag -> epsilon )\n");
									return 0;
	case TOKEN_COMMA:	
						fprintf(yyoutP, "Rule(ARG_LIST_Tag -> , EXPR ARG_LIST_Tag )\n");
						parse_EXPR();
						return parse_ARG_LIST_Tag() + 1;
	default: 
			errorRecovery("ARG_LIST_Tag",t);
	}
}

void parse_FUNC_FULL_DEFS_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_EOF: 
					back_token();
					fprintf(yyoutP, "Rule(FUNC_FULL_DEFS_Tag -> epsilon )\n");
					break;
	case TOKEN_KEYWORD_VOID:
	case TOKEN_KEYWORD_FLOAT:
	case TOKEN_KEYWORD_INT:	
							back_token();
							fprintf(yyoutP, "Rule(FUNC_FULL_DEFS_Tag ->  FUNC_WITH_BODY FUNC_FULL_DEFS_Tag )\n");
							parse_FUNC_WITH_BODY();
							parse_FUNC_FULL_DEFS_Tag();
							break;	
	default: 
			errorRecovery("FUNC_FULL_DEFS_Tag",t);
	}
}

EntryType parse_FACTOR()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_ID: 
					fprintf(yyoutP, "Rule(FACTOR -> id FACTOR_Tag )\n");
					return parse_FACTOR_Tag(t->lexeme);
	case TOKEN_LEFT_PARENTHESIS:	
									fprintf(yyoutP, "Rule(FACTOR -> (EXPR) )\n");
									EntryType tpye =  parse_EXPR();
									match(TOKEN_RIEHT_PARENTHESIS);
									return tpye;
								
	case TOKEN_INTEGER_NUMBER:
								fprintf(yyoutP, "Rule(FACTOR ->  int_num )\n");
								return Integer;
	case TOKEN_FLOAT_NUMBER:
								fprintf(yyoutP, "Rule(FACTOR ->  float_num )\n");
								return Float;
	default: 
			errorRecovery("FACTOR",t);
	}
}

EntryType parse_TERM()
{
	fprintf(yyoutP, "Rule(TERM -> FACTOR TERM_Tag )\n");
	EntryType entry_factor_type = parse_FACTOR();
	EntryType entry_term_tag_type = parse_TERM_Tag();
	return calculteEntryType(entry_factor_type, entry_term_tag_type);
}

EntryType parse_EXPR()
{
	fprintf(yyoutP, "Rule(EXPR -> TERM EXPR_Tag )\n");
	EntryType entry_term_type = parse_TERM();
	EntryType entry_expr_tag_type = parse_EXPR_Tag();
	return calculteEntryType(entry_term_type, entry_expr_tag_type);
}

void parse_CONDITION()
{
	fprintf(yyoutP, "Rule(CONDITION -> EXPR rel_op EXPR )\n");
	parse_EXPR();
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_LESS_THAN:
	case TOKEN_GREATER_THAN:
	case TOKEN_GREATER_THAN_OR_EQUAL_TO:
	case TOKEN_LESS_THAN_OR_EQUAL_TO:
	case TOKEN_IS_NOT_EQUAL_TO:
	case TOKEN_DOUBLE_EQUAL: break;
	default: fprintf(yyoutP, "Line %d : found token of kind{ %s }, expected: { rel_op } \n", t->lineNumber, getKindName(t->kind));
	}
	parse_EXPR(); 
}

int parse_EXPR_LIST()
{
	fprintf(yyoutP, "Rule(EXPR_LIST -> EXPR EXPR_LIST_Tag )\n");
	EntryType type = parse_EXPR();
	int result = parse_EXPR_LIST_Tag();
	if (type != Integer)
	{
		fprintf(yyoutS, "{Line %d} - Expr inside array must to be integer\n", t->lineNumber);
		return -1;
	}
	if (result == -1)
	{
		return -1;
	}
	return result + 1;
}

void parse_VAR()
{
	t = next_token();
	if (t->kind == TOKEN_ID)
	{
		fprintf(yyoutP, "Rule(VAR -> id VAR_Tag )\n");
		parse_VAR_Tag();
	}
	else errorRecovery("VAR",t);

}



EntryType parse_RETURN_STMT()
{
	t = next_token();
	if (t->kind == TOKEN_KEYWORD_RETURN)
	{
		fprintf(yyoutP, "Rule(RETURN_STMT -> return RETURN_STMT_Tag )\n");
		return parse_RETURN_STMT_Tag();
	}
	else errorRecovery("RETURN_STMT",t);
}

int parse_ARG_LIST()
{
	fprintf(yyoutP, "Rule(ARG_LIST -> EXPR ARG_LIST_Tag )\n");
	parse_EXPR();
	return parse_ARG_LIST_Tag() + 1;
}

int parse_ARGS()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_INTEGER_NUMBER:
	case TOKEN_FLOAT_NUMBER:
	case TOKEN_ID:
	case TOKEN_LEFT_PARENTHESIS:	
									back_token();
									fprintf(yyoutP, "Rule(ARGS -> ARG_LIST )\n");
									return	parse_ARG_LIST();
								
	case TOKEN_RIEHT_PARENTHESIS:	
									back_token();
									fprintf(yyoutP, "Rule(ARGS -> epsilon )\n");
									return 0;
	default: 
			errorRecovery("ARGS",t);
	}
}

void parse_CALL()
{
	Token  *t = next_token();
	if (t->kind == TOKEN_ID)
	{
		fprintf(yyoutP, "Rule(CALL -> id( ARGS ) )\n");
		match(TOKEN_LEFT_PARENTHESIS);
		parse_ARGS();
		match(TOKEN_RIEHT_PARENTHESIS);
	}
	else errorRecovery("CALL",t);
}

void parse_IF_STMT()
{
	t = next_token();
	if (t->kind == TOKEN_KEYWORD_IF)
	{
		fprintf(yyoutP, "Rule(IF_STMT -> if(CONDITION)STMT )\n");
		match(TOKEN_LEFT_PARENTHESIS);
		parse_CONDITION();
		match(TOKEN_RIEHT_PARENTHESIS);
		parse_STMT();
	}
	else errorRecovery("IF_STMT",t);
}

void parse_STMT_LIST_Tag()
{
	Token  *t = next_token();
	switch (t->kind)
	{
	case TOKEN_SEMICOLON:	
							fprintf(yyoutP, "Rule(STMT_LIST_Tag -> ; STMT STMT_LIST_Tag  )\n");
							parse_STMT();
							parse_STMT_LIST_Tag();
							break;	
	case TOKEN_RIGHT_CURLY_BRACES:
									back_token();
									fprintf(yyoutP, "Rule(STMT_LIST_Tag -> epsilon  )\n");
									break;
	default: 
			errorRecovery("STMT_LIST_Tag",t);
	}
}

void parse_STMT_LIST()
{
	fprintf(yyoutP, "Rule(STMT_LIST -> STMT STMT_LIST_Tag)\n");
	parse_STMT();
	parse_STMT_LIST_Tag();
}

void parse_STMT()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_ID: 
			fprintf(yyoutP, "Rule(STMT -> id STMT_Tag)\n");
			parse_STMT_Tag(t->lexeme);
			break;
	case TOKEN_KEYWORD_IF: 
			back_token();
			fprintf(yyoutP, "Rule(STMT -> IF_STMT )\n");
			parse_IF_STMT();
			break;
	case TOKEN_KEYWORD_RETURN: 
			back_token();
			fprintf(yyoutP, "Rule(STMT -> RETURN_STMT)\n");
			EntryType type =  parse_RETURN_STMT();
			if (type != currFuncReturnType)
			{
				fprintf(yyoutS, "{Line %d} - return statment dosen't match to function type\n", t->lineNumber);
			}
			else
			{
				didFunctionReturnValidType = 1;
			}
			break;
	case TOKEN_LEFT_CURLY_BRACES: 
					back_token();
					fprintf(yyoutP, "Rule(STMT -> COMP_STMT)\n");
					parse_COMP_STMT();
			break;
	default: 
			errorRecovery("STMT",t);
	}
}

void parse_VAR_DEC_LIST_Tag()
{
	Token  *t = next_token();
	switch (t->kind)
	{
	case TOKEN_KEYWORD_FLOAT:
	case TOKEN_KEYWORD_INT:	
									back_token();
									fprintf(yyoutP, "Rule(VAR_DEC_LIST_Tag  - > VAR_DEC VAR_DEC_LIST_Tag)\n");
									parse_VAR_DEC();
									parse_VAR_DEC_LIST_Tag();
									break;	
	case TOKEN_LEFT_CURLY_BRACES:
	case TOKEN_ID:
	case TOKEN_KEYWORD_IF:
	case TOKEN_KEYWORD_RETURN:
								fprintf(yyoutP, "Rule(VAR_DEC_LIST_Tag  - > epsilon)\n");
								back_token();
								break;	
	default: 
			errorRecovery("VAR_DEC_LIST_Tag",t);
	}
}

void parse_VAR_DEC_LIST()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_KEYWORD_FLOAT:
	case TOKEN_KEYWORD_INT:
								 back_token();
								 fprintf(yyoutP, "Rule(VAR_DEC_LIST -> VAR_DEC VAR_DEC_LIST_Tag)\n");
								 parse_VAR_DEC();
								 parse_VAR_DEC_LIST_Tag();
								 break;	
	case TOKEN_LEFT_CURLY_BRACES:
	case TOKEN_ID:
	case TOKEN_KEYWORD_IF:
	case TOKEN_KEYWORD_RETURN:	
								back_token();
								fprintf(yyoutP, "Rule(VAR_DEC_LIST -> epsilon )\n");
								break;	
	default: 
			errorRecovery("VAR_DEC_LIST",t);
	}
}

void parse_COMP_STMT()
{
	t = next_token();
	if (t->kind == TOKEN_LEFT_CURLY_BRACES)
	{
		make_table();
		fprintf(yyoutP, "Rule(COMP_STMT -> { VAR_DEC_LIST STMT_LIST })\n");
		parse_VAR_DEC_LIST();
		parse_STMT_LIST();
		match(TOKEN_RIGHT_CURLY_BRACES);
		pop_table(yyoutS);
	}
	else errorRecovery("COMP_STMT",t);
}

void parse_PARAM_LIST_Tag(TableEntry* head)
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_COMMA:	
						fprintf(yyoutP, "Rule(PARAM_LIST_Tag -> , PARAM PARAM_LIST_Tag)\n");
						TableEntry* param = parse_PARAM();
						TableEntry* ptr = head;
						while (ptr != NULL)
						{
							if (ptr->nextParam == NULL)
							{
								ptr->nextParam = param;
								break;
							}
							ptr = ptr->nextParam;
						}
						parse_PARAM_LIST_Tag(head);
						break;	
	case TOKEN_RIEHT_PARENTHESIS:
									back_token();
									fprintf(yyoutP, "Rule(PARAM_LIST_Tag -> epsilon )\n");
									break;
	default: 
			errorRecovery("PARAM_LIST_Tag",t);
	}
}

TableEntry* parse_PARAM_LIST()
{
	TableEntry* head = NULL;
	fprintf(yyoutP, "Rule(PARAM_LIST -> PARAM PARAM_LIST_Tag)\n");
	head = parse_PARAM();
	parse_PARAM_LIST_Tag(head);
	return head;
}

TableEntry* parse_PARAM()
{
	TableEntry* param = (TableEntry*)calloc(1, sizeof(TableEntry));
	fprintf(yyoutP, "Rule(PARAM -> TYPE id PARAM_Tag )\n");
	param->entry_type = parse_TYPE();
	match(TOKEN_ID);
	param->name = t->lexeme;
	param->line = t->lineNumber;
	parse_PARAM_Tag(param);
	return param;
}

void parse_PARAM_Tag(TableEntry* entry)
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_COMMA: 
	case TOKEN_RIEHT_PARENTHESIS: 
		entry->entry_role = Variable;
		
									back_token();
									fprintf(yyoutP, "Rule(PARAM_Tag -> epsilon)\n");
									break;
	case TOKEN_LEFT_BRACKET:
		entry->entry_role = Array;
							fprintf(yyoutP, "Rule(PARAM_Tag -> [ DIM_SIZES ])\n");
							entry->arrayDimnestions = parse_DIM_SIZES();
							match(TOKEN_RIGHT_BRACKET);
							break;
	default: 
			errorRecovery("PARAM_Tag",t);
	}
}

TableEntry* parse_PARAMS()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_KEYWORD_INT: 
	case TOKEN_KEYWORD_FLOAT:
								fprintf(yyoutP, "Rule(PARAMS -> PARAM_LIST)\n");
								back_token();
								return parse_PARAM_LIST();
								
	case TOKEN_RIEHT_PARENTHESIS:
									fprintf(yyoutP, "Rule(PARAMS -> epslion)\n");
									back_token();
									return NULL;
	default: 
			errorRecovery("PARAMS",t);
	}
}

EntryType parse_RETURNED_TYPE()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_KEYWORD_INT: 
	case TOKEN_KEYWORD_FLOAT:
							fprintf(yyoutP, "Rule(RETURNED_TYPE -> TYPE)\n");
							back_token();
							return parse_TYPE(); 
							
	case TOKEN_KEYWORD_VOID:
							fprintf(yyoutP, "Rule(RETURNED_TYPE -> void )\n");
							return Void;
	default: 
			errorRecovery("RETURNED_TYPE",t);
	}
}

TableEntry* parse_FUNC_PROTOTYPE(int isNeedToInsert)
{
	TableEntry* id_entry = (TableEntry*)calloc(1, sizeof(TableEntry));
	fprintf(yyoutP, "Rule(FUNC_PROTOTYPE -> RETURNED_TYPE id (PARAMS)) \n");
	id_entry->entry_type =  parse_RETURNED_TYPE();
	id_entry->entry_role = Function;
	match(TOKEN_ID);
	id_entry->name = t->lexeme;
	match(TOKEN_LEFT_PARENTHESIS);
	id_entry->nextParam = parse_PARAMS();
	match(TOKEN_RIEHT_PARENTHESIS);
	return id_entry;
}

int parse_DIM_SIZES_Tag()
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_COMMA:	
							match(TOKEN_INTEGER_NUMBER);
							fprintf(yyoutP, "Rule(DIM_SIZES_Tag -> , int_num DIM_SIZES_Tag ) \n");
							return parse_DIM_SIZES_Tag() + 1;
							break;
	case TOKEN_RIGHT_BRACKET: 
							fprintf(yyoutP, "Rule(DIM_SIZES_Tag -> epslion) \n");
							back_token();
							return 0;
	default: 
			errorRecovery("DIM_SIZES_Tag",t); break;
	}
}

int parse_DIM_SIZES()
{

	t = next_token();
	if (t->kind != TOKEN_INTEGER_NUMBER)
		errorRecovery("DIM_SIZES",t);
	else 
    {
		fprintf(yyoutP, "Rule(DIM_SIZES -> int_num DIM_SIZES_Tag) \n");
		return parse_DIM_SIZES_Tag() + 1;
	}
}

EntryType parse_TYPE()
{   
	if (first == 1) 
	{
		t = getFirst();
		first = 0;
	}
	else
	{
        t = next_token();
	}
	switch (t->kind)
	{
	case TOKEN_KEYWORD_INT: 
							fprintf(yyoutP, "Rule(TYPE -> int) \n");
							return Integer;
	case TOKEN_KEYWORD_FLOAT: 
							fprintf(yyoutP, "Rule(TYPE -> float) \n");
							return Float;
	default: errorRecovery("TYPE",t); break;
	}
}

void parse_VAR_DEC()
{
	fprintf(yyoutP, "Rule(VAR_DEC -> TYPE id VAR_DEC_Tag) \n");
	EntryType type = parse_TYPE();
	match(TOKEN_ID);
	TableEntry* id_entry = insert(t->lexeme);
	if (id_entry == NULL)
	{
		fprintf(yyoutS, "{Line %d} - var '%s' is already declared\n", t->lineNumber , t->lexeme);
	}
	else
	{
		id_entry->entry_type = type;
		id_entry->line = t->lineNumber;
	}
	parse_VAR_DEC_Tag(id_entry);
}

void parse_VAR_DEC_Tag(TableEntry* entry)
{
	t = next_token();
	switch (t->kind)
	{
	case TOKEN_SEMICOLON:
		if (entry != NULL)
		{
			entry->entry_role = Variable;
		}
		fprintf(yyoutP, "Rule(VAR_DEC' -> ;) \n");
		break;
	case TOKEN_LEFT_BRACKET:	
		fprintf(yyoutP, "Rule(VAR_DEC' -> [DIM_SIZES];) \n");
		int num = parse_DIM_SIZES();
		match(TOKEN_RIGHT_BRACKET);
		match(TOKEN_SEMICOLON);
		if (entry != NULL)
		{
			entry->entry_role = Array;
			entry->arrayDimnestions = num;

		}
							break;	
	default: 
			errorRecovery("VAR_DEC_Tag", t); break;
	}
}


void match(eTOKENS kind)
{
	t = next_token();
	if (t->kind == TOKEN_EOF)
		back_token();
	if (t->kind != kind)
	{
		error(t, kind);
	}
}


int checkForDuplicate(TableEntry* head)
{
	char** list = (char**)calloc(50, sizeof(char*));
	for (int i = 0; i < 50; i++)
	{
		list[i] = NULL;
	}
	TableEntry* ptr = head;
	while (ptr != NULL)
	{
		for (int i = 0; i < 50; i++)
		{
			if (list[i] == NULL)
			{
				list[i] = ptr->name;
				break;
			}
		}
		ptr = ptr->nextParam;
	}
	int size = 0;
	for (int i = 0; i < 50; i++)
	{
		if (list[i] == NULL)
		{
			break;
		}
		else
		{
			size++;
		}
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i != j)
			{
				if (strcmp(list[i], list[j]) == 0)
				{
					return 0;
				}
			}

		}
	}
	return 1;

}


void insertFunctionToTable(TableEntry* entry,int isFunctionImplemented)
{
	TableEntry* id_entry = insert(entry->name);
	if (id_entry == NULL)
	{
		fprintf(yyoutS, "{Line %d} - func '%s' is already declared\n", t->lineNumber, entry->name);
	}
	else
	{
		id_entry->isFunctionImplemented = isFunctionImplemented;
		id_entry->entry_role = Function;
		id_entry->entry_type = entry->entry_type;
		id_entry->line = t->lineNumber;
		id_entry->nextParam = entry->nextParam;
	}
}

void assertEqualsFunction(TableEntry* left,TableEntry* right)
{
	if (left->entry_type != right->entry_type)
	{
		fprintf(yyoutS, "{Line %d} - invalid function - '%s' implement \n", t->lineNumber, left->name);
		return;
	}
	int sizeLeft = 0;
	int sizeRight = 0;
	TableEntry* leftPtr = left->nextParam;
	TableEntry* rightPtr = right->nextParam;
	while (leftPtr != NULL)
	{
		sizeLeft++;
		leftPtr = leftPtr->nextParam;
	}
	while (rightPtr != NULL)
	{
		sizeRight++;
		rightPtr = rightPtr->nextParam;
	}
	if (sizeLeft != sizeRight)
	{
		fprintf(yyoutS, "{Line %d} - invalid function - '%s' implement \n", t->lineNumber, left->name);
		return;
	}
	leftPtr = left->nextParam;
	rightPtr = right->nextParam;
	for (int i = 0; i < sizeLeft; i++)
	{
		if (leftPtr->entry_role != rightPtr->entry_role)
		{
			fprintf(yyoutS, "{Line %d} - invalid function - '%s' implement \n", t->lineNumber, left->name);
			return;
		}
		else if (leftPtr->entry_type != rightPtr->entry_type)
		{
			fprintf(yyoutS, "{Line %d} - invalid function - '%s' implement \n", t->lineNumber, left->name);
			return;
		}	
		leftPtr = leftPtr->nextParam;
		rightPtr = rightPtr->nextParam;
	}
	left->isFunctionImplemented = 1;
	return;
}


void insertParamsToTable(TableEntry* entry)
{

	TableEntry* ptr = entry;
	while (ptr != NULL)
	{

		TableEntry* id_entry = insert(ptr->name);
		if (id_entry == NULL)
		{
			fprintf(yyoutS, "{Line %d} - var '%s' is already declared\n", t->lineNumber, ptr->name);
		}
		else
		{
			id_entry->entry_type = ptr->entry_type;
			id_entry->line = ptr->line;
			id_entry->arrayDimnestions = ptr->arrayDimnestions;
			id_entry->entry_role = ptr->entry_role;
		}
		ptr = ptr->nextParam;
	}
}

EntryType calculteEntryType(EntryType first, EntryType second)
{
	if (first == Exception || second == Exception)
	{
		return Exception;
	}
	if (second == Float || first == Float)
	{
		return Float;
	}
	return Integer;

}