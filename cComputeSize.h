#pragma once
//************************************************
// cComputSize.h
//
// Computes the size of types and functions
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

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
        virtual void Visit(cVarDeclNode * node)
        {
            if (node->GetSize() != 1)
                m_offset = AlignOffset(m_offset);

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

            if (node->IsFuncBlock())
                node->SetSize(m_offset);
            else
                node->SetSize(m_offset - oldOffset);
        }

        //******************************************************************************************
        virtual void Visit(cVarExprNode * node)
        {
            int size = node->GetDecl()->GetSize();

            if (node->GetType()->IsArray())
                size = dynamic_cast<cArrayDeclNode*>(node->GetType())->GetElementType()->GetSize();

            node->SetOffset(node->GetDecl()->GetOffset());
            node->SetSize(size);

            node->VisitAllChildren(this);
        }

        //******************************************************************************************
        virtual void Visit(cArrayDeclNode * node)
        {
            int numRanges = node->NumRanges();
            vector<int> rowSizes;
            vector<int> startIndexes;

            int prevRowLength = 1;
            for (int i = 0; i < numRanges; ++i)
            {
                rowSizes.push_back(node->GetElementType()->GetSize() * prevRowLength);
                startIndexes.push_back(node->GetRangeStart(i));

                prevRowLength *= (node->GetRangeEnd(i) - node->GetRangeStart(i) + 1) * 
                    node->GetElementType()->GetSize();
            }   

            node->SetSize(prevRowLength);
            node->SetRowSizes(rowSizes);
            node->SetStartIndexes(startIndexes);

            node->VisitAllChildren(this);
        }

        
        //******************************************************************************************
        virtual void Visit(cFuncDeclNode * node)
        {
            int oldOffset = m_offset;
            m_offset = 0;

            node->SetOffset(m_offset);
            node->SetSize(node->GetType()->GetSize());
            m_offset += node->GetType()->GetSize();

            node->VisitAllChildren(this);
            m_offset = oldOffset;
        }

        //******************************************************************************************
        virtual void Visit(cVarDeclsNode * node)
        {
            //if it's just a regular vardeclsnode then just visit normally
            if (!node->IsArgs())
            {
                node->VisitAllChildren(this);
                return;
            }

            //if it's arguments in a func or proc, do some funky stuff
            int oldOffset = m_offset;
            m_offset = -12;

            int numArgs = node->NumDecls();
            for (int i = 0; i < numArgs; ++i)
            {
                cVarDeclNode * arg = node->GetDecl(i);
                arg->SetSize(arg->GetType()->GetSize());
                arg->SetOffset(m_offset);

                m_offset -= arg->GetSize();
                m_offset = RoundDown(m_offset);
            }

            node->SetSize(-12 - m_offset);
            m_offset = oldOffset;
        }

        //******************************************************************************************
        virtual void Visit(cFuncExprNode * node)
        {
            cVarDeclsNode* params =
                dynamic_cast<cFuncDeclNode*>(node->GetDecl())->GetParams();

            if (params == nullptr)
                node->SetParamSize(0);
            else
                node->SetParamSize(params->GetSize());

            node->VisitAllChildren(this);
        }

        //******************************************************************************************
        virtual void Visit(cProcDeclNode * node)
        {
            int oldOffset = m_offset;
            m_offset = 0;
            node->VisitAllChildren(this);
            m_offset = oldOffset;
        }

        //******************************************************************************************
        virtual void Visit(cProcCallNode * node)
        {
            cVarDeclsNode* params =
                dynamic_cast<cProcDeclNode*>(node->GetDecl())->GetParams();

            if (params == nullptr)
                node->SetParamSize(0);
            else
                node->SetParamSize(params->GetSize());

            node->VisitAllChildren(this);
        }

    private:
        int m_offset;

        //the only efficient line in this codebase
        int AlignOffset(int offset) { return (offset + 3) & ~0x03; }

        //there's probably a more optimal way of doing this one but 
        //I couldn't figure it out
        int RoundDown(int offset) { return -((-offset + 3) & ~0x03); }
};
