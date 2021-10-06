%{
#include <stdio.h>
#include <stdlib.h>

int string_pos = 1;
enum token{
    MAIN = 1 , IF = 2 , ELSE = 3 , WHILE = 4 , INT_TYPE = 5 , STRING_TYPE = 6 , VAR = 7 , SEP = 8 , INT = 9 , BIN = 10 ,
    STRING = 11 , PLUS = 12 , MINUS = 13 , MUL = 14 , DIV = 15 , LP = 16 , RP = 17 , LB = 18 , RB = 19 , XOR = 20 ,
    ASSIGN = 21 , EQ = 22 , NEQ = 23 , LT = 24 , LE = 25 , GT = 26 , GE = 27 , NOT = 28 , AND = 29 , OR = 30
};

//TODO: add transfrom BIN to INf and check numbers size
%}

%option yylineno
%option noyywrap

MAIN main
IF if
ELSE else
WHILE while
INT_TYPE Int
STRING_TYPE String
VAR [a-z][a-zA-Z0-9_]*
SEP ;
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
{STRING_TYPE}          {string_pos+=strlen(yytext); return STRING_TYPE; }
{VAR}          {string_pos+=strlen(yytext); return VAR; }
{SEP}          {string_pos+=strlen(yytext); return SEP; }
{INT}          {string_pos+=strlen(yytext); return INT; }
{BIN}          {string_pos+=strlen(yytext); return BIN; }
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