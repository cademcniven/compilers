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
            AddChild(type->GetDecl());
            AddChild(ranges);
        }

        virtual string NodeType() { return string("array"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string GetTypeName() { return dynamic_cast<cSymbol*>(GetChild(0))->GetName(); }
        virtual bool IsArray() { return true; }
        cDeclNode * GetElementType()
        {
            return dynamic_cast<cDeclNode*>(GetChild(1));
        }
};
