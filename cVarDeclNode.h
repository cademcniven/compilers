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
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual bool IsVar() { return true; }
        virtual string GetTypeName() { return "var"; }
        cDeclNode * GetType()
        {
            return dynamic_cast<cDeclNode*>(GetChild(0));
        }
};
