#pragma once
//**************************************
// cArrayDeclNode.h
//
// Defines an AST node for array declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cBaseTypeNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cArrayDeclNode : public cDeclNode
{
    public:
        cArrayDeclNode(cSymbol * name, cSymbol * type, 
            cDeclsNode * ranges) : cDeclNode()
        {
            cSymbol * symbol = g_symbolTable.LookupSymbol(name->GetName());

            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = name;
           
            symbol->SetTokenType(TYPE_ID);
            symbol->SetDecl(this);
            g_symbolTable.InsertSymbol(symbol);

            AddChild(symbol);
            AddChild(new cBaseTypeNode(type->GetName(),
                type->GetSize(), type->IsFloat()));
            AddChild(ranges);
        }

        virtual string NodeType() { return string("array"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
