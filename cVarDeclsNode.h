#pragma once
//**************************************
// cVarDeclsNode.h
//
// Defines AST node for list of variable declarations
//
// Author: Cade McNiven 
// cade.mcniven
//

#include <list>
#include "cDeclsNode.h"
#include "cVarDeclNode.h"
#include "cIdListNode.h"

using std::list;

class cVarDeclsNode : public cDeclsNode
{
    public:
        cVarDeclsNode() : cDeclsNode() {}
        cVarDeclsNode(cIdListNode *param, cSymbol * type)
            : cDeclsNode()
        {
            auto nodes = param->GetIds();
            for (auto& i : nodes)
                AddChild(new cVarDeclNode(i, type));
        }

        void AddDecl(cVarDeclNode *node) { AddChild(node); }
        void AddDecls(cVarDeclsNode *decls) { AddAllChildren(decls); }
        virtual string NodeType() { return string("var_decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
