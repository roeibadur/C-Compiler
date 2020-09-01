%option noyywrap

%{
#define _CRT_SECURE_NO_WARNINGS
#include"Token.h"	
#include "ErrorHandling.h"	
#include "SyntaxParser.h"
int lineNumber = 1;
FILE  *yyoutP;
%}


digit    [0-9]
letter	 [a-zA-Z]
id		 [a-z]({letter}|{digit})*(("_")({digit}|{letter})+)*
int_num ([1-9]{digit}*)|[0]
float_num {int_num}"."{int_num}+(e|E)("+"|"-")?{int_num}  

Keyword_int "int"
Keyword_float "float"
Keyword_void "void"
Keyword_if   "if"
Keyword_return "return"

assignment_op "="
comma ","
colon ":"
semicolon ";"
left_parenthesis "("
right_parenthesis ")" 
left_brackets "["
right_brackets "]"
left_curly_braces "{"
right_curly_braces "}"
%x comment1  comment2

Less_than "<"
Greater_than ">"
Greater_than_or_equal_to ">="
Less_than_or_equal_to "<="
Is_not_equal_to "!="
Double_Equal "=="

Multiplication "*"
Plus "+"



%%
[\n]         		{ 		++lineNumber; }
							
[ \t]           			{}

"/*"         				BEGIN(comment1);
<comment1>[^*\n]*        	/* eat anything that's not a '*' */
<comment1>"*"+[^*/\n]*  	/* eat up '*'s not followed by '/'s */
<comment1>\n             	++lineNumber;
<comment1>"*"+"/"        	BEGIN(INITIAL);


"//"   						BEGIN(comment2);
<comment2>[^\n]             /*do nothing till new line */
<comment2>\n   				{lineNumber++;
							BEGIN(INITIAL);}






{Keyword_int}		{		create_and_store_token(TOKEN_KEYWORD_INT,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_KEYWORD_INT lexeme %s\n", lineNumber, yytext);
							return 1;}
							
{Keyword_float}		{		create_and_store_token(TOKEN_KEYWORD_FLOAT,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_KEYWORD_FLOAT lexeme %s\n", lineNumber, yytext); 
							return 1;}
							
{Keyword_void}		{		create_and_store_token(TOKEN_KEYWORD_VOID,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_KEYWORD_VOID lexeme %s\n", lineNumber, yytext); 
							return 1;}
							

{Keyword_if}		{		create_and_store_token(TOKEN_KEYWORD_IF,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_KEYWORD_IF lexeme %s\n", lineNumber, yytext); 
							return 1;}	

{Keyword_return}	{		create_and_store_token(TOKEN_KEYWORD_RETURN,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_KEYWORD_RETURN lexeme %s\n", lineNumber, yytext); 
							return 1;}					

{id}            	{       create_and_store_token(TOKEN_ID,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_ID lexeme %s\n", lineNumber, yytext); 
							return 1;}


{Greater_than_or_equal_to} {	create_and_store_token(TOKEN_GREATER_THAN_OR_EQUAL_TO ,  yytext, lineNumber); 
								fprintf(yyout,"Line %d : found token of type TOKEN_GREATER_THAN_OR_EQUAL lexeme %s\n", lineNumber, yytext); 
								return 1;}


{Less_than_or_equal_to} {	create_and_store_token(TOKEN_LESS_THAN_OR_EQUAL_TO ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_LESS_THAN_OR_EQUAL_TO lexeme %s\n", lineNumber, yytext); 
							return 1;}
							
{Less_than} 	    {  		create_and_store_token(TOKEN_LESS_THAN,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_LESS_THAN lexeme %s\n", lineNumber, yytext); 
							return 1;}
							
							
{Greater_than}      {		create_and_store_token(TOKEN_GREATER_THAN,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_GREATER_THAN lexeme %s\n", lineNumber, yytext); 
							return 1;}

{Is_not_equal_to}  {        create_and_store_token(TOKEN_IS_NOT_EQUAL_TO ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_IS_NOT_EQUAL_TO lexeme %s\n", lineNumber, yytext); 
							return 1;}


{Double_Equal}  {           create_and_store_token(TOKEN_DOUBLE_EQUAL ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_DOUBLE_EQUAL lexeme %s\n", lineNumber, yytext); 
							return 1;}

{Multiplication}   {        create_and_store_token(TOKEN_MULTIPLICATION ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_MULTIPLICATION lexeme %s\n", lineNumber, yytext); 
							return 1;}

{Plus}			   {         create_and_store_token(TOKEN_PLUS ,  yytext, lineNumber); 
							 fprintf(yyout,"Line %d : found token of type TOKEN_PLUS lexeme %s\n", lineNumber, yytext); 
							 return 1;}
							 
							 
{float_num}     	{ 		create_and_store_token(TOKEN_FLOAT_NUMBER,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_FLOAT_NUMBER lexeme %s\n", lineNumber, yytext); 
							return 1;}							 
							

{int_num}       	{ 		create_and_store_token(TOKEN_INTEGER_NUMBER,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_INTEGER_NUMBER lexeme %s\n", lineNumber, yytext); 
							return 1;} 
							 	        

{assignment_op} 	{ 		create_and_store_token(TOKEN_OP_EQUAL ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_OP_EQUAL lexeme %s\n", lineNumber, yytext);  
							return 1;}
							

{comma}         	{		create_and_store_token(TOKEN_COMMA ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_COMMA lexeme %s\n", lineNumber, yytext);  	
							return 1;} 


{colon}         	{		create_and_store_token(TOKEN_COLON ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type comma lexeme %s\n", lineNumber, yytext);  
							return 1;} 


{semicolon}     	{		create_and_store_token(TOKEN_SEMICOLON ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_SEMICOLON  lexeme %s\n", lineNumber, yytext);  
							return 1;} 


{left_parenthesis}  {		create_and_store_token(TOKEN_LEFT_PARENTHESIS ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_LEFT_PARENTHESIS  lexeme %s\n", lineNumber, yytext);  	
							return 1;} 


{right_parenthesis} {		create_and_store_token(TOKEN_RIEHT_PARENTHESIS ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_RIEHT_PARENTHESIS  lexeme %s\n", lineNumber, yytext);  
							return 1;} 
 

{left_brackets}    	{		create_and_store_token(TOKEN_LEFT_BRACKET ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_LEFT_BRACKET %s\n", lineNumber, yytext);  
							return 1;}

{right_brackets}   	{		create_and_store_token(TOKEN_RIGHT_BRACKET ,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_RIGHT_BRACKET lexeme %s\n", lineNumber, yytext);  
							return 1;}

{left_curly_braces} {		create_and_store_token(TOKEN_LEFT_CURLY_BRACES,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_LEFT_CURLY_BRACES lexeme %s\n", lineNumber, yytext);  
							return 1;}



{right_curly_braces} {		create_and_store_token(TOKEN_RIGHT_CURLY_BRACES,  yytext, lineNumber); 
							fprintf(yyout,"Line %d : found token of type TOKEN_RIGHT_CURLY_BRACES lexeme %s\n", lineNumber, yytext); 
							return 1;}

<<EOF>>    			{		create_and_store_token(TOKEN_EOF, "", lineNumber);
							fprintf(yyout,"Line %d : found token of type TOKEN_EOF\n", lineNumber);
							return 0;}							
		
						
.       			{		fprintf(yyout,"Line %d : Unrecognized character!: %s\n", lineNumber, yytext );
							return 1;}

%%

void main()
{

int num;
printf("press 1 for test1 or press 2 for test2\n");
scanf("%d", &num);
if (num == 1)
{    
	yyin=fopen("C:\\temp\\test1.txt","r"); 
	yyout=fopen("C:\\temp\\test1_313299505_312350671_lex.txt","w");		 
				
}
else if(num == 2)
{
	yyin=fopen("C:\\temp\\test2.txt","r"); 
	yyout=fopen("C:\\temp\\test2_313299505_312350671_lex.txt","w");  
				 
}
else
{
	printf("chosen Default test1\n");
	yyin=fopen("C:\\temp\\test1.txt","r"); 
	yyout=fopen("C:\\temp\\test1_313299505_312350671_lex.txt","w");
}

if(yyin == NULL || yyout == NULL  )
{ 
    printf("The Files not opend !\n");
	return; 
}
while( yylex()!=0){} 		
fclose(yyout);		
if ( num == 1)
{
	yyoutP = fopen("C:\\temp\\test1_313299505_312350671_syntactic.txt","w");
}
else if(num == 2)
{
    yyoutP = fopen("C:\\temp\\test2_313299505_312350671_syntactic.txt","w");
}
else{yyoutP = fopen("C:\\temp\\test1_313299505_312350671_syntactic.txt","w"); }
		
if(yyoutP == NULL )
{ 
    printf("The Files not opend !\n");
	return; 
}        
parser(); 
freeList();
printf("The test Finished\n");	
fclose(yyin);
fclose(yyoutP);
}