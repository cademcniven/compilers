#pragma once
#include "astnodes.h"
#include "cVisitor.h"
#include "emit.h"

class cCodeGen : public cVisitor
{
    public:
        cCodeGen(std::string filename) : cVisitor() 
        {
            InitOutput(filename);
        }

        ~cCodeGen()
        {
            FinalizeOutput();
        }

        virtual void VisitAllNodes(cAstNode * node) { node->Visit(this); }

        virtual void Visit(cProgramNode * node)
        {
            EmitString("main:\n");
            EmitString("ADJSP ");
            EmitInt(node->GetBlock()->GetSize());
            EmitString("\n");

            node->VisitAllChildren(this);

            EmitString("PUSH 0\n");
            EmitString("RETURNV");
        }

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

        virtual void Visit(cIntExprNode * node)
        {
            EmitString("PUSH ");
            EmitInt(node->GetValue());
            EmitString("\n");
        }

        virtual void Visit(cAssignNode * node)
        {
            node->GetRhs()->Visit(this);

            int offset = node->GetLhs()->GetOffset();
            int size = node->GetLhs()->GetSize();

            if (size == 1)
                EmitString("POPCVAR ");
            else
                EmitString("POPVAR ");

            EmitInt(offset);
            EmitString("\n");
        }

        virtual void Visit(cVarExprNode * node)
        {
            int offset = node->GetOffset();
            int size = node->GetSize();

            if (size == 1)
                EmitString("PUSHCVAR ");
            else
                EmitString("PUSHVAR ");

            EmitInt(offset);
            EmitString("\n");
        }

        virtual void Visit(cBinaryExprNode * node)
        {
            node->GetLhs()->Visit(this);
            node->GetRhs()->Visit(this);
            node->GetOp()->Visit(this);
        }

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
            }
        }
};
