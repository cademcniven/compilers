#pragma once
//**************************************
// cArrayDeclNode.h
//
// Defines an AST node for array declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cBaseTypeNode.h"
#include "cSymbolTable.h"
#include <vector>

using std::vector;

extern cSymbolTable g_symbolTable;

class cArrayDeclNode : public cDeclNode
{
    public:
        cArrayDeclNode(cSymbol * name, cSymbol * type, 
            cDeclsNode * ranges) : cDeclNode()
        {
            cSymbol * symbol = g_symbolTable.LookupSymbol(name->GetName());

            if (symbol != nullptr)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = name;
           
            symbol->SetTokenType(TYPE_ID);
            symbol->SetDecl(this);
            g_symbolTable.InsertSymbol(symbol);

            AddChild(symbol);
            AddChild(type->GetDecl());
            AddChild(ranges);

            m_size = 0;
        }

        virtual string NodeType() { return string("array"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string GetTypeName() { return dynamic_cast<cSymbol*>(GetChild(0))->GetName(); }
        virtual bool IsArray() { return true; }
        cDeclNode * GetElementType()
        {
            return dynamic_cast<cDeclNode*>(GetChild(1));
        }

        int NumRanges() 
        {
            return dynamic_cast<cDeclsNode*>(GetChild(2))->NumDecls();
        }

        virtual cDeclNode* GetType() { return this; }

        int GetRangeStart(int range)
        {
            return dynamic_cast<cRangeDeclNode*>
                (dynamic_cast<cDeclsNode*>(GetChild(2))->GetDecl(range))->GetRangeStart();
        }
        
        int GetRangeEnd(int range)
        {
            return dynamic_cast<cRangeDeclNode*>
                (dynamic_cast<cDeclsNode*>(GetChild(2))->GetDecl(range))->GetRangeEnd();
        }

        virtual int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }

        void SetRowSizes(vector<int> rowSizes) { m_rowSizes = rowSizes; }
        void SetStartIndexes(vector<int> startIndexes) { m_startIndexes = startIndexes; }

        virtual string AttributesToString()
        {
            string attributes = " size=\"";
            attributes += std::to_string(m_size);
            attributes += "\"";

            attributes += " rowsizes=\"";
            int numRanges = NumRanges();
            for (int i = 0; i < numRanges; ++i)
            {
                if (i != 0)
                    attributes += " ";

                attributes += std::to_string(m_rowSizes[i]);
            }
            attributes += "\"";

            attributes += " startindexes=\"";
            for (int i = 0; i < numRanges; ++i)
            {
                if (i != 0)
                    attributes += " ";

                attributes += std::to_string(m_startIndexes[i]);
            }
            attributes += "\"";

            return attributes;
        }

    private:
        int m_size;
        vector<int> m_rowSizes;
        vector<int> m_startIndexes;
};
