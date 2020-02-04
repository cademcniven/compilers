#pragma once
//**************************************
// cIntExprNode.h
//
// Defines an AST node for a binary expressions.
//
// Author: Cade McNiven
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cBinaryExprNode(cExprNode *lhs, 
            int op, cExprNode *rhs) : cExprNode()
        {
            AddChild(lhs);
            AddChild(new cOpNode(op));
            AddChild(rhs);
        }

        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
