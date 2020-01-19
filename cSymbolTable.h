#pragma once
//**************************************************
// cSymbolTable.h
// cSymbolTable class definition
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//
// Date: 1/18/2020

#include <unordered_map>
#include <string>
#include "cSymbol.h"

using std::string;
using std::unordered_map;

class cSymbolTable
{
  public:
    cSymbolTable() : _topTableIndex(-1) {}
    void IncreaseScope();
    void DecreaseScope();
    void InsertSymbol(cSymbol * symbol);
    cSymbol * LookupSymbol(string name);
    cSymbol * LookupSymbolLocal(string name);
  private:
    unordered_map<int, unordered_map<string, cSymbol*>> _symbolTables;
    int _topTableIndex;
};
