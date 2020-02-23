#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Authors: Phil Howard 
// phil.howard@oit.edu
// cade.mcniven@oit.edu
//

#include "cAstNode.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}
        virtual bool IsReal() { return false; }
        virtual bool IsInt() { return false; }
        virtual bool IsChar() { return false; }
        virtual bool IsType() { return false; }
        virtual bool IsFunc() { return false; }
        virtual bool IsVar() { return false; }
        virtual int GetSize() { return 0; }
        virtual string GetTypeName() = 0;
};
