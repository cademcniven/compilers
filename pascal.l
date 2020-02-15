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
#include "astnodes.h"
#include "cSymbolTable.h"
#include "pascalparse.h"
#include "lex.h"

using std::string;

        // comment out the other line to generate debug info
#define DO_RETURN(a) return (a)
//#define DO_RETURN(a) return Return(a)
int Return(int val);
string ToLower(string str);
int ProcessIdentifier();
cSymbol *  Insert(string name);

extern cSymbolTable g_symbolTable;

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

        /* definitions go here. PUNCTUATION is an example. */
PUNCTUATION                  [:()\[\];,.+\-*/=^<>]
IDENTIFIER                   [a-zA-Z][a-zA-Z0-9]*

%%

        /* token definitions go here. "if" is an example. */
"{".*"}"
{IDENTIFIER}           DO_RETURN(ProcessIdentifier());
[0-9]+                 {    yylval.int_val = std::atoi(yytext);
                            DO_RETURN(INT_VAL);}
[0-9]+\.[0-9]+         {    yylval.real_val = std::atof(yytext);
                            DO_RETURN(REAL_VAL);}
":="                   DO_RETURN(ASSIGN);
">="                   DO_RETURN(GE);
"<="                   DO_RETURN(LE);
"<>"                   DO_RETURN(NOT_EQUAL);
{PUNCTUATION}          DO_RETURN(yytext[0]);
[ \n\t\r]
.                      DO_RETURN(JUNK_TOKEN);

%%

//*************************************************************
// This function allows us to do extra processing on each token
int Return(int val)
{
        printf("Scanned '%s': %d\n", yytext, val);
        return val;
}

//*************************************************************
int ProcessIdentifier()
{
    string name = ToLower(yytext);

    cSymbol * symbol = g_symbolTable.LookupSymbol(name);

    if (symbol == nullptr)
        symbol = new cSymbol(name, -1);

    yylval.symbol = symbol;

    return symbol->GetTokenType();
}

//*************************************************************
string ToLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}
