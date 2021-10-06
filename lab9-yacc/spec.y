%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYDEBUG 1
%}


%token IDENTIFIER
%token CONSTANT
%token BEGINN
%token CHARACTER
%token DO
%token ELSE
%token END
%token FOR
%token FUNCTION
%token IF
%token INT
%token READ
%token RETURN
%token STRING
%token THEN
%token TO
%token VECTOR
%token WHILE
%token WRITE
%token LEFT_SQUARE_PARENTHESIS
%token RIGHT_SQUARE_PARENTHESIS
%token LEFT_ROUND_PARENTHESIS
%token RIGHT_ROUND_PARENTHESIS
%token LEFT_ACCOLADE
%token RIGHT_ACCOLADE
%token PLUS
%token MINUS
%token MULTIPLY
%token DIVISION
%token MOD
%token ASSIGNMENT
%token EQUAL
%token NOT_EQUAL
%token SMALLER
%token GREATER
%token LESS_OR_EQUAL
%token GREATER_OR_EQUAL


%start program

%%

program : FUNCTION IDENTIFIER BEGINN stmtlist END ;
declaration : type IDENTIFIER ;
type : CHARACTER | INT | STRING | VECTOR;
stmtlist : stmt | stmt stmtlist ;
stmt : simplestmt | structstmt ;
simplestmt : assignstmt | iostmt | declaration ;
structstmt : ifstmt | forstmt ;
assignstmt : IDENTIFIER ASSIGNMENT expression ; 
iostmt : READ LEFT_ROUND_PARENTHESIS IDENTIFIER RIGHT_ROUND_PARENTHESIS | WRITE LEFT_ROUND_PARENTHESIS IDENTIFIER RIGHT_ROUND_PARENTHESIS ;
ifstmt : IF condition THEN stmt | IF condition THEN stmt ELSE stmt ; 
forstmt : FOR IDENTIFIER EQUAL expression TO expression DO stmt ;
condition : expression relation expression ;
relation : SMALLER | GREATER | EQUAL | NOT_EQUAL | LESS_OR_EQUAL | GREATER_OR_EQUAL ;
expression : simpleExp | sumExp | substractExp |  | multiplyExp | divideExp | moduloExp ;
simpleExp : CONSTANT | IDENTIFIER ;
sumExp : simpleExp PLUS simpleExp ; 
substractExp : simpleExp MINUS simpleExp ;
multiplyExp : simpleExp MULTIPLY simpleExp ; 
divideExp : simpleExp DIVISION simpleExp ; 
moduloExp : simpleExp MOD simpleExp ;


%%

yyerror(char *s)
{
  printf("%s\n", s);
}

extern FILE *yyin;

main(int argc, char **argv)
{
  if (argc > 1) 
    yyin = fopen(argv[1], "r");
  if ( (argc > 2) && ( !strcmp(argv[2], "-d") ) ) 
    yydebug = 1;
  if ( !yyparse() ) 
    fprintf(stderr,"\t Success! \n");
}