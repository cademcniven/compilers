#pragma once
//**************************************
// cProcCallNode.h
//
// Defines AST node for procCall statements
//
// Inherits from cStmtNode so procCall statments can go in statement lists, and be
// used anywhere a statement is legal.
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cSymbol.h"
#include "cExprListNode.h"

class cProcCallNode : public cStmtNode
{
    public:
        cProcCallNode(cSymbol * name, cExprListNode * args) : cStmtNode()
        {
            AddChild(name);
            AddChild(args);
        }

        virtual string NodeType() { return string("procCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
