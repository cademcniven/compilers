#pragma once
//**************************************
// cCompoundStmtNode.h
//
// Defines AST node for compound statements
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cCompoundStmtNode : public cStmtNode
{
    public:
        cCompoundStmtNode(cStmtsNode * stmts) : cStmtNode()
        {
            AddAllChildren(stmts);
        }

        virtual string NodeType() { return string("compound"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
