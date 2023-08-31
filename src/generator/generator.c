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

typedef struct {
    char* name;
    int loc;
} Ident;

typedef struct {
    int cap;
    int i_index;
    Ident* idents;
} Vars;

void gen_term(NodeTerm* term, FILE* outfile, int* stack_s, Vars* vars)
{
    switch (term->type)
    {
        case NODE_TERM_INT_LIT:
            WRITEOUT("    mov rax, ");
            WRITEBUF(((NodeTermIntLit*)term->var)->int_lit);
            WRITEOUT("\n");
            GENPUSH("rax");
            return;
        case NODE_TERM_IDENT:
        {
            Ident* I = NULL;
            NodeTermIdent* term_ident = (NodeTermIdent*)term->var;
            for(int i = 0; i < vars->cap; i++)
            {
                if(!strcmp(vars->idents[i].name, term_ident->ident.value))
                {
                    I = &vars->idents[i];
                    break;
                }
            }
            if(I == NULL)
            {
                printf("Unknown identifier %s at line %d.\n", term_ident->ident.value, term_ident->ident.line);
            }
            WRITEOUT("    push QWORD [rsp + ");
            char buffer[20];
            sprintf(buffer, "%d", ((*stack_s)++ - I->loc - 1) * 8);
            fwrite(buffer, strlen(buffer), 1, outfile);
            WRITEOUT("]\n");
            return;
        }
        default:
            printf("Unrecognized term.\n");
    }
}

void gen_expr(NodeExpr* expr, FILE* outfile, int* stack_s, Vars* vars)
{
    switch (expr->type)
    {
        case NODE_EXPR_TERM:
            gen_term(expr->var, outfile, stack_s, vars);
            return;
        case NODE_EXPR_BIN_EXPR:
        {
            NodeBinExpr* bin_expr = (NodeBinExpr*)expr->var;
            switch (bin_expr->type)
            {
                case NODE_BIN_EXPR_ADD:
                {
                    NodeBinExprAdd* bin_expr_add = (NodeBinExprAdd*)bin_expr->var;
                    gen_expr(bin_expr_add->lhs, outfile, stack_s, vars);
                    gen_expr(bin_expr_add->rhs, outfile, stack_s, vars);
                    GENPOP("rax");
                    GENPOP("rbx");
                    WRITEOUT("    add rax, rbx\n");
                    GENPUSH("rax");
                    return;
                }
                case NODE_BIN_EXPR_SUB:
                {
                    NodeBinExprSub* bin_expr_sub = (NodeBinExprSub*)bin_expr->var;
                    gen_expr(bin_expr_sub->lhs, outfile, stack_s, vars);
                    gen_expr(bin_expr_sub->rhs, outfile, stack_s, vars);
                    GENPOP("rax");
                    GENPOP("rbx");
                    WRITEOUT("    sub rax, rbx\n");
                    GENPUSH("rax");
                    return;
                }
                case NODE_BIN_EXPR_MUL:
                {
                    NodeBinExprMul* bin_expr_mul = (NodeBinExprMul*)bin_expr->var;
                    gen_expr(bin_expr_mul->lhs, outfile, stack_s, vars);
                    gen_expr(bin_expr_mul->rhs, outfile, stack_s, vars);
                    GENPOP("rax");
                    GENPOP("rbx");
                    WRITEOUT("    mul rbx\n");
                    GENPUSH("rax");
                    return;
                }
                case NODE_BIN_EXPR_DIV:
                {
                    NodeBinExprDiv* bin_expr_div = (NodeBinExprDiv*)bin_expr->var;
                    gen_expr(bin_expr_div->rhs, outfile, stack_s, vars);
                    gen_expr(bin_expr_div->lhs, outfile, stack_s, vars);
                    GENPOP("rax");
                    GENPOP("rbx");
                    WRITEOUT("    xor rdx, rdx\n    div rbx\n");
                    GENPUSH("rax");
                    return;
                }
            }
            return;
        }
        default:
            printf("Unrecognized expression.\n");
            return;
    }
}

void gen_stmt(NodeStmt stmt, FILE* outfile, int* stack_s, Vars* vars)
{
    switch (stmt.type)
    {
        case NODE_STMT_EXIT:
            gen_expr(((NodeStmtExit*)stmt.var)->expr, outfile, stack_s, vars);
            WRITEOUT("    mov rax, 60\n");
            GENPOP("rdi");
            WRITEOUT("    syscall\n");
            return;
        case NODE_STMT_LET:
        {
            int found = 0;
            NodeStmtLet* stmt_let = (NodeStmtLet*)stmt.var;
            for(int i = 0; i < vars->cap; i++)
            {
                if(!strcmp(vars->idents[i].name, stmt_let->ident.value))
                {
                    found = 1;
                    break;
                }
            }
            if(found)
            {
                printf("Identifier %s at line %d already used.", stmt_let->ident.value, stmt_let->ident.line);
            }
            else
            {
                vars->idents[vars->i_index].loc = *stack_s;
                vars->idents[vars->i_index++].name = stmt_let->ident.value;
                gen_expr(stmt_let->expr, outfile, stack_s, vars);
            }
            return;
        }
        default:
            printf("Unrecognized statement.\n");
            return;
    }
}

void gen_prog(NodeProg prog, FILE* outfile)
{
    printf("Generation started...\n");
    int stack_s = 0;

    Vars vars;
    vars.cap = prog.count;
    vars.i_index = 0;
    vars.idents = malloc(vars.cap  * sizeof(Ident));
    for(int i = 0; i < vars.cap; i++)
    {
        vars.idents[i].name = "";
    }

    WRITEOUT("global _start\n_start:\n");

    for(int i = 0; i < prog.count; i++)
    {
        gen_stmt(prog.stmts[i], outfile, &stack_s, &vars);
    }

    free(vars.idents);

    WRITEOUT("    mov rax, 60\n    mov rdi, 0\n    syscall\n");

    printf("Generation finished.\n");
}