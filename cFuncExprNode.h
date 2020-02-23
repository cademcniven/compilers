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

class cFuncExprNode : public cExprNode
{
    public:
        cFuncExprNode(cSymbol * name, cExprListNode * args) : cExprNode()
        {
            AddChild(name);
            AddChild(args);
        }

        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode * GetType() 
        { 
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
};
