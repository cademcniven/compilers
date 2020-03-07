#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines an AST node for a binary expressions.
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "astnodes.h"

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
        virtual bool IsBinaryExpr() { return true; }
        cExprNode * GetLhs()
        {
            return dynamic_cast<cExprNode*>(GetChild(0));
        }
        
        cExprNode * GetRhs()
        {
            return dynamic_cast<cExprNode*>(GetChild(2));
        }

        cOpNode * GetOp()
        {
            return dynamic_cast<cOpNode*>(GetChild(1));
        }

        virtual cDeclNode * GetType()
        {
            cExprNode * lhs = dynamic_cast<cExprNode*>(GetChild(0));
            cExprNode * rhs = dynamic_cast<cExprNode*>(GetChild(2));

            //if the lhs or rhs is a binaryExpr, recursively find the type
            cDeclNode * lhsType = (lhs->IsBinaryExpr()) 
                    ? lhs->GetType()
                    : dynamic_cast<cExprNode*>(GetChild(0))->GetType();
            cDeclNode * rhsType = (rhs->IsBinaryExpr()) 
                    ? rhs->GetType()
                    : dynamic_cast<cExprNode*>(GetChild(2))->GetType();

            if (lhsType->IsReal()) return lhsType;
            if (rhsType->IsReal()) return rhsType;
            if (lhsType->IsChar() && 
                (!rhsType->IsChar() && rhsType->IsInt())) return rhsType;
            if (rhsType->IsChar() &&
                (!lhsType->IsChar() && lhsType->IsInt())) return lhsType;
            if (lhsType->IsInt()) return lhsType;
            if (rhsType->IsInt()) return rhsType;

            //if it made it to here then both sides are user-defined types
            return rhsType;
        }
};
