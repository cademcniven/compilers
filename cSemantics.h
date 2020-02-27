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
            cDeclNode * decl = node->GetDecl();
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
            else
            {
                //check the types of each argument
                int numArgs = node->NumArgs();
                for (int i = 0; i < numArgs; ++i)
                {
                    cDeclNode * argType = node->GetArg(i)->GetType();
                    cDeclNode * paramType = 
                        dynamic_cast<cFuncDeclNode*>(node->GetDecl())->GetParam(i)->GetType();

                    if (!(argType->IsChar() && paramType->IsInt()) && 
                        argType->GetTypeName() != paramType->GetTypeName())
                    {
                        string error = "Cannot assign ";
                        error += argType->GetTypeName();
                        error += " to ";
                        error += paramType->GetTypeName();

                        node->SemanticError(error);
                    }
                }
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
            //array stuff
            else if (node->GetExprList() != nullptr)
            {
                //trying to dereference something that's not an array
                if (!node->GetType()->IsArray())
                {
                    string error = node->GetName();
                    error += " is not an array"; 
                    node->SemanticError(error);
                }
                //using the wrong number of indexes
                else if (dynamic_cast<cArrayDeclNode*>(node->GetType())->NumRanges() 
                        != node->NumExprs())
                {
                    string error = node->GetName();
                    error += " does not have the correct number of indexes";
                    node->SemanticError(error);
                }
                else
                {
                    int numIndexes = node->NumExprs();
                    for (int i = 0; i < numIndexes; ++i)
                    {
                        cDeclNode * type = node->GetExpr(i)->GetType();

                        if (!type->IsInt())
                        {
                            string error = "Index of ";
                            error += node->GetName();
                            error += " is not an integer";
                            node->SemanticError(error);
                        }
                    }
                }
            }
            node->VisitAllChildren(this);
        }

        virtual void Visit(cAssignNode * node)
        {
            //visit the children first to make sure that both sides of
            //the assignment have valid declarations
            node->VisitAllChildren(this);
            if (node->HasSemanticError())
                return;

            cDeclNode * lhsType = node->GetLhs()->GetType();
            cDeclNode * rhsType = node->GetRhs()->GetType();

            if (lhsType->IsArray() 
                && dynamic_cast<cVarExprNode*>(node->GetLhs())->GetExprList() != nullptr)
                lhsType = dynamic_cast<cArrayDeclNode*>(lhsType)->GetElementType();
            if (rhsType->IsArray()
                && dynamic_cast<cVarExprNode*>(node->GetRhs())->GetExprList() != nullptr)
                rhsType = dynamic_cast<cArrayDeclNode*>(rhsType)->GetElementType();

            if (lhsType->IsReal())
            {
                if (!(rhsType->IsReal() || rhsType->IsInt() || rhsType->IsChar()))
                {
                    string error = "Cannot assign ";
                    error += rhsType->GetTypeName();
                    error += " to ";
                    error += lhsType->GetTypeName();

                    node->SemanticError(error);
                }
            }
            else if (lhsType->IsChar())
            {
                if (!rhsType->IsChar())
                {
                    string error = "Cannot assign ";
                    error += rhsType->GetTypeName();
                    error += " to ";
                    error += lhsType->GetTypeName();

                    node->SemanticError(error);
                }
            }
            else if (lhsType->IsInt())
            {
                if (!rhsType->IsInt() && !rhsType->IsChar())
                {
                    string error = "Cannot assign ";
                    error += rhsType->GetTypeName();
                    error += " to ";
                    error += lhsType->GetTypeName();

                    node->SemanticError(error);
                }
            }
                        //this happens for user defined types
            else if (!lhsType->IsFunc())
            {
                if (lhsType->GetTypeName() != rhsType->GetTypeName())
                {
                    string error = "Cannot assign ";
                    error += rhsType->GetTypeName();
                    error += " to ";
                    error += lhsType->GetTypeName();

                    node->SemanticError(error);
                }
            }
        }
};
