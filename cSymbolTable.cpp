//****************************************************
// cSymbolTable.cpp
// implements the class definition specified by cSymbolTable.h
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//
// Date: 1/19/2020

#include <algorithm>
#include "cSymbolTable.h"

//*********************************************************************
void cSymbolTable::IncreaseScope()
{
    unordered_map<string, cSymbol*> table;
    _symbolTables[++_topTableIndex] = table;
}

//*********************************************************************
void cSymbolTable::DecreaseScope()
{
    if (!_symbolTables.empty())
        _symbolTables.erase(_topTableIndex--);
}

//*********************************************************************
void cSymbolTable::InsertSymbol(cSymbol * symbol)
{
    if (_symbolTables.empty())
        return;

    _symbolTables[_topTableIndex][symbol->GetName()] = symbol;
    
}

//*********************************************************************
cSymbol * cSymbolTable::LookupSymbol(string name)
{
    cSymbol * symbol = nullptr;

    for (int i = _topTableIndex; i >= 0; --i)
    {
        if (_symbolTables[i].find(name) != _symbolTables[i].end())
        {
            symbol = _symbolTables[i][name];
            break; 
        }
    }

    return symbol;
}

//*********************************************************************
cSymbol * cSymbolTable::LookupSymbolLocal(string name)
{
    cSymbol * symbol = nullptr;

    if (_symbolTables.empty())
        return symbol;

    if (_symbolTables[_topTableIndex].find(name) != _symbolTables[_topTableIndex].end())
        symbol = _symbolTables[_topTableIndex][name];

    return symbol;
}

//*********************************************************************
void cSymbolTable::InsertKeywords()
{
    if (!_symbolTables.empty())
        return;

    IncreaseScope();

    cSymbol * keyword = new cSymbol("char", 1);
    InsertSymbol(keyword);

    keyword = new cSymbol("integer", 2);
    InsertSymbol(keyword);

    keyword = new cSymbol("real", 3);
    InsertSymbol(keyword);

    keyword = new cSymbol("begin", OPEN);
    InsertSymbol(keyword);

    keyword = new cSymbol("end", CLOSE);
    InsertSymbol(keyword);

    keyword = new cSymbol("if", IF);
    InsertSymbol(keyword);

    keyword = new cSymbol("else", ELSE);
    InsertSymbol(keyword);

    keyword = new cSymbol("while", WHILE);
    InsertSymbol(keyword);

    keyword = new cSymbol("program", PROGRAM);
    InsertSymbol(keyword);

    keyword = new cSymbol("procedure", PROCEDURE);
    InsertSymbol(keyword);

    keyword = new cSymbol("function", FUNCTION);
    InsertSymbol(keyword);

    keyword = new cSymbol("then", THEN);
    InsertSymbol(keyword);

    keyword = new cSymbol("repeat", REPEAT);
    InsertSymbol(keyword);

    keyword = new cSymbol("until", UNTIL);
    InsertSymbol(keyword);

    keyword = new cSymbol("for", FOR);
    InsertSymbol(keyword);

    keyword = new cSymbol("do", DO);
    InsertSymbol(keyword);

    keyword = new cSymbol("to", TO);
    InsertSymbol(keyword);

    keyword = new cSymbol("downto", DOWNTO);
    InsertSymbol(keyword);

    keyword = new cSymbol("forward", FORWARD);
    InsertSymbol(keyword);

    keyword = new cSymbol("const", CONST);
    InsertSymbol(keyword);

    keyword = new cSymbol("type", TYPE);
    InsertSymbol(keyword);

    keyword = new cSymbol("var", VAR);
    InsertSymbol(keyword);

    keyword = new cSymbol("array", ARRAY);
    InsertSymbol(keyword);

    keyword = new cSymbol("record", RECORD);
    InsertSymbol(keyword);

    keyword = new cSymbol("of", OF);
    InsertSymbol(keyword);

    keyword = new cSymbol("nil", NIL);
    InsertSymbol(keyword);

    keyword = new cSymbol("or", OR);
    InsertSymbol(keyword);

    keyword = new cSymbol("div", DIV);
    InsertSymbol(keyword);

    keyword = new cSymbol("mod", MOD);
    InsertSymbol(keyword);

    keyword = new cSymbol("write", WRITE);
    InsertSymbol(keyword);

    keyword = new cSymbol("and", AND);
    InsertSymbol(keyword);

    keyword = new cSymbol("not", NOT);
    InsertSymbol(keyword);
}
