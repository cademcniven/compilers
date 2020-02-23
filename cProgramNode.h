#pragma once
//**************************************
// cProgramNode.h
//
// Defines AST node for a lang program
//
// Authors: Phil Howard, Cade McNiven
// phil.howard@oit.edu
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"
#include "cBlockNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cProgramNode : public cAstNode
{
    public:
        // param is the block that makes up the program
        cProgramNode(cSymbol *name, cBlockNode *block) : cAstNode()
        {
            AddChild(name);
            AddChild(block);
        }

        virtual string NodeType() { return string("program"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
