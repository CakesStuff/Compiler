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
    //else if(ident_case)
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
    //check for binExpr
    //if(...){}
    //else {
    NodeExpr* expr = aalloc(sizeof(NodeExpr));
    expr->type = NODE_EXPR_TERM;
    expr->var = term;
    return expr;
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
        //else if( LET ASSIGNMENT
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
