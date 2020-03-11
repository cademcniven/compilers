#pragma once
#include "astnodes.h"
#include "cVisitor.h"
#include "emit.h"

class cCodeGen : public cVisitor
{
    public:
        //*******************************************************************************
        cCodeGen(std::string filename) : cVisitor() 
        {
            InitOutput(filename);
        }

        //*******************************************************************************
        ~cCodeGen()
        {
            FinalizeOutput();
        }

        //*******************************************************************************
        virtual void VisitAllNodes(cAstNode * node) { node->Visit(this); }

        //*******************************************************************************
        virtual void Visit(cProgramNode * node)
        {
            if (node->GetBlock()->GetDecls() != nullptr)
                node->GetBlock()->GetDecls()->Visit(this);

            EmitString("main:\n");
            EmitString("ADJSP ");
            EmitInt(node->GetBlock()->GetSize());
            EmitString("\n");

            node->GetBlock()->GetStmts()->Visit(this);

            EmitString("PUSH 0\n");
            EmitString("RETURNV");
        }

        //*******************************************************************************
        virtual void Visit(cWriteNode * node)
        {
            for (int i = 0; i < node->NumExprs(); ++i)
            {
                node->GetExpr(i)->Visit(this);
                EmitString("CALL @print\n");
                EmitString("POP\n");
                EmitString("POP\n");
            }
        }

        //*******************************************************************************
        virtual void Visit(cIntExprNode * node)
        {
            EmitString("PUSH ");
            EmitInt(node->GetValue());
            EmitString("\n");
        }

        //*******************************************************************************
        virtual void Visit(cAssignNode * node)
        {
            node->GetRhs()->Visit(this);

            int offset = node->GetLhs()->GetOffset();
            int size = node->GetLhs()->GetSize();

            if (node->GetLhs()->GetType()->IsArray())
            {
                cArrayDeclNode * arr =
                    dynamic_cast<cArrayDeclNode*>(node->GetLhs()->GetType());
                EmitString("PUSHFP\n");
                EmitString("PUSH ");
                EmitInt(node->GetLhs()->GetOffset());
                EmitString("\nPLUS\n");
                node->GetLhs()->GetExpr(0)->Visit(this);
                EmitString("\nPUSH ");
                EmitInt(arr->GetElementType()->GetSize());
                EmitString("\nTIMES\n");
                EmitString("PLUS\n");
                if (arr->GetElementType()->GetSize() == 1)
                    EmitString("POPCVARIND\n");
                else

                EmitString("POPVARIND\n");
            }
            else
            {
                if (size == 1)
                    EmitString("POPCVAR ");
                else
                    EmitString("POPVAR ");

                EmitInt(offset);
                EmitString("\n");
            }
        }

        //*******************************************************************************
        virtual void Visit(cVarExprNode * node)
        {
            int offset = node->GetOffset();
            int size = node->GetSize();

            if (node->GetType()->IsArray())
            {
                cArrayDeclNode * arr =
                    dynamic_cast<cArrayDeclNode*>(node->GetType());
                EmitString("PUSHFP\n");
                EmitString("PUSH ");
                EmitInt(node->GetOffset());
                EmitString("\nPLUS\n");
                node->GetExpr(0)->Visit(this);
                EmitString("\nPUSH ");
                EmitInt(arr->GetElementType()->GetSize());
                EmitString("\nTIMES\n");
                EmitString("PLUS\n");
                if (arr->GetElementType()->GetSize() == 1)
                    EmitString("PUSHCVARIND\n");
                else
                    EmitString("PUSHVARIND\n");
            }
            else
            {
                if (size == 1)
                    EmitString("PUSHCVAR ");
                else
                    EmitString("PUSHVAR ");

                EmitInt(offset);
                EmitString("\n");
            }
        }

        //*******************************************************************************
        virtual void Visit(cBinaryExprNode * node)
        {
            node->GetLhs()->Visit(this);
            node->GetRhs()->Visit(this);
            node->GetOp()->Visit(this);
        }

        //*******************************************************************************
        virtual void Visit(cUnaryExprNode * node)
        {
            node->GetExpr()->Visit(this);
            
            //we can't visit the op node because
            //- means something different in this context
            int op = node->GetOp()->GetOp();

            if (op == '-')
                EmitString("NEG\n");
            else if (op == NOT)
                EmitString("NOT\n");
        }

        //*******************************************************************************
        virtual void Visit(cOpNode * node)
        {
            int op = node->GetOp();

            switch (op)
            {
                case OR:
                    EmitString("OR\n");
                    break;
                case AND:
                    EmitString("AND\n");
                    break;
                case MOD:
                    EmitString("MOD\n");
                    break;
                case DIV:
                    EmitString("DIVIDE\n");
                    break;
                case NOT:
                    EmitString("MINUS\n");
                    break;
                case LE:
                    EmitString("LE\n");
                    break;
                case GE:
                    EmitString("GE\n");
                    break;
                case NOT_EQUAL:
                    EmitString("NE\n");
                    break;
                case '<':
                    EmitString("LT\n");
                    break;
                case '>':
                    EmitString("GT\n");
                    break;
                case '/':
                    EmitString("DIVIDE\n");
                    break;
                case '*':
                    EmitString("TIMES\n");
                    break;
                case '+':
                    EmitString("PLUS\n");
                    break;
                case '-':
                    EmitString("MINUS\n");
                    break;
                case '=':
                    EmitString("EQ\n");
                    break;
            }
        }

        //*******************************************************************************
        virtual void Visit(cIfNode * node)
        {
            node->GetCondition()->Visit(this);

            //label 1 for jumping to else statement
            string label1 = GenerateLabel();
            //label 2 for jumping past else statement
            string label2 = GenerateLabel();

            EmitString("JUMPE @");
            EmitString(label1);
            EmitString("\n");

            node->GetIfStmt()->Visit(this);

            EmitString("JUMP @");
            EmitString(label2);
            EmitString("\n");

            EmitString(label1);
            EmitString(":\n");

            if (node->GetElseStmt() != nullptr)
                node->GetElseStmt()->Visit(this);

            EmitString(label2);
            EmitString(":\n");
        }

        //*******************************************************************************
        virtual void Visit(cFuncDeclNode * node)
        {
            if (node->GetBlock() == nullptr)
                return;

            EmitString(".function ");
            EmitString(node->GetName());
            EmitString("\n");

            EmitString(node->GetName());
            EmitString(":\n");
            EmitString("ADJSP ");
            EmitInt(node->GetBlock()->GetSize() + node->GetType()->GetSize());
            EmitString("\n");

            node->GetBlock()->Visit(this);

            EmitString("PUSHVAR 0\n");
            EmitString("RETURNV\n");
           
        }

        //*******************************************************************************
        virtual void Visit(cFuncExprNode * node)
        {
            int numArgs = node->NumArgs();
            if (numArgs != 0)
            {
                for (int i = numArgs - 1; i >= 0; --i)
                    node->GetArg(i)->Visit(this);
            }

            EmitString("CALL @");
            EmitString(node->GetName());
            EmitString("\n");

            EmitString("POPARGS ");
            EmitInt(node->GetParamSize());
            EmitString("\n");
        }
        
        //*******************************************************************************
        virtual void Visit(cWhileNode * node)
        {
            string label1 = GenerateLabel();
            string label2 = GenerateLabel();

            EmitString(label1);
            EmitString(":\n");

            node->GetCondition()->Visit(this);
            
            //if the condition is false, jump past the while loop code
            EmitString("JUMPE @");
            EmitString(label2);
            EmitString("\n");

            //otherwise do the loop stuff
            node->GetStmt()->Visit(this);

            EmitString("JUMP @");
            EmitString(label1);
            EmitString("\n");

            EmitString(label2);
            EmitString(":\n");
        }

        //*******************************************************************************
        virtual void Visit(cProcDeclNode * node)
        {
            if (node->GetBlock() == nullptr)
                return;

            EmitString(".function ");
            EmitString(node->GetName());
            EmitString("\n");

            EmitString(node->GetName());
            EmitString(":\n");
            EmitString("ADJSP ");
            EmitInt(node->GetBlock()->GetSize());
            EmitString("\n");

            node->GetBlock()->Visit(this);

            EmitString("RETURN\n");
        }

        //*******************************************************************************
        virtual void Visit(cProcCallNode * node)
        {
            int numArgs = node->NumArgs();
            if (numArgs != 0)
            {
                for (int i = numArgs - 1; i >= 0; --i)
                    node->GetArg(i)->Visit(this);
            }

            EmitString("CALL @");
            EmitString(node->GetName());
            EmitString("\n");


            for (int i = 0; i < numArgs; ++i)
            {
                EmitString("POPVAR ");
                EmitInt(node->GetArg(i)->GetType()->GetOffset());
                EmitString("\n");
            }
        }
};
