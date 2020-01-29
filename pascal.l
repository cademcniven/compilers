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
int ProcessIdentifier();
cSymbol *  Insert(string name);

extern cSymbolTable g_symbolTable;
yylval_t yylval;

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

        /* definitions go here. PUNCTUATION is an example. */
PUNCTUATION                  [()\[\];,.+\-*/=^<>]
IDENTIFIER                   [a-zA-Z][a-zA-Z0-9]*

%%

        /* token definitions go here. "if" is an example. */
"{".*"}"
{IDENTIFIER}           DO_RETURN(ProcessIdentifier());
[0-9]+                 DO_RETURN(INT_VAL);
[0-9]+\.[0-9]+         DO_RETURN(REAL_VAL);
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
        symbol = new cSymbol(name, -1);
        g_symbolTable.InsertSymbol(symbol);
    }

    return symbol;
}

//*************************************************************
int ProcessIdentifier()
{
    string name = ToLower(yytext);
    if (name == "begin")
    {
        IncreaseScope();
        return yytext[0];
    }

    if (name == "end")
    {
        DecreaseScope();
        return yytext[0];
    }

    cSymbol * symbol = g_symbolTable.LookupSymbol(name);
    if (symbol != nullptr && symbol->GetTokenType() != IDENTIFIER)
        return symbol->GetTokenType();

    yylval.symbol = Insert(yytext);
    return IDENTIFIER;
}

//*************************************************************
string ToLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}
