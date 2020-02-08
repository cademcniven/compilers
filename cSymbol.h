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
        //TODO: fix this garbage
        cSymbol(string name, int id) : cAstNode()
        {
            m_decl = nullptr;
            m_name = name;

            if (id == -1)
            {
                m_id = ++nextId;
                m_tokenType = IDENTIFIER;
                m_size = 0;
                m_isFloat = false;
            }
            else
            {
                m_id = TYPE_ID;
                m_tokenType = id;
                
                //if we're inserting the keyword "char"
                if (id == 1)
                {
                    m_size = 1;
                    m_isFloat = false;
                    m_tokenType = TYPE_ID;
                }
                //if we're inserting the keyword "integer"
                else if (id == 2)
                {
                    m_size = 4;
                    m_isFloat = false;
                    m_tokenType = TYPE_ID;
                }
                //if we're inserting the keyword "real"
                else if (id == 3)
                {
                    m_size = 8;
                    m_isFloat = true;
                    m_tokenType = TYPE_ID;
                }
            }
        }

        // return the name of the cSymbol
        string GetName() { return m_name; }

        int GetTokenType() { return m_tokenType; }

        void SetTokenType(int type) { m_tokenType = type; }

        int GetSize() { return m_size; }

        bool IsFloat() { return m_isFloat; }

        cDeclNode * GetDecl() { return m_decl; }

        void SetDecl(cDeclNode * decl) { m_decl = decl; }

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
        int m_tokenType;
        int m_size;
        bool m_isFloat;
        cDeclNode* m_decl;
};
