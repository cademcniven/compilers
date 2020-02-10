%{
//**************************************
// pascal.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <iostream>
#include "lex.h"
#include "astnodes.h"

extern cSymbolTable g_symbolTable;

%}

%locations

 /* union defines the type for lexical values */
%union{
    int               int_val;
    double            real_val;
    cAstNode*         ast_node;
    cProgramNode*     program_node;
    cBlockNode*       block_node;
    cStmtsNode*       stmts_node;
    cStmtNode*        stmt_node;
    cExprNode*        expr_node;
    cSymbol*          symbol;
    cDeclsNode*       decls_node;
    cDeclNode*        decl_node;
    cExprListNode*    exprList_node;
    cIntExprNode*     intExpr_node;
    cIdListNode*      idList_node;
    cVarDeclsNode*    varDecls_node;
    cVarDeclNode*     varDecl_node;
    cProcDeclsNode*   procDecls_node;
    cProcDeclNode*    procDecl_node;
    cFuncDeclNode*    funcDecl_node;
    cFuncExprNode*    funcExpr_node;
    cRangeDeclNode*   rangeDecl_node;
    cVarExprNode*     varExpr_node;
    cRecordDeclNode*  recordDecl_node;
    }

%{
    int yyerror(const char *msg);
    cAstNode *yyast_root;
%}

%start  program

%token PROGRAM PROCEDURE FUNCTION FORWARD
%token OPEN CLOSE
%token WRITE
%token WHILE IF THEN ELSE REPEAT UNTIL
%token FOR DO TO DOWNTO
%token CONST TYPE VAR
%token RECORD ARRAY OF
%token NIL
%token CHAR INTEGER REAL
%token JUNK_TOKEN

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <real_val>  REAL_VAL
%token <int_val>   AND
%token <int_val>   OR
%token <int_val>   NOT
%token <int_val>   DIV
%token <int_val>   MOD
%token <int_val>   NOT_EQUAL
%token <int_val>   LE
%token <int_val>   GE
%token <int_val>   ASSIGN

%type <program_node> program
%type <symbol> header
%type <block_node> block
%type <symbol> type
%type <decls_node> decls
%type <decls_node> constdecls
%type <decls_node> constdecl
%type <expr_node> constant
%type <decls_node> typedecls
%type <decls_node> typedecl
%type <decl_node> singleType
%type <decls_node> rangeList
%type <rangeDecl_node> range
%type <varDecls_node> goodvar
%type <varDecls_node> onevar
%type <varDecls_node> recorddef
%type <varDecls_node> vardecls
%type <varDecls_node> vardecl;
%type <procDecls_node> procdecls
%type <varDecls_node> paramSpec
%type <decl_node> procdecl
%type <varDecls_node> parlist
%type <idList_node> idlist
%type <funcExpr_node> func_call
%type <funcDecl_node> funcProto
%type <funcDecl_node> funcHeader
%type <procDecl_node> procHeader
%type <stmts_node> statements
%type <stmt_node> statement
%type <expr_node> expr
%type <expr_node> oneExpr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <varExpr_node> variable
%type <symbol> varpart
%type <exprList_node> exprList
%type <recordDecl_node> recHeader
%%

program: header block '.'
                                { $$ = new cProgramNode($1, $2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
header: PROGRAM IDENTIFIER ';'
                                {
                                   g_symbolTable.IncreaseScope();
                                   $$ = $2; 
                                }
block:  decls OPEN statements CLOSE
                                {   
                                    $$ = new cBlockNode($1, $3); 
                                }
decls: constdecls typedecls vardecls procdecls
                                { if ($1 == nullptr && $2 == nullptr &&
                                      $3 == nullptr && $4 == nullptr)
                                    $$ = nullptr;

                                  else
                                  {
                                      $$ = new cDeclsNode(nullptr);
                                      $$->AddDecls($1);
                                      $$->AddDecls($2);
                                      $$->AddDecls($3);
                                      $$->AddDecls($4);
                                  }
                                }
                                  
constdecls: CONST constdecl ';'
                                { $$ = $2;  }
        | /*empty */
                                { $$ = nullptr; }
constdecl: constdecl ';' IDENTIFIER '=' constant 
                                { $$ = $1;
                                  $$->AddDecl(new cConstDeclNode($3, $5)); 
                                }
        |  IDENTIFIER '=' constant 
                                { $$ = new cDeclsNode();
                                  $$->AddDecl(new cConstDeclNode($1, $3)); 
                                }
typedecls: TYPE typedecl
                                { $$ = $2; }
        | /*empty */
                                { $$ = nullptr; }
typedecl: typedecl singleType
                                { $$ = $1;
                                  $$->AddDecl($2); 
                                }
        |  singleType
                                { $$ = new cDeclsNode();
                                  $$->AddDecl($1); 
                                }
        |  error ';'
                                {}
singleType:  IDENTIFIER '=' recHeader recorddef ';'
                                { $$ = new cRecordDeclNode($1, $4); }
        | IDENTIFIER '=' ARRAY '[' rangeList ']' OF type ';'
                                { $$ = new cArrayDeclNode($1, $8, $5); }
rangeList: rangeList ',' range
                                { $$ = $1;
                                  $$->AddDecl($3); 
                                }
        |  range
                                { $$ = new cDeclsNode();
                                  $$->AddDecl($1); 
                                }
range: INT_VAL '.' '.' INT_VAL
                                { $$ = new cRangeDeclNode($1, $4);  }

vardecls: VAR vardecl
                                { $$ = $2; }
        | /* empty */
                                { $$ = nullptr; }
vardecl: vardecl onevar
                                { $$ = $1;
                                  $$->AddDecls($2); 
                                }
        | onevar
                                { $$ = new cVarDeclsNode();
                                  $$->AddDecls($1); 
                                }
onevar: goodvar ';'
                                { $$ = $1; }
        | error ';'
                                { }
goodvar: idlist ':' type
                                { $$ = new cVarDeclsNode($1, $3); }
procdecls: procdecls procdecl
                                { if ($1 == nullptr)
                                    $$ = new cProcDeclsNode();
                                    
                                  $$->AddDecl($2); 
                                }
        | /* empty */
                                { $$ = nullptr; }

procdecl: procHeader paramSpec ';' block ';'
                                { $$ = $1;
                                  $1->AddParams($2);
                                  $1->AddBlock($4);
                                  g_symbolTable.DecreaseScope(); 
                                }
        |  procHeader paramSpec ';' FORWARD ';'
                                { $$ = $1;
                                  $1->AddParams($2);
                                  g_symbolTable.DecreaseScope(); 
                                }
                                  
        |  funcProto ';' block ';'
                                { $$ = $1;
                                  $1->AddBlock($3);
                                  g_symbolTable.DecreaseScope(); 
                                }
        |  funcProto ';' FORWARD ';'
                                { $$ = $1;
                                  g_symbolTable.DecreaseScope(); 
                                }
        |  error ';' block ';'
                                { g_symbolTable.DecreaseScope(); }
        |  error ';' FORWARD ';'
                                { g_symbolTable.DecreaseScope(); }
procHeader: PROCEDURE IDENTIFIER 
                                { $$ = new cProcDeclNode($2);
                                  g_symbolTable.IncreaseScope();
                                }
funcHeader: FUNCTION IDENTIFIER
                                { $$ = new cFuncDeclNode($2);
                                  g_symbolTable.IncreaseScope();
                                }
funcProto: funcHeader paramSpec ':' type
                                { $$ = $1;
                                  $$->AddType($4);
                                  $$->AddParams($2);
                                }
paramSpec: '(' parlist ')'
                                { $$ = $2; }
        | /* empty */
                                { $$ = nullptr; }

idlist: idlist ',' IDENTIFIER
                                { $$ = $1;
                                  $$->AddId($3); 
                                }
    |    IDENTIFIER
                                { $$ = new cIdListNode($1);  }

parlist: parlist ';' VAR idlist ':' type 
                                { $$ = $1;
                                  $$->AddDecls(new cVarDeclsNode($4, $6)); 
                                }
    |    parlist ';' idlist ':' type 
                                { $$ = $1;
                                  $$->AddDecls(new cVarDeclsNode($3, $5)); 
                                }
    |    VAR idlist ':' type
                                { $$ = new cVarDeclsNode($2, $4); }
    |    idlist ':' type
                                { $$ = new cVarDeclsNode($1, $3); }

type: TYPE_ID
                                { $$ = $1; }
recHeader: RECORD
                                { }
recorddef: vardecl CLOSE
                                { $$ = $1; }
constant: INT_VAL
                                { $$ = new cIntExprNode($1); }
    |   '-' INT_VAL
                                { $$ = new cUnaryExprNode('-', new cIntExprNode($2));  }

statements: statements statement
                                { $$ = $1;
                                  $$->AddStmt($2);
                                }
    |   statement
                                { $$ = new cStmtsNode($1); }

statement: variable ASSIGN expr ';'
                                { $$ = new cAssignNode($1, $3); }
    |   IF expr THEN statement
                                { $$ = new cIfNode($2, $4); }
    |   IF expr THEN statement ELSE statement
                                { $$ = new cIfNode($2, $4, $6); }
    |   REPEAT statements UNTIL expr ';'
                                { }
    |   WHILE expr DO statement
                                { $$ = new cWhileNode($2, $4); }
    |   FOR IDENTIFIER ASSIGN expr TO expr DO statement
                                {}
    |   FOR IDENTIFIER ASSIGN expr DOWNTO expr DO statement
                                {}
    |   IDENTIFIER '(' exprList ')' ';'
                                { $$ = new cProcCallNode($1, $3); }
    |   IDENTIFIER ';'
                                { $$ = new cProcCallNode($1, nullptr); }
    |   WRITE '(' exprList ')' ';'
                                { $$ = new cWriteNode($3); }
    |   OPEN statements CLOSE
                                { $$ = new cCompoundStmtNode($2); }
    |   NIL ';'
                                { $$ = new cNilNode(); }
    |   error ';'
                                { }

exprList: exprList ',' oneExpr
                                { $$ = $1;
                                  $$->AddExpr($3); 
                                }
        | oneExpr
                                { $$ = new cExprListNode($1); }
        | /* empty */
                                { $$ = nullptr; }
oneExpr: expr
                                { $$ = $1; }

func_call:  IDENTIFIER '(' exprList ')'
                                { $$ = new cFuncExprNode($1, $3);  }

variable: variable '.' varpart
                                { $$ = $1; 
                                  $$->AddSymbol($3); }
        | variable '[' exprList ']'
                                { $$ = $1;
                                  $$->AddExprList($3); 
                                }
        | varpart
                                { $$ = new cVarExprNode($1); }

varpart:  IDENTIFIER
                                { $$ = $1; }

expr:       expr '=' addit
                                { $$ = new cBinaryExprNode($1, '=', $3); }
        |   expr '>' addit
                                { $$ = new cBinaryExprNode($1, '>', $3); }
        |   expr '<' addit
                                { $$ = new cBinaryExprNode($1, '<', $3); }
        |   expr LE addit
                                { $$ = new cBinaryExprNode($1, LE, $3); }
        |   expr GE addit
                                { $$ = new cBinaryExprNode($1, GE, $3); }
        |   expr NOT_EQUAL addit
                                { $$ = new cBinaryExprNode($1, NOT_EQUAL, $3); }
        |   addit
                                { $$ = $1; }

addit:      addit '+' term
                                { $$ = new cBinaryExprNode($1, '+', $3); }
        |   addit '-' term
                                { $$ = new cBinaryExprNode($1, '-', $3); }
        |   addit OR term
                                { $$ = new cBinaryExprNode($1, OR, $3); }
        |   term
                                { $$ = $1; }
        |   '-' term
                                { $$ = new cUnaryExprNode('-', $2); }

term:       term '*' fact
                                { $$ = new cBinaryExprNode($1, '*', $3); }
        |   term '/' fact
                                { $$ = new cBinaryExprNode($1, '/', $3); }
        |   term MOD fact
                                { $$ = new cBinaryExprNode($1, MOD, $3); }
        |   term DIV fact
                                { $$ = new cBinaryExprNode($1, DIV, $3); }
        |   term AND fact
                                { $$ = new cBinaryExprNode($1, AND, $3); }
        |   fact
                                { $$ = $1; }

fact:        '(' expr ')'
                                { $$ = $2; }
        |   INT_VAL
                                { $$ = new cIntExprNode($1); }
        |   REAL_VAL
                                { $$ = new cRealExprNode($1);  }
        |   variable
                                { $$ = $1; }
        |   func_call
                                { $$ = $1; }
        |   NOT fact
                                { $$ = new cUnaryExprNode(NOT, $2); }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cout << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
