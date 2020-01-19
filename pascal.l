%{
//**************************************
// lang.l
//
// Scanner definition file for CST 320 project
//
// Authors: Phil Howard, Cade McNiven 
// phil.howard@oit.edu
// cade.mcniven@oit.edu
//
// Date: Jan 16, 2020
//

#include <string>
#include <algorithm>
#include "tokens.h"
#include "lex.h"
#include "cSymbolTable.h"

using std::string;

    // comment out the other line to generate debug info
#define DO_RETURN(a) return (a)
//#define DO_RETURN(a) return Return(a)
int Return(int val);
string ToLower(string str);
void IncreaseScope();
void DecreaseScope();
cSymbol *  Insert(string name);

extern cSymbolTable g_symbolTable;
yylval_t yylval;

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. PUNCTUATION is an example. */
PUNCTUATION          [()\[\];,.+\-*/=^<>]
IDENTIFIER           [a-zA-Z][a-zA-Z0-9]*

%%

"BEGIN"                     {
                              IncreaseScope();
                              return yytext[0];
                            }
"END"                       {
                              DecreaseScope();
                              return yytext[0];
                            }

    /* token definitions go here. "if" is an example. */
"{".*"}"
"IF"                        DO_RETURN(IF);
"ELSE"                      DO_RETURN(ELSE);
"WHILE"                     DO_RETURN(WHILE);
"PROGRAM"                   DO_RETURN(PROGRAM);
"PROCEDURE"                 DO_RETURN(PROCEDURE);
"FUNCTION"                  DO_RETURN(FUNCTION);
"THEN"                      DO_RETURN(THEN);
"REPEAT"                    DO_RETURN(REPEAT);
"UNTIL"                     DO_RETURN(UNTIL);
"FOR"                       DO_RETURN(FOR);
"DO"                        DO_RETURN(DO);
"TO"                        DO_RETURN(TO);
"DOWNTO"                    DO_RETURN(DOWNTO);
"FORWARD"                   DO_RETURN(FORWARD);
"CONST"                     DO_RETURN(CONST);
"TYPE"                      DO_RETURN(TYPE);
"VAR"                       DO_RETURN(VAR);
"ARRAY"                     DO_RETURN(ARRAY);
"RECORD"                    DO_RETURN(RECORD);
"OF"                        DO_RETURN(OF);
"NIL"                       DO_RETURN(NIL);
"integer"                   DO_RETURN(INTEGER);
"real"                      DO_RETURN(REAL);
"char"                      DO_RETURN(CHAR);
"<>"                        DO_RETURN(NOT_EQUAL);
"<="                        DO_RETURN(LE);
">="                        DO_RETURN(GE);
":="                        DO_RETURN(ASSIGN);
"OR"                        DO_RETURN(OR);
"DIV"                       DO_RETURN(DIV);
"MOD"                       DO_RETURN(MOD);
"WRITE"                     DO_RETURN(WRITE);
"AND"                       DO_RETURN(AND);
"NOT"                       DO_RETURN(NOT);
{IDENTIFIER}                {
                              yylval.symbol = Insert(yytext);
                              return IDENTIFIER;
                            }
[0-9]+                      DO_RETURN(INT_VAL);
[0-9]+\.[0-9]+              DO_RETURN(REAL_VAL);
{PUNCTUATION}               DO_RETURN(yytext[0]);
[ \n\t\r]
.                           DO_RETURN(JUNK_TOKEN);

%%

//*************************************************************
// This function allows us to do extra processing on each token
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

//*************************************************************
void IncreaseScope()
{
  g_symbolTable.IncreaseScope();
}

//*************************************************************
void DecreaseScope()
{
  g_symbolTable.DecreaseScope();
}

//*************************************************************
cSymbol *  Insert(string name)
{
  name = ToLower(name);
  cSymbol * symbol = g_symbolTable.LookupSymbolLocal(name);
  
  if (symbol == nullptr)
  {
    symbol = new cSymbol(name);
    g_symbolTable.InsertSymbol(symbol);
  }

  return symbol;
}

//*************************************************************
string ToLower(string str)
{
  transform(str.begin(), str.end(), str.begin(), ::tolower);

  return str;
}
