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

using std::string;

class cBaseTypeNode: public cAstNode
{
    public:
        cBaseTypeNode(string typeName, int size, bool isFloat) : cAstNode()
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
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        string m_typeName;
        int m_size;
        bool m_isFloat;
};
