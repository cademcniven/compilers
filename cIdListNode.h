#pragma once
//**************************************
// cIdListNode.h
//
// List of cSymbols that are all IDENTIFIERs
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"

class cIdListNode : cAstNode
{
    public:
        // param is first actual param passed to function
        cIdListNode(cSymbol *param = nullptr) : cAstNode()
        {
            if (param != nullptr) AddChild(param);
        }

        void AddId(cSymbol *node) 
        {
            if (node != nullptr)
                AddChild(node);
        }
       
        int NumIds() { return NumChildren(); }

        cSymbol * GetId(int index) { return dynamic_cast<cSymbol*>(GetChild(index)); }
        virtual string NodeType() { return "IdList"; }
        virtual void Visit(cVisitor * visitor) { }
};
