#include "tokenizer.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define ADD_TOKEN(token) \
if(token_count >= token_capacity)\
{\
    token_capacity *= 2;\
    tokens = realloc(tokens, sizeof(Token) * token_capacity);\
}\
tokens[token_count++] = token

#define ADD_TOKEN_TYPE(type) \
index++;\
Token token = {type, ""};\
token.line = line;\
ADD_TOKEN(token)

Token* tokenize(char* src, int length)
{
    printf("Started tokenizing...\n");
    int token_count = 0;
    int token_capacity = 8;
    Token* tokens = malloc(sizeof(Token) * token_capacity);
    char buffer[11];
    int buf_index = 0;
    int index = 0;
    int line = 1;
    while(index < length)
    {
        if(isalpha(src[index]))
        {
            buffer[buf_index++] = src[index++];
            while(index < length && buf_index < 10 && isalnum(src[index]))
            {
                buffer[buf_index++] = src[index++];
            }
            buffer[buf_index] = 0;
            if(!strcmp(buffer, "exit"))
            {
                Token token = {TOKEN_EXIT, ""};
                token.line = line;
                ADD_TOKEN(token);
                buf_index = 0;
                continue;
            }
            else if(!strcmp(buffer, "let"))
            {
                Token token = {TOKEN_LET, ""};
                token.line = line;
                ADD_TOKEN(token);
                buf_index = 0;
                continue;
            }
            else
            {
                Token token = {TOKEN_IDENT, ""};
                token.line = line;
                strcpy(token.value, buffer);
                ADD_TOKEN(token);
                buf_index = 0;
                continue;
            }
        }
        else if(isdigit(src[index]))
        {
            buffer[buf_index++] = src[index++];
            while(index < length && buf_index < 10 && isdigit(src[index]))
            {
                buffer[buf_index++] = src[index++];
            }
            buffer[buf_index] = 0;
            Token token = {TOKEN_INTLIT, ""};
            token.line = line;
            strcpy(token.value, buffer);
            ADD_TOKEN(token);
            buf_index = 0;
            continue;
        }
        else if(src[index] == ';')
        {
            ADD_TOKEN_TYPE(TOKEN_SEMI);
            continue;
        }
        else if(src[index] == '(')
        {
            ADD_TOKEN_TYPE(TOKEN_OPENP);
            continue;
        }
        else if(src[index] == ')')
        {
            ADD_TOKEN_TYPE(TOKEN_CLOSEP);
            continue;
        }
        else if(src[index] == '=')
        {
            ADD_TOKEN_TYPE(TOKEN_EQ);
            continue;
        }
        else if(src[index] == '+')
        {
            ADD_TOKEN_TYPE(TOKEN_PLUS);
            continue;
        }
        else if(src[index] == '-')
        {
            ADD_TOKEN_TYPE(TOKEN_MINUS);
            continue;
        }
        else if(src[index] == '*')
        {
            ADD_TOKEN_TYPE(TOKEN_MUL);
            continue;
        }
        else if(src[index] == '/')
        {
            ADD_TOKEN_TYPE(TOKEN_DIV);
            continue;
        }
        else if(src[index] == '\n')
        {
            line++;
            index++;
            continue;
        }
        else if(isspace(src[index]))
        {
            index++;
            continue;
        }
        else
        {
            printf("Unknown token start %c at line %d\n", src[index], line);
            free(tokens);
            return NULL;
        }
    }
    printf("Tokenization finished with %d tokens.\n", token_count);
    ADD_TOKEN_TYPE(TOKEN_END);
    return tokens;
}