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
