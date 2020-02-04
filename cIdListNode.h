#pragma once
//**************************************
// cIdListNode.h
//
// List of cSymbols that are all IDENTIFIERs
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include <list>
#include "cSymbol.h"

using std::list;

class cIdListNode
{
    public:
        // param is first actual param passed to function
        cIdListNode(cSymbol *param = nullptr)
        {
            if (param != nullptr) m_symbols.push_back(param);
        }

        void AddId(cSymbol *node) 
        {
            if (node != nullptr)
                m_symbols.push_back(node);
        }

        list<cSymbol*> GetIds() { return m_symbols; }
    protected:
        list<cSymbol*> m_symbols;
};
