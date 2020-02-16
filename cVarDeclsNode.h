#pragma once
//**************************************
// cVarDeclsNode.h
//
// Defines AST node for list of variable declarations
//
// Author: Cade McNiven 
// cade.mcniven
//

#include "cDeclsNode.h"
#include "cVarDeclNode.h"
#include "cIdListNode.h"

class cVarDeclsNode : public cDeclsNode
{
    public:
        cVarDeclsNode() : cDeclsNode() {}
        cVarDeclsNode(cIdListNode *param, cSymbol * type)
            : cDeclsNode()
        {
            for (int i = 0; i < param->NumIds(); ++i)
            {
                AddChild(new cVarDeclNode(param->GetId(i), type));
            }
        }

        void AddDecl(cVarDeclNode *node) { AddChild(node); }
        void AddDecls(cVarDeclsNode *decls) { AddAllChildren(decls); }
        int NumDecls() { return NumChildren(); }
        virtual string NodeType() { return string("var_decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
