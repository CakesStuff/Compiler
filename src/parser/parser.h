#pragma once

#include "../tokenizer/tokenizer.h"

typedef struct {
    Token int_lit;
} NodeTermIntLit;

//NodeBinExprAdd

//NodeBinExpr

//NodeTermIdent

typedef struct {
    int type;
    void* var;
} NodeTerm;

typedef struct {
    int type;
    void* var;
} NodeExpr;

typedef struct {
    NodeExpr* expr;
} NodeStmtExit;

//NodeStmtLet

typedef struct {
    int type;
    void* var;
} NodeStmt;

typedef struct {
    NodeStmt* stmts;
    int count;
} NodeProg;

enum NodeTermOptions {
    NODE_TERM_INT_LIT,
    //NODE_TERM_IDENT,
};

enum NodeExprOptions {
    NODE_EXPR_TERM,
    //NODE_EXPR_BIN_EXPR,
};

enum NodeStmtOptions {
    NODE_STMT_EXIT,
    //NODE_STMT_LET,
};

NodeProg parse(Token* tokens);
