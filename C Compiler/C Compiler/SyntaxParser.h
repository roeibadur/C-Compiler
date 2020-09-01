#include <stdio.h>
#include "Token.h"
#include "ErrorHandling.h"
#include "symbolTable.h"
void parse_PROG();
void parse_GLOBAL_VARS();
void parse_GLOBAL_VARS_Tag();
void parse_FUNC_WITH_BODY();
void parse_FUNC_FULL_DEFS();
void parse_STMT_Tag(char* lexme);
EntryType parse_FACTOR_Tag(char*);
EntryType parse_TERM_Tag();
EntryType parse_EXPR_Tag();
int parse_EXPR_LIST_Tag();
int parse_VAR_Tag();
EntryType parse_RETURN_STMT_Tag();
int parse_ARG_LIST_Tag();
void parse_FUNC_FULL_DEFS_Tag();
EntryType parse_FACTOR();
EntryType parse_TERM();
EntryType parse_EXPR();
void parse_CONDITION();
int parse_EXPR_LIST();
void parse_VAR();
void parser(int);
EntryType parse_RETURN_STMT();
int parse_ARG_LIST();
int parse_ARGS();
void parse_CALL();
void parse_IF_STMT();
void parse_STMT_LIST_Tag();
void parse_STMT_LIST();
void parse_STMT();
void parse_VAR_DEC_LIST_Tag();
void parse_VAR_DEC_LIST();
void parse_COMP_STMT();
void insertFunctionToTable(TableEntry* entry);
void parse_PARAM_LIST_Tag(TableEntry*);
TableEntry* parse_PARAM_LIST();
void insertParamsToTable(TableEntry* entry);
void insertFunctionToTable(TableEntry* entry, int isFunctionImplemented);
int checkForDuplicate(TableEntry* head);
EntryType calculteEntryType(EntryType first, EntryType second);
void assertEqualsFunction(TableEntry* left, TableEntry* right);
void match(eTOKENS t);
TableEntry* parse_PARAM();
void parse_PARAM_Tag(TableEntry*);
TableEntry* parse_PARAMS();
EntryType parse_RETURNED_TYPE();
TableEntry* parse_FUNC_PROTOTYPE(int isNeedToInsert);
int parse_DIM_SIZES_Tag();
int parse_DIM_SIZES();
EntryType parse_TYPE();
void parse_VAR_DEC();
void parse_VAR_DEC_Tag(TableEntry*);

