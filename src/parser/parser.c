#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include "arena_allocator/allocator.h"

#define ADD_STMT(stmt) \
if(prog.count >= capacity)\
{\
    capacity *= 2;\
    prog.stmts = realloc(prog.stmts, sizeof(NodeStmt) * capacity);\
}\
prog.stmts[prog.count++] = stmt

#define BIN_EXPR_GEN(node, operator) \
(*index)++;\
NodeBinExpr* bin_expr = aalloc(sizeof(NodeBinExpr));\
node* bin_expr_op = aalloc(sizeof(node));\
NodeExpr* lhs = aalloc(sizeof(NodeExpr));\
lhs->type = NODE_EXPR_TERM;\
lhs->var = term;\
NodeExpr* rhs = parse_expr(tokens, index);\
if(rhs == NULL)\
{\
    return NULL;\
}\
bin_expr_op->lhs = lhs;\
bin_expr_op->rhs = rhs;\
bin_expr->type = operator;\
bin_expr->var = bin_expr_op;\
NodeExpr* expr = aalloc(sizeof(NodeExpr));\
expr->type = NODE_EXPR_BIN_EXPR;\
expr->var = bin_expr;\
if(rhs->type == NODE_EXPR_BIN_EXPR && ((NodeBinExpr*)rhs->var)->type > operator)\
{\
    void* temp = bin_expr_op->lhs;\
    bin_expr_op->lhs = ((NodeBinExprAdd*)((NodeBinExpr*)rhs->var)->var)->rhs;\
    ((NodeBinExprAdd*)((NodeBinExpr*)rhs->var)->var)->rhs = ((NodeBinExprAdd*)((NodeBinExpr*)rhs->var)->var)->lhs;\
    ((NodeBinExprAdd*)((NodeBinExpr*)rhs->var)->var)->lhs = temp;\
    bin_expr->type = ((NodeBinExpr*)rhs->var)->type;\
    ((NodeBinExpr*)rhs->var)->type = operator;\
}\
return expr

NodeTerm* parse_term(Token* tokens, int* index)
{
    if(tokens[*index].type == TOKEN_INTLIT)
    {
        NodeTermIntLit* term_int_lit = aalloc(sizeof(NodeTermIntLit));
        term_int_lit->int_lit = tokens[(*index)++];
        NodeTerm* term = aalloc(sizeof(NodeTerm));
        term->type = NODE_TERM_INT_LIT;
        term->var = term_int_lit;
        return term;
    }
    else if(tokens[*index].type == TOKEN_IDENT)
    {
        NodeTermIdent* term_ident = aalloc(sizeof(NodeTermIdent));
        term_ident->ident = tokens[(*index)++];
        NodeTerm* term = aalloc(sizeof(NodeTerm));
        term->type = NODE_TERM_IDENT;
        term->var = term_ident;
        return term;
    }
    else
    {
        printf("Unrecognized term at line ~%d.\n", tokens[*index].line);
        return NULL;
    }
}

NodeExpr* parse_expr(Token* tokens, int* index)
{
    NodeTerm* term = parse_term(tokens, index);
    if(term == NULL)
    {
        return NULL;
    }
    if(tokens[*index].type == TOKEN_PLUS)
    {
        BIN_EXPR_GEN(NodeBinExprAdd, NODE_BIN_EXPR_ADD);
    }
    else if(tokens[*index].type == TOKEN_MINUS)
    {
        BIN_EXPR_GEN(NodeBinExprSub, NODE_BIN_EXPR_SUB);
    }
    else if(tokens[*index].type == TOKEN_MUL)
    {
        BIN_EXPR_GEN(NodeBinExprMul, NODE_BIN_EXPR_MUL);
    }
    else if(tokens[*index].type == TOKEN_DIV)
    {
        BIN_EXPR_GEN(NodeBinExprDiv, NODE_BIN_EXPR_DIV);
    }
    else
    {
        NodeExpr* expr = aalloc(sizeof(NodeExpr));
        expr->type = NODE_EXPR_TERM;
        expr->var = term;
        return expr;
    }
}

NodeProg parse(Token* tokens)
{
    printf("Starting parsing...\n");
    int index = 0;
    int capacity = 8;
    NodeProg prog = {malloc(sizeof(NodeStmt) * capacity), 0};

    while(tokens[index].type != TOKEN_END)
    {
        if(tokens[index].type == TOKEN_EXIT && tokens[index + 1].type == TOKEN_OPENP)
        {
            index += 2;
            NodeStmtExit* stmt_exit = aalloc(sizeof(NodeStmtExit));
            stmt_exit->expr = parse_expr(tokens, &index);
            if(stmt_exit->expr == NULL)
            {
                NodeProg ret = {NULL};
                return ret;
            }
            NodeStmt stmt = {NODE_STMT_EXIT, stmt_exit};
            ADD_STMT(stmt);
            if(tokens[index++].type != TOKEN_CLOSEP)
            {
                printf("Expected ')' in line %d.\n", tokens[index - 1].line);
                NodeProg ret = {NULL};
                return ret;
            }
            if(tokens[index++].type != TOKEN_SEMI)
            {
                printf("Expected ';' in line %d.\n", tokens[index - 1].line);
                NodeProg ret = {NULL};
                return ret;
            }
        }
        else if(tokens[index].type == TOKEN_LET && tokens[index + 1].type == TOKEN_IDENT && tokens[index + 2].type == TOKEN_EQ)
        {
            index++;
            NodeStmtLet* stmt_let = aalloc(sizeof(NodeStmtLet));
            stmt_let->ident = tokens[index++];
            index++;
            stmt_let->expr = parse_expr(tokens, &index);
            if(stmt_let->expr == NULL)
            {
                NodeProg ret = {NULL};
                return ret;
            }
            if(tokens[index++].type != TOKEN_SEMI)
            {
                printf("Expected ';' in line %d.\n", tokens[index - 1].line);
                NodeProg ret = {NULL};
                return ret;
            }
            NodeStmt stmt = {NODE_STMT_LET, stmt_let};
            ADD_STMT(stmt);
        }
        else
        {
            printf("Unrecognized sequence starting with node %d at line %d.\n", tokens[index].type, tokens[index].line);
            NodeProg ret = {NULL};
            return ret;
        }
    }
    printf("Parsing finished with %d statements.\n", prog.count);
    return prog;
}
