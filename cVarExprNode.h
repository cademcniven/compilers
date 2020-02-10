#pragma once
//**************************************
// cVarExprNode.h
//
// Defines an AST node for variable expressions
//
// Inherits from cExprNode so that variables can be used anywhere 
// expressions are used.
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"
#include "cExprListNode.h"

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol* value) : cExprNode()
        {
            m_value = value;
            AddChild(value);
        }

        void AddExprList(cExprListNode * exprList) { AddChild(exprList); }

        // sort of unsure if this was the correct solution to the problem.
        // just doing AddChild would be in line with other similar objects,
        // but it caused the ids to be wrong. Adding code like what's in 
        // the decl nodes fixed the problem but may be incorrect.
        void AddSymbol(cSymbol * identifier) 
        { 
            cSymbol * symbol = g_symbolTable.LookupSymbol(identifier->GetName());

            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = identifier;

            g_symbolTable.InsertSymbol(symbol);
            AddChild(symbol); 
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        cSymbol* m_value;
};
