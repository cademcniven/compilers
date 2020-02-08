#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines an AST node for variable declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cBaseTypeNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol * identifier, cSymbol * type) : cDeclNode()
        {
            AddChild(new cBaseTypeNode(type->GetName(), 
                type->GetSize(), type->IsFloat()));
            
            cSymbol * symbol = g_symbolTable.LookupSymbol(identifier->GetName());
            
            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = identifier;

            AddChild(symbol);
            g_symbolTable.InsertSymbol(symbol);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
