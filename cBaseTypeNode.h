#pragma once
//**************************************
// cBaseTypeNode.h
//
// Defines an AST node for base types (int, real, etc)
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include <string>
#include "cAstNode.h"
#include "cDeclNode.h"

using std::string;

class cBaseTypeNode: public cDeclNode
{
    public:
        cBaseTypeNode(string typeName, int size, bool isFloat) : cDeclNode()
        {
            m_typeName = typeName;
            m_size = size;
            m_isFloat = isFloat;
        }

        virtual string AttributesToString() 
        {
            return " name=\"" + m_typeName + "\"" +
                   " size=\"" + std::to_string(m_size) + "\"" +
                   " isFloat=\"" + std::to_string(static_cast<int>(m_isFloat)) +
                   "\"";
        }
        virtual string NodeType() { return string("type"); }
        virtual bool IsReal() { return m_isFloat; }
        virtual bool IsInt() { return m_size == 4; }
        virtual bool IsChar() { return m_size == 1; }
        virtual bool IsType() { return true; }
        virtual int GetSize() { return m_size; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string GetTypeName() { return m_typeName; }
        virtual cDeclNode* GetType() { return this; }
    protected:
        string m_typeName;
        int m_size;
        bool m_isFloat;
};
