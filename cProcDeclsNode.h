#pragma once
//**************************************
// cProcDeclsNode.h
//
// Defines AST node for list of procedure declarations
//
// Author: Cade McNiven 
// cade.mcniven
//

#include "cDeclsNode.h"
#include "cProcDeclNode.h"

class cProcDeclsNode : public cDeclsNode
{
    public:
        cProcDeclsNode() : cDeclsNode() {}

        void AddDecl(cDeclNode *node) { AddChild(node); }
        void AddDecls(cDeclsNode *decls) { AddAllChildren(decls); }
        virtual string NodeType() { return string("proc_decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
