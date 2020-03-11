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

        void AddParams(cVarDeclsNode * params) 
        { 
            params->SetIsArgs(true);
            AddChild(params); 
        }

        cVarDeclsNode * GetParams()
        {
            return dynamic_cast<cVarDeclsNode*>(GetChild(1));
        }

        void AddBlock(cBlockNode * block) 
        { 
            block->SetIsFuncBlock(true);
            AddChild(block); 
        }

        cBlockNode * GetBlock()
        {
            return dynamic_cast<cBlockNode*>(GetChild(2));            
        }

        string GetName()
        {
            return dynamic_cast<cSymbol*>(GetChild(0))->GetName();
        }

        virtual string NodeType() { return string("proc"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string GetTypeName() { return "Procedure"; }
        virtual cDeclNode* GetType() { return nullptr; }
};
