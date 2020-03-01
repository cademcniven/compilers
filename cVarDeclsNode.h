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
            m_isArgs = false;
            m_size = 0;

            for (int i = 0; i < param->NumIds(); ++i)
            {
                AddChild(new cVarDeclNode(param->GetId(i), type));
            }
        }

        void AddDecl(cVarDeclNode *node) { AddChild(node); }
        void AddDecls(cVarDeclsNode *decls) { AddAllChildren(decls); }
        int NumDecls() { return NumChildren(); }
        cVarDeclNode * GetDecl(int index) { return dynamic_cast<cVarDeclNode*>(GetChild(index)); }
        virtual string NodeType() { return string("var_decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        bool IsArgs() { return m_isArgs; }
        void SetIsArgs(bool isArgs) { m_isArgs = isArgs; }
        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }

        virtual string AttributesToString()
        {
            string attributes = "";
            
            if (m_size != 0)
            {
                attributes += " size=\"";
                attributes += std::to_string(m_size);
                attributes += "\"";
            }

            return attributes;
        }

    private:
        bool m_isArgs;
        int m_size;
};
