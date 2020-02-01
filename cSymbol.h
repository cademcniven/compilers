#pragma once
//*********************************************************
// cSymbol.h
//
// cSymbol class declaration.
// cSymbols are objects that represent identifiers and other such strings in 
// the compiler.
//
// Authors: Phil Howard, Cade McNiven
// phil.howard@oit.edu
// cade.mcniven@oit.edu
//

#include <string>
#include <utility>
#include "cAstNode.h"
#include "pascalparse.h"

using std::string;

class cSymbol : public cAstNode
{
    public:
        cSymbol(string name, int id) : cAstNode()
        {
            m_name = name;

            if (id == -1)
            {
                m_id = ++nextId;
                m_tokenType = IDENTIFIER;
            }
            else
            {
                m_id = id;
                m_tokenType = id;
            }
        }

        // return the name of the cSymbol
        string GetName() { return m_name; }

        int GetTokenType() { return m_tokenType; }

        virtual string AttributesToString()
        {
            string result(" id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";
            return result;
        }

        virtual string NodeType() { return string("sym"); }
        virtual void Visit(cVisitor * visitor) { visitor->Visit(this); }
    protected:
        static long long nextId;    // used to generate unique IDs
        long long m_id;             // ID for this cSymbol
        string m_name;              // name for this cSymbol
        int m_tokenType;
};
