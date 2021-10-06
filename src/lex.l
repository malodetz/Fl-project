%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  T_KEYWORD = 1, T_SEPARATOR = 2, T_SYMBOL = 3, T_STATE = 4
};
int string_pos = 1;
%}

%option yylineno
%option noyywrap

KEYWORD     states|start|terminals|alphabet|transitions
SEPARATOR    :|;
SYMBOL     \'[A-Za-z_0-9]*\'
STATE     [A-Za-z_0-9]*

%%

{KEYWORD}       {return T_KEYWORD; }
{SEPARATOR}        {return T_SEPARATOR; }
{SYMBOL}        {return T_SYMBOL; }
{STATE}        {return T_STATE; }

[ \t\r]   {string_pos++;}
[\n] {string_pos = 1;}

. {
  printf("ERROR in line %u, pos %u, symbol %s\n", yylineno, string_pos, yytext);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }
    if (t == T_KEYWORD) {
      printf("T_KEYWORD %s %d %d\n", yytext, yylineno, string_pos);
      string_pos+=strlen(yytext);
    } else if (t == T_SEPARATOR) {
      printf("T_SEPARATOR %s %d %d\n", yytext, yylineno, string_pos);
      string_pos+=strlen(yytext);
    } else if (t == T_SYMBOL) {
      printf("T_SYMBOL %s %d %d\n", yytext, yylineno, string_pos);
      string_pos+=strlen(yytext);
    }else if (t == T_STATE) {
      printf("T_STATE %s %d %d\n", yytext, yylineno, string_pos);
      string_pos+=strlen(yytext);
    }
  };
  return EXIT_SUCCESS ;
}