#include <stdio.h>
#include "Token.h"


const char* getKindName(enum eTOKENS Kind);
void error(Token *expected, eTOKENS actual);
void errorRecovery(char* parserName, Token *t);
eTOKENS* getFollowTokens(char* parserName);
int checkIfFollow(eTOKENS* arrayToken, eTOKENS target);
void printTokensArray(eTOKENS *followArray, int size);
eTOKENS* getFirstTokens(char* parserName);
void PrintFirstTokens(char* parserName, Token *token);
