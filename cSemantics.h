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
            else
            {
                //check the types of each argument
                int numArgs = node->NumArgs();
                for (int i = 0; i < numArgs; ++i)
                {
                    cDeclNode * argType = node->GetArg(i)->GetType();
                    cDeclNode * paramType = 
                        dynamic_cast<cFuncDeclNode*>(node->GetType())->GetParam(i)->GetType();
                    if (argType->IsVar())
                        argType = dynamic_cast<cVarDeclNode*>(argType)->GetType();
                    if (argType->IsFunc())
                        argType = dynamic_cast<cFuncDeclNode*>(argType)->GetType();
                    if (paramType->IsVar())
                        paramType = dynamic_cast<cVarDeclNode*>(paramType)->GetType();
                    if (paramType->IsFunc())
                        paramType = dynamic_cast<cFuncDeclNode*>(paramType)->GetType();

                    if (argType->GetTypeName() != paramType->GetTypeName())
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
                if (!dynamic_cast<cVarDeclNode*>(node->GetType())->GetType()->IsArray())
                {
                    string error = node->GetName();
                    error += " is not an array"; 
                    node->SemanticError(error);
                }
                //using the wrong number of indexes
                else if (dynamic_cast<cArrayDeclNode*>
                        (dynamic_cast<cVarDeclNode*>(node->GetType())->GetType())->NumRanges() 
                        != node->NumExprs())
                {
                    string error = node->GetName();
                    error += " does not have the correct number of indexes";
                    node->SemanticError(error);
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

            //we want to find the actual type of both sides, not just a vardecl or funcdecl
            if (lhsType->IsVar())
                lhsType = dynamic_cast<cVarDeclNode*>(lhsType)->GetType();
            if (rhsType->IsVar())
                rhsType = dynamic_cast<cVarDeclNode*>(rhsType)->GetType();
            if (lhsType->IsFunc())
                lhsType = dynamic_cast<cFuncDeclNode*>(lhsType)->GetType();
            if (rhsType->IsFunc())
                rhsType = dynamic_cast<cFuncDeclNode*>(rhsType)->GetType();
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
            else if (lhsType->IsInt())
            {
                if (!(rhsType->IsInt() || rhsType->IsChar()))
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
                //if the rhs is an int, check the value of it to see if it can be
                //interpreted as a char
                bool intIsChar = false;
                if (rhsType->IsInt())
                {
                    cIntExprNode * intval = dynamic_cast<cIntExprNode*>(node->GetRhs());

                    if (intval != nullptr)
                    {
                        int value = 
                            dynamic_cast<cIntExprNode*>(node->GetRhs())->GetValue();
                        if (value <= 127 && value >= -128)
                            intIsChar = true;
                    }
                }

                if (!rhsType->IsChar() && !intIsChar)
                {
                    string error = "Cannot assign ";
                    error += rhsType->GetTypeName();
                    error += " to ";
                    error += lhsType->GetTypeName();

                    node->SemanticError(error);
                }
            }
            else if (lhsType->IsFunc())
            {

            }
            //this happens for user defined types
            else
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
