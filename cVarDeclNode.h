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

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol * identifier, cSymbol * type) : cDeclNode()
        {
            AddChild(new cBaseTypeNode(type->GetName(), 
                type->GetSize(), type->IsFloat()));
            AddChild(identifier);

            //TODO
            //Add symbol table insertion and error handling  stuff here
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
