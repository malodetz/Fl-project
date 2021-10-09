%{
#include <stdio.h>
#include <stdlib.h>

int string_pos = 1;
enum token{
   MAIN = 1 , IF = 2 , ELSE = 3 , WHILE = 4 , INT_TYPE = 5 , BOOL_TYPE = 6 , STRING_TYPE = 7 , VAR = 8 , SEP = 9 ,
   COMMENT = 10 , INT = 11 , BIN = 12 , TRUE_VAL = 13 , FALSE_VAL = 14 , STRING = 15 , PLUS = 16 , MINUS = 17 ,
   MUL = 18 , DIV = 19 , LP = 20 , RP = 21 , LB = 22 , RB = 23 , XOR = 24 , ASSIGN = 25 , EQ = 26 , NEQ = 27 , LT = 28 ,
   LE = 29 , GT = 30 , GE = 31 , NOT = 32 , AND = 33 , OR = 34
};

//TODO: add transfrom BIN to INT and check numbers size
%}

%option yylineno
%option noyywrap

MAIN main
IF if
ELSE else
WHILE while
INT_TYPE Int
BOOL_TYPE Bool
STRING_TYPE String
TRUE_VAL True
FALSE_VAL False
VAR [a-z][a-zA-Z0-9_]*
SEP ;
COMMENT \/\/.*
INT  [0-9]+
BIN 0b[0-1]+
STRING \'[^\'\"]*\'|\"[^\'\"]*\"
PLUS "+"
MINUS "-"
MUL "*"
DIV "/"
LP "("
RP ")"
LB "{"
RB "}"
XOR "^"
ASSIGN "="
EQ "=="
NEQ "!="
LT "<"
LE "<="
GT ">"
GE ">="
NOT "!"
AND "&&"
OR "||"

%%

{MAIN}          {string_pos+=strlen(yytext); return MAIN; }
{IF}          {string_pos+=strlen(yytext); return IF; }
{ELSE}          {string_pos+=strlen(yytext); return ELSE; }
{WHILE}          {string_pos+=strlen(yytext); return WHILE; }
{INT_TYPE}          {string_pos+=strlen(yytext); return INT_TYPE; }
{BOOL_TYPE}          {string_pos+=strlen(yytext); return BOOL_TYPE; }
{STRING_TYPE}          {string_pos+=strlen(yytext); return STRING_TYPE; }
{VAR}          {string_pos+=strlen(yytext); return VAR; }
{SEP}          {string_pos+=strlen(yytext); return SEP; }
{INT}          {string_pos+=strlen(yytext); return INT; }
{BIN}          {string_pos+=strlen(yytext); return BIN; }
{TRUE_VAL}          {string_pos+=strlen(yytext); return TRUE_VAL; }
{FALSE_VAL}          {string_pos+=strlen(yytext); return FALSE_VAL; }
{STRING}          {string_pos+=strlen(yytext); return STRING; }
{PLUS}          {string_pos+=strlen(yytext); return PLUS; }
{MINUS}          {string_pos+=strlen(yytext); return MINUS; }
{MUL}          {string_pos+=strlen(yytext); return MUL; }
{DIV}          {string_pos+=strlen(yytext); return DIV; }
{LP}          {string_pos+=strlen(yytext); return LP; }
{RP}          {string_pos+=strlen(yytext); return RP; }
{LB}          {string_pos+=strlen(yytext); return LB; }
{RB}          {string_pos+=strlen(yytext); return RB; }
{XOR}          {string_pos+=strlen(yytext); return XOR; }
{ASSIGN}          {string_pos+=strlen(yytext); return ASSIGN; }
{EQ}          {string_pos+=strlen(yytext); return EQ; }
{NEQ}          {string_pos+=strlen(yytext); return NEQ; }
{LT}          {string_pos+=strlen(yytext); return LT; }
{LE}          {string_pos+=strlen(yytext); return LE; }
{GT}          {string_pos+=strlen(yytext); return GT; }
{GE}          {string_pos+=strlen(yytext); return GE; }
{NOT}          {string_pos+=strlen(yytext); return NOT; }
{AND}          {string_pos+=strlen(yytext); return AND; }
{OR}          {string_pos+=strlen(yytext); return OR; }

{COMMENT}         {string_pos+=strlen(yytext);}
[ \t\r]           {string_pos++;}
[\n]              {string_pos = 1;}

<<EOF>>           {return 0;}

.                 {
                    printf("ERROR in line %u, pos %u, symbol %s\n", yylineno, string_pos, yytext);
                    exit(1);
                  }

%%

int main(int argc, char* argv[]) {
  freopen(argv[1], "r", stdin);
  while (1) {
     int t = yylex();
     if (t == 0) {
          break;
     }
    printf("%s %d\n", yytext, t);
  };
  return EXIT_SUCCESS ;
}