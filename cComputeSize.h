#pragma once
#include "astnodes.h"
#include "cVisitor.h"
#include <vector>

using std::vector;

#define WORD_SIZE 4

class cComputeSize : public cVisitor
{
    public:
        cComputeSize() : cVisitor() { m_offset = 0; }
        virtual void VisitAllNodes(cAstNode * node) { node->VisitAllChildren(this); }

        //******************************************************************************************
        virtual void Visit(cFuncDeclNode * node)
        {
            node->VisitAllChildren(this);
        }

        //******************************************************************************************
        virtual void Visit(cVarDeclNode * node)
        {
            if (node->GetSize() != 1)
            {
                m_offset = AlignOffset(m_offset);
            }

            if (node->GetType()->IsArray())
                node->SetSize(node->GetType()->GetSize());

            node->SetOffset(m_offset);
            m_offset += node->GetSize();

            node->VisitAllChildren(this);
        }

        //******************************************************************************************
        virtual void Visit(cBlockNode * node)
        {
            int oldOffset = m_offset;
            node->VisitAllChildren(this);

            m_offset = AlignOffset(m_offset);
            node->SetSize(m_offset - oldOffset);
        }

        //******************************************************************************************
        virtual void Visit(cVarExprNode * node)
        {
            node->SetOffset(node->GetDecl()->GetOffset());
            node->SetSize(node->GetDecl()->GetSize());

            node->VisitAllChildren(this);
        }

        //******************************************************************************************
        virtual void Visit(cArrayDeclNode * node)
        {
            int numRanges = node->NumRanges();
            int size = 0;
            vector<int> rowSizes;
            vector<int> startIndexes;

            for (int i = 0; i < numRanges; ++i)
            {
                int rowSize = node->GetRangeEnd(i) - node->GetRangeStart(i) + 1;
                rowSizes.push_back(node->GetElementType()->GetSize());
                startIndexes.push_back(node->GetRangeStart(i));
                size += rowSizes[i] * rowSize;
            }   

            node->SetSize(size);
            node->SetRowSizes(rowSizes);
            node->SetStartIndexes(startIndexes);

            node->VisitAllChildren(this);
        }

    private:
        int m_offset;

        int AlignOffset(int offset)
        {
            int remainder = offset % WORD_SIZE;
            if (remainder == 0) return offset;

            return offset + WORD_SIZE - remainder;
        }
};
