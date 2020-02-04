#pragma once
//**************************************
// cRealExprNode.h
//
// Defines an AST node for real number constants
//
// Inherits from cExprNode so that real number constants can be used anywhere 
// expressions are used.
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"

class cRealExprNode : public cExprNode
{
    public:
        cRealExprNode(float value) : cExprNode()
        {
            m_value = value;
        }

        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }
        virtual string NodeType() { return string("REAL"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        float m_value;        // value of real constant (literal)
};
