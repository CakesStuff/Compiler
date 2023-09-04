#pragma once

#include "../tokenizer/tokenizer.h"

typedef struct {
    Token int_lit;
} NodeTermIntLit;

typedef struct {
    Token str_lit;
} NodeTermStrLit;

typedef struct {
    Token ident;
} NodeTermIdent;

typedef struct {
    void* lhs;
    void* rhs;
} NodeBinExprAdd;

typedef struct {
    void* lhs;
    void* rhs;
} NodeBinExprSub;

typedef struct {
    void* lhs;
    void* rhs;
} NodeBinExprMul;

typedef struct {
    void* lhs;
    void* rhs;
} NodeBinExprDiv;

typedef struct {
    int type;
    void* var;
} NodeBinExpr;

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

typedef struct {
    NodeTermStrLit* str_lit;
} NodeStmtPrint;

typedef struct {
    Token ident;
    NodeExpr* expr;
} NodeStmtLet;

typedef struct {
    int type;
    void* var;
} NodeStmt;

typedef struct {
    NodeStmt* stmts;
    int count;
} NodeProg;

enum NodeBinExprOptions {
    NODE_BIN_EXPR_MUL,
    NODE_BIN_EXPR_DIV,
    NODE_BIN_EXPR_ADD,
    NODE_BIN_EXPR_SUB,
};

enum NodeTermOptions {
    NODE_TERM_INT_LIT,
    NODE_TERM_STR_LIT,
    NODE_TERM_IDENT,
};

enum NodeExprOptions {
    NODE_EXPR_TERM,
    NODE_EXPR_BIN_EXPR,
};

enum NodeStmtOptions {
    NODE_STMT_EXIT,
    NODE_STMT_PRINT,
    NODE_STMT_LET,
};

NodeProg parse(Token* tokens);
