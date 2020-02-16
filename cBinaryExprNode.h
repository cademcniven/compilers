#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines an AST node for a binary expressions.
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cExprNode *lhs, 
            int op, cExprNode *rhs) : cExprNode()
        {
            AddChild(lhs);
            AddChild(new cOpNode(op));
            AddChild(rhs);
        }

        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode * GetType()
        {
            cDeclNode * lhs = dynamic_cast<cExprNode*>(GetChild(0))->GetType();
            cDeclNode * rhs = dynamic_cast<cExprNode*>(GetChild(2))->GetType();

            if (lhs->IsReal()) return lhs;
            if (rhs->IsReal()) return rhs;
            if (lhs->IsInt()) return lhs;
            if (rhs->IsInt()) return rhs;
            if (lhs->IsChar()) return lhs;
            if (rhs->IsChar()) return rhs;

            //if it made it to here then both sides are user-defined types
            return rhs;
        }
};
