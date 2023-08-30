#include "generator.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define WRITEOUT(str) fwrite(str, sizeof(str) - 1, 1, outfile)
#define WRITEBUF(token) fwrite(token.value, strlen(token.value), 1, outfile)

#define GENPUSH(str) \
WRITEOUT("    push ");\
WRITEOUT(str);\
WRITEOUT("\n");\
(*stack_s)++

#define GENPOP(str) \
WRITEOUT("    pop ");\
WRITEOUT(str);\
WRITEOUT("\n");\
(*stack_s)--

void gen_term(NodeTerm* term, FILE* outfile, int* stack_s)
{
    switch (term->type)
    {
        case NODE_TERM_INT_LIT:
            WRITEOUT("    mov rax, ");
            WRITEBUF(((NodeTermIntLit*)term->var)->int_lit);
            WRITEOUT("\n");
            GENPUSH("rax");
            return;
        //case NODE_TERM_IDENT:
        //    return;
        default:
            printf("Unrecognized term.\n");
    }
}

void gen_expr(NodeExpr* expr, FILE* outfile, int* stack_s)
{
    switch (expr->type)
    {
        case NODE_EXPR_TERM:
            gen_term(expr->var, outfile, stack_s);
            return;
        //case NODE_EXPR_BIN_EXPR
        //    return;
        default:
            printf("Unrecognized expression.\n");
            return;
    }
}

void gen_stmt(NodeStmt stmt, FILE* outfile, int* stack_s)
{
    switch (stmt.type)
    {
        case NODE_STMT_EXIT:
            gen_expr(((NodeStmtExit*)stmt.var)->expr, outfile, stack_s);
            WRITEOUT("    mov rax, 60\n");
            GENPOP("rdi");
            WRITEOUT("    syscall\n");
            return;
        //case NODE_STMT_LET:
        //    return;
        default:
            printf("Unrecognized statement.\n");
            return;
    }
}

void gen_prog(NodeProg prog, FILE* outfile)
{
    printf("Generation started...\n");
    int stack_s = 0;

    WRITEOUT("global _start\n_start:\n");

    for(int i = 0; i < prog.count; i++)
    {
        gen_stmt(prog.stmts[i], outfile, &stack_s);
    }

    WRITEOUT("    mov rax, 60\n    mov rdi, 0\n    syscall\n");

    printf("Generation finished.\n");
}