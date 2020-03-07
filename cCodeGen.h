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
};
