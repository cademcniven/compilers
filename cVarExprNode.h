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
#include "cExprListNode.h"

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol* value) : cExprNode()
        {
            m_value = value;
            AddChild(value);
        }

        void AddExprList(cExprListNode * exprList) { AddChild(exprList); }

        cExprListNode * GetExprList()
        {
            return dynamic_cast<cExprListNode*>(GetChild(1));
        }

        cExprNode * GetExpr(int index)
        {
            return GetExprList()->GetExpr(index);
        }

        int NumExprs() { return GetExprList()->NumExprs(); }

        // sort of unsure if this was the correct solution to the problem.
        // just doing AddChild would be in line with other similar objects,
        // but it caused the ids to be wrong. Adding code like what's in 
        // the decl nodes fixed the problem but may be incorrect.
        void AddSymbol(cSymbol * identifier) 
        { 
            cSymbol * symbol = g_symbolTable.LookupSymbol(identifier->GetName());

            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = identifier;

            g_symbolTable.InsertSymbol(symbol);
            AddChild(symbol); 

            SetSize(symbol->GetDecl()->GetSize());
            SetOffset(symbol->GetDecl()->GetOffset());
        }

        virtual string AttributesToString()
        {
            string attributes = "";
            if (m_size != 0)
            {
                attributes += " size=\"";
                attributes += std::to_string(m_size);
                attributes += "\"";
            }

            attributes += " offset=\"";
            attributes += std::to_string(m_offset);
            attributes += "\"";

            return attributes;
        }
        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode * GetType() 
        { 
            if (m_value->GetDecl() == nullptr)
                return nullptr;

            return m_value->GetDecl()->GetType(); 
        }
        
        cDeclNode * GetDecl() { return m_value->GetDecl(); }

        string GetName()
        {
            return dynamic_cast<cSymbol*>(GetChild(0))->GetName();
        }

        int GetSize() { return m_size; }
        int GetOffset() { return m_offset; }
        void SetSize(int size) { m_size = size; }
        void SetOffset(int offset) { m_offset = offset; }
    protected:
        cSymbol* m_value;
        int m_size;
        int m_offset;
};
