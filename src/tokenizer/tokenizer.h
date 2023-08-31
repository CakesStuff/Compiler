#pragma once

typedef enum {
    TOKEN_SEMI,
    TOKEN_EXIT,
    TOKEN_INTLIT,
    TOKEN_END,
    TOKEN_OPENP,
    TOKEN_CLOSEP,
    TOKEN_IDENT,
    TOKEN_LET,
    TOKEN_EQ,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
} TokenType;

typedef struct {
    TokenType type;
    char value[10];
    int line;
} Token;

Token* tokenize(char* src, int length);