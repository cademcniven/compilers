#pragma once
//**************************************
// cFuncDecl.h
//
// Defines an AST node for function declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cProcDeclNode.h"

extern cSymbolTable g_symbolTable;

class cFuncDeclNode : public cDeclNode
{
    public:
        cFuncDeclNode(cSymbol * header) : cDeclNode()
        {
            cSymbol * symbol = g_symbolTable.LookupSymbol(header->GetName());

            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = header;
            
            g_symbolTable.InsertSymbol(symbol);

            AddChild(symbol);
        }

        void AddParams(cVarDeclsNode * params) { AddChild(params); }
        void AddBlock(cBlockNode * block) { AddChild(block); }
        void AddType(cSymbol * type)
        {
            AddChild(new cBaseTypeNode(type->GetName(),
                type->GetSize(), type->IsFloat()));
        }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
