%{
#include <stdio.h>
#include <cstdlib>
#include <limits.h>
#include <cassert>
#include <string>
#include "gram.tab.hpp"

int string_pos = 1;

int check_int(){
    assert(yyleng <= 10);
    char* p_end;
    long long tmp = std::strtoll(yytext, &p_end, 10);
    assert(tmp < INT_MAX);
    return (int)(tmp);
}

int check_bin(){
    assert(yyleng <= 34);
    std::string s = std::string(yytext, yyleng);
    return std::stoi(s.substr(2), 0, 2);
}

void check_and_set_string(){
    assert(yyleng < 4096);
    yylval.word = new std::string(yytext, yyleng);
}

%}

%option yylineno
%option noyywrap

MAIN main
IF if
ELSE else
WHILE while
SKIP skip
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
POW "^"
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

{MAIN}          {check_and_set_string(); string_pos+=strlen(yytext); return MAIN; }
{IF}            {check_and_set_string(); string_pos+=strlen(yytext); return IF; }
{ELSE}          {check_and_set_string(); string_pos+=strlen(yytext); return ELSE; }
{WHILE}         {check_and_set_string(); string_pos+=strlen(yytext); return WHILE; }
{SKIP}          {check_and_set_string(); string_pos+=strlen(yytext); return SKIP;}
{INT_TYPE}      {check_and_set_string(); string_pos+=strlen(yytext); return INT_TYPE; }
{BOOL_TYPE}     {check_and_set_string(); string_pos+=strlen(yytext); return BOOL_TYPE; }
{STRING_TYPE}   {check_and_set_string(); string_pos+=strlen(yytext); return STRING_TYPE; }
{VAR}           {check_and_set_string(); string_pos+=strlen(yytext); return VAR; }
{SEP}           {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return SEP; }
{INT}           {yylval.num = check_int(); string_pos+=strlen(yytext); return INT; }
{BIN}           {yylval.num = check_bin(); string_pos+=strlen(yytext); return INT; }
{TRUE_VAL}      {yylval.boolean = true; string_pos+=strlen(yytext); return TRUE_VAL; }
{FALSE_VAL}     {yylval.boolean = false; string_pos+=strlen(yytext); return FALSE_VAL; }
{STRING}        {check_and_set_string(); string_pos+=strlen(yytext); return STRING; }
{PLUS}          {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return PLUS; }
{MINUS}         {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return MINUS; }
{MUL}           {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return MUL; }
{DIV}           {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return DIV; }
{LP}            {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return LP; }
{RP}            {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return RP; }
{LB}            {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return LB; }
{RB}            {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return RB; }
{POW}           {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return POW; }
{ASSIGN}        {yylval.sym = yytext[0]; string_pos+=strlen(yytext); return ASSIGN; }
{EQ}            {check_and_set_string(); string_pos+=strlen(yytext); return EQ; }
{NEQ}           {check_and_set_string(); string_pos+=strlen(yytext); return NEQ; }
{LT}            {check_and_set_string(); string_pos+=strlen(yytext); return LT; }
{LE}            {check_and_set_string(); string_pos+=strlen(yytext); return LE; }
{GT}            {check_and_set_string(); string_pos+=strlen(yytext); return GT; }
{GE}            {check_and_set_string(); string_pos+=strlen(yytext); return GE; }
{NOT}           {check_and_set_string(); string_pos+=strlen(yytext); return NOT; }
{AND}           {check_and_set_string(); string_pos+=strlen(yytext); return AND; }
{OR}            {check_and_set_string(); string_pos+=strlen(yytext); return OR; }

{COMMENT}       {string_pos+=strlen(yytext);}
[ \t\r]         {string_pos++;}
[\n]            {string_pos = 1;}

.               {
                  printf("ERROR in line %u, pos %u, symbol %s\n", yylineno, string_pos, yytext);
                  exit(1);
                }

%%