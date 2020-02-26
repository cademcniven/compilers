#pragma once
//**************************************
// cRangeDeclNode.h
//
// Defines an AST node for range declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cRangeDeclNode : public cDeclNode
{
    public:
        cRangeDeclNode(int start, int end) : cDeclNode()
        {
            m_start = start;
            m_end = end;
        }

        virtual string AttributesToString()
        {
            return " start=\"" + std::to_string(m_start) + "\"" +
                   " end=\"" + std::to_string(m_end) + "\"";
        }

        virtual string NodeType() { return string("range"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string GetTypeName() { return "range"; }
        virtual cDeclNode* GetType() 
        { 
            return g_symbolTable.LookupSymbol("integer")->GetDecl();
        }
    private:
        int m_start;
        int m_end;
};
