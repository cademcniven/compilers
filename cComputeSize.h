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
            node->SetOffset(m_offset);
            m_offset += node->GetSize();
            m_offset += m_offset % WORD_SIZE;

            node->VisitAllChildren(this);
        }

        virtual void Visit(cBlockNode * node)
        {
            node->VisitAllChildren(this);

            m_offset += m_offset % WORD_SIZE;
            node->SetSize(m_offset);
        }

    private:
        int m_offset;
};
