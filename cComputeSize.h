#pragma once
#include "astnodes.h"
#include "cVisitor.h"

#define WORD_SIZE 4

class cComputeSize : public cVisitor
{
    public:
        cComputeSize() : cVisitor() { m_offset = 0; }
        virtual void VisitAllNodes(cAstNode * node) { node->VisitAllChildren(this); }

        virtual void Visit(cFuncDeclNode * node)
        {
            node->VisitAllChildren(this);
        }

        virtual void Visit(cVarDeclNode * node)
        {
            if (node->GetSize() != 1)
            {
                m_offset += m_offset % WORD_SIZE;
                m_offset += m_offset % WORD_SIZE;
            }

            node->SetOffset(m_offset);
            m_offset += node->GetSize();

            node->VisitAllChildren(this);
        }

        virtual void Visit(cBlockNode * node)
        {
            int oldOffset = m_offset;
            node->VisitAllChildren(this);

            m_offset += m_offset % WORD_SIZE;
            m_offset += m_offset % WORD_SIZE;
            node->SetSize(m_offset - oldOffset);
        }

        virtual void Visit(cVarExprNode * node)
        {
            node->SetOffset(node->GetDecl()->GetOffset());
            node->SetSize(node->GetDecl()->GetSize());

            node->VisitAllChildren(this);
        }

    private:
        int m_offset;
};
