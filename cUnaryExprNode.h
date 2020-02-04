#pragma once
//**************************************
// cUnaryExprNode.h
//
// Defines an AST node for unary expressions.
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cOpNode.h"

class cUnaryExprNode : public cExprNode
{
    public:
        cUnaryExprNode(int op, cExprNode *rhs) : cExprNode()
        {
            AddChild(new cOpNode(op));
            AddChild(rhs);
        }

        virtual string NodeType() { return string("unaryExpr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
