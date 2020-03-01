#pragma once
//**************************************
// cBlockNode.h
//
// Defines AST node for a block of code (stuff inside {})
//
// Since blocks can take the place of statements, this class inherits from 
// cStmtNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cBlockNode : public cStmtNode
{
    public:
        // params are the decls and statements contained in the block
        cBlockNode(cDeclsNode *decls, cStmtsNode *statements)
            : cStmtNode()
        {
            AddChild(decls);
            AddChild(statements);
            m_size = 0;
            m_isFuncBlock = false;
        }

        virtual string AttributesToString()
        {
            if (m_size == 0)
                return "";

            return  " size=\"" + std::to_string(m_size) + "\"";
        }
        virtual string NodeType() { return string("block"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }
        bool IsFuncBlock() { return m_isFuncBlock; }
        void SetIsFuncBlock(bool isFuncBlock) { m_isFuncBlock = isFuncBlock; }
    private:
        int m_size;
        bool m_isFuncBlock;
};
