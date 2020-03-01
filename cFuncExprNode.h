#pragma once
//**************************************
// cFuncExprNode.h
//
// Defines AST node for function calls
//
// Inherits from cExprNode so functions can be used as parts of expressions
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"
#include "cExprListNode.h"

extern cSymbolTable g_symbolTable;

class cFuncExprNode : public cExprNode
{
    public:
        cFuncExprNode(cSymbol * name, cExprListNode * args) : cExprNode()
        {
            AddChild(name);
            AddChild(args);

            m_paramSize = 0;
        }

        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode * GetType() 
        { 
            if (GetChild(0) == nullptr)
                return nullptr;

            return dynamic_cast<cSymbol*>(GetChild(0))->GetDecl()->GetType(); 
        }

        virtual cDeclNode * GetDecl() 
        { 
            if (GetChild(0) == nullptr)
                return nullptr;

            return dynamic_cast<cSymbol*>(GetChild(0))->GetDecl(); 
        }

        string GetName()
        {
            return dynamic_cast<cSymbol*>(GetChild(0))->GetName();
        }

        int NumArgs()
        {
            cExprListNode * args = dynamic_cast<cExprListNode*>(GetChild(1));
            if (args == nullptr) return 0;
            else return args->NumExprs();
        }

        cExprNode * GetArg(int index) 
        { 
            return dynamic_cast<cExprListNode*>(GetChild(1))->GetExpr(index);
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
