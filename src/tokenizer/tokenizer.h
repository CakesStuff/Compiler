#pragma once

typedef enum {
    TOKEN_SEMI,
    TOKEN_EXIT,
    TOKEN_INTLIT,
    TOKEN_END,
    TOKEN_OPENP,
    TOKEN_CLOSEP,
} TokenType;

typedef struct {
    TokenType type;
    char value[10];
    int line;
} Token;

Token* tokenize(char* src, int length);