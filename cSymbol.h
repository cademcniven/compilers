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
#include "cDeclNode.h"

using std::string;

class cSymbol : public cAstNode
{
    public:
        cSymbol(string name, int id) : cAstNode()
        {
            m_decl = nullptr;
            m_name = name;
            m_isType = false;

            if (id == -1)
            {
                m_id = ++nextId;
                m_tokenType = IDENTIFIER;
            }
            else
            {
                m_id = TYPE_ID;
                m_tokenType = id;
            }
        }

        // return the name of the cSymbol
        string GetName() { return m_name; }

        int GetTokenType() { return m_tokenType; }
        void SetTokenType(int type) { m_tokenType = type; }
        bool IsType() { return m_isType; }

        cDeclNode * GetDecl() { return m_decl; }
        void SetDecl(cDeclNode * decl) 
        { 
            m_decl = decl; 
            m_isType = decl->IsType();
            if (m_isType) m_tokenType = TYPE_ID;
        }

        virtual string AttributesToString()
        {
            string result(" id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";
            return result;
        }

        virtual string NodeType() { return string("sym"); }
        virtual void Visit(cVisitor * visitor) 
        { 
            visitor->Visit(this); 
        }
    protected:
        static long long nextId;    // used to generate unique IDs
        long long m_id;             // ID for this cSymbol
        string m_name;              // name for this cSymbol
        bool m_isType;
        int m_tokenType;
        cDeclNode* m_decl;
};
