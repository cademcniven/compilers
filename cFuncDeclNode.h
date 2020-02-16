#pragma once
//**************************************
// cFuncDecl.h
//
// Defines an AST node for function declarations
//
// Author: Cade McNiven
// cade.mcniven@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cProcDeclNode.h"

extern cSymbolTable g_symbolTable;

class cFuncDeclNode : public cDeclNode
{
    public:
        cFuncDeclNode(cSymbol * header) : cDeclNode()
        {
            bool hasProto = false;
            m_isForward = false;
            m_prevDecl = nullptr;
            cSymbol * localSymbol = g_symbolTable.LookupSymbolLocal(header->GetName());

            //check if the symbol exists and the same scope, and see if it
            //was a forward declaration or not
            if (localSymbol != nullptr)
            {
                if (!localSymbol->GetDecl()->IsFunc())
                {
                    string error = "Symbol ";
                    error += header->GetName();
                    error += " already exists in current scope";
                    SemanticParseError(error);
                }
                else
                {
                    m_prevDecl = 
                        dynamic_cast<cFuncDeclNode*>(localSymbol->GetDecl());

                    if (m_prevDecl->GetIsForward())
                        hasProto = true;
                }
            }

            cSymbol * symbol = g_symbolTable.LookupSymbol(header->GetName());

            if (symbol != nullptr && !hasProto)
                symbol = new cSymbol(symbol->GetName(), -1);
            else
                symbol = header;
            
            symbol->SetDecl(this);
            g_symbolTable.InsertSymbol(symbol);

            AddChild(symbol);
            m_name = symbol->GetName();
        }

        void AddParams(cVarDeclsNode * params) 
        { 
            if (m_prevDecl != nullptr)
            {
                if (m_prevDecl->NumParams() != params->NumDecls())
                {
                    string error = m_name;
                    error += " redeclared with a different number of parameters";
                    SemanticParseError(error);
                }
                else
                    AddChild(params);
            }
            else
                AddChild(params); 
        }
        cVarDeclsNode * GetParams() { return dynamic_cast<cVarDeclsNode*>(GetChild(2)); }
        int NumParams() { return dynamic_cast<cVarDeclsNode*>(GetChild(2))->NumDecls(); }

        void AddBlock(cBlockNode * block) 
        { 
            if (m_prevDecl != nullptr)
            {
                if (m_prevDecl->GetBlock() != nullptr)
                {
                    string error = m_name;
                    error += " already has a definition";
                    SemanticParseError(error);
                }
                else
                    AddChild(block);
            }
            else
                AddChild(block); 
        }
        cBlockNode * GetBlock() { return dynamic_cast<cBlockNode*>(GetChild(3)); }

        void AddType(cSymbol * type) 
        {
            if (m_prevDecl != nullptr)
            {
                if (m_prevDecl->GetType() != type->GetDecl())
                {
                 string error = m_name;
                 error += " previously declared with different return type";
                 SemanticParseError(error);   
                }
                else
                    AddChild(type->GetDecl());
            }
            else
                AddChild(type->GetDecl()); 
        }
        cDeclNode * GetType() { return dynamic_cast<cDeclNode*>(GetChild(1)); }

        void SetIsForward(bool forward) { m_isForward = forward; }
        bool GetIsForward() { return m_isForward; }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual bool IsFunc() { return true; }

    private:
        bool m_isForward;
        string m_name;
        cFuncDeclNode * m_prevDecl;
};
