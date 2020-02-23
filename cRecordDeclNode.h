#pragma once
//**************************************
// cRecordDeclNode.h
//
// Defines an AST node for record declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cRecordDeclNode : public cDeclNode
{
    public:
        cRecordDeclNode(cSymbol * name, cVarDeclsNode * decls) : cDeclNode() 
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
            AddChild(decls);
        }

        virtual string NodeType() { return string("record"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string GetTypeName() { return "record"; }
};
