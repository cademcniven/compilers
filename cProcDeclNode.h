#pragma once
//**************************************
// cProcDeclNode.h
//
// Defines an AST node for procedue declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cProcDeclNode : public cDeclNode
{
    public:
        cProcDeclNode(cSymbol * header) : cDeclNode()
        {
            cSymbol * symbol = g_symbolTable.LookupSymbol(header->GetName());

            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = header;
            
            symbol->SetDecl(this);
            g_symbolTable.InsertSymbol(symbol);

            AddChild(symbol);
        }

        void AddParams(cVarDeclsNode * params) { AddChild(params); }
        void AddBlock(cBlockNode * block) { AddChild(block); }

        virtual string NodeType() { return string("proc"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
