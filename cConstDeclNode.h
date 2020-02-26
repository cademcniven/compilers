#pragma once
//**************************************
// cConstDeclNode.h
//
// Defines an AST node for const declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cExprNode.h"

class cConstDeclNode : public cDeclNode
{
    public:
        cConstDeclNode(cSymbol * identifier, cExprNode * expr) : cDeclNode()
        {
            cSymbol * symbol = g_symbolTable.LookupSymbol(identifier->GetName());
            
            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = identifier;

            AddChild(symbol);
            g_symbolTable.InsertSymbol(symbol);

            symbol->SetDecl(this);

            AddChild(expr);
        }

        virtual string NodeType() { return string("const_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string GetTypeName() { return "const"; }
        virtual cDeclNode* GetType()
        {
            return dynamic_cast<cExprNode*>(GetChild(1))->GetType();
        }
};
