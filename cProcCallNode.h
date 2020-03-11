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

            m_paramSize = 0;
        }

        virtual string NodeType() { return string("procCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual cDeclNode * GetDecl()
        {
            if (GetChild(0) == nullptr)
                return nullptr;

            return dynamic_cast<cSymbol*>(GetChild(0))->GetDecl();
        }

        int NumArgs()
        {
            return dynamic_cast<cExprListNode*>(GetChild(1))->NumExprs();
        }

        cExprNode * GetArg(int index)
        {
            return dynamic_cast<cExprListNode*>(GetChild(1))->GetExpr(index);
        }

        string GetName()
        {
            return dynamic_cast<cSymbol*>(GetChild(0))->GetName();
        }

        int GetParamSize() { return m_paramSize; }
        void SetParamSize(int paramSize) { m_paramSize = paramSize; }

        virtual string AttributesToString()
        {
            string attributes = "";

            if (m_paramSize != 0)
            {
                attributes += " paramSize=\"";
                attributes += std::to_string(m_paramSize);
                attributes += "\"";
            }

            return attributes;
        }

    private:
        int m_paramSize;
};
