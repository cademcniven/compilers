#pragma once
//**************************************
// cAssignNode.h
//
// Defines AST node for ASSIGN statements
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cVarExprNode.h"
#include "cExprNode.h"

class cAssignNode : public cStmtNode
{
    public:
        cAssignNode(cVarExprNode* var, cExprNode* expr) : cStmtNode()
        {
            AddChild(var);
            AddChild(expr);
        }

        virtual string NodeType() { return string("assign"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        cVarExprNode * GetLhs()
        {
            return dynamic_cast<cVarExprNode*>(GetChild(0));
        }
        
        cExprNode * GetRhs()
        {
            return dynamic_cast<cExprNode*>(GetChild(1));
        }

};
