#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines an AST node for variable declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cBaseTypeNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol * identifier, cSymbol * type) : cDeclNode()
        {
            if (g_symbolTable.LookupSymbolLocal(identifier->GetName()) != nullptr)
            {
                string error = "Symbol ";
                error += identifier->GetName();
                error += " already exists in current scope";

                SemanticParseError(error);
            }

            AddChild(type->GetDecl());
            
            cSymbol * symbol = g_symbolTable.LookupSymbol(identifier->GetName());
            
            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = identifier;

            symbol->SetDecl(this);
            g_symbolTable.InsertSymbol(symbol);

            AddChild(symbol);

            m_offset = 0;
            m_size = type->GetDecl()->GetSize();
        }

        virtual string AttributesToString()
        {
            string attributes = "";
            if (m_size != 0)
            {
                attributes += " size=\"";
                attributes += std::to_string(m_size);
                attributes += "\"";
            }

            attributes += " offset=\"";
            attributes += std::to_string(m_offset);
            attributes += "\"";

            return attributes;
        }
        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual bool IsVar() { return true; }
        virtual string GetTypeName() { return "var"; }
        cDeclNode * GetType()
        {
            cDeclNode * type = dynamic_cast<cDeclNode*>(GetChild(0));

            if (type->IsVar())
                return dynamic_cast<cVarDeclNode*>(type)->GetType();
            
            return type;
        }

        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }
        int GetOffset() { return m_offset; }
        void SetOffset(int offset) { m_offset = offset; }

        private:
            int m_offset;
            int m_size;
};
