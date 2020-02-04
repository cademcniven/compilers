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

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol* value) : cExprNode()
        {
            m_value = value;
            AddChild(value);
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        cSymbol* m_value;        // value of integer constant (literal)
};
