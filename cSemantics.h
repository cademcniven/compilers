#pragma once
#include "astnodes.h"
#include "cVisitor.h"

class cSemantics : public cVisitor
{
    public:
        cSemantics() : cVisitor() {}
        virtual void VisitAllNodes(cAstNode * node) { node->VisitAllChildren(this); }

        virtual void Visit(cFuncExprNode * node)
        {
            cDeclNode * decl = node->GetType();
            if (decl == nullptr)
            {
                string error = node->GetName();
                error += " is not declared";
                node->SemanticError(error);
            }
            else if (!decl->IsFunc())
            {
                string error = node->GetName();
                error += " is not a function";
                node->SemanticError(error);
            }
            else if (dynamic_cast<cFuncDeclNode*>(decl)->GetBlock() == nullptr)
            {
                string error = node->GetName();
                error += " is not fully defined";
                node->SemanticError(error);
            }
            else if (node->NumArgs() !=
                     dynamic_cast<cFuncDeclNode*>(decl)->NumParams())
            {
                string error = node->GetName();
                error += " called with wrong number of arguments";
                node->SemanticError(error);
            }

            node->VisitAllChildren(this);
        }

        virtual void Visit(cVarExprNode * node)
        {
            if (node->GetType() == nullptr)
            {
                string error = "Variable ";
                error += node->GetName();
                error += " is not defined"; 
                node->SemanticError(error);
            }

            node->VisitAllChildren(this);
        }
};
