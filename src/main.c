#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer/tokenizer.h"
#include "parser/parser.h"
#include "parser/arena_allocator/allocator.h"
#include "generator/generator.h"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s <source> <out>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        printf("Could not open source file.\n");
        return 2;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    if(size == 0)
    {
        printf("File %s is empty.\n", argv[1]);
        return 3;
    }

    printf("Starting compilation on file: %s with size: %dB\n", argv[1], size);

    char file_buffer[size];
    if(!fread(&file_buffer, size, 1, file))
    {
        printf("Unable to read file.\n");
        fclose(file);
        return 4;
    }

    fclose(file);

    //TOKENIZER
    Token* tokens = tokenize(file_buffer, size);
    if(tokens == NULL)
    {
        printf("Unable to tokenize.\n");
        return 5;
    }

    ainit();

    //PARSER
    NodeProg program = parse(tokens);
    if(program.stmts == NULL)
    {
        printf("Unable to parse.\n");
        free(tokens);
        aclose();
        return 6;
    }

    free(tokens);

    //GENERATOR
    FILE* out_file = fopen("out.asm", "w");
    if(out_file == NULL)
    {
        printf("Could not open outfile %s.\n", "out.asm");
        aclose();
        free(program.stmts);
        return 7;
    }

    gen_prog(program, out_file);

    fclose(out_file);

    printf("Compilation finished.\n");

    aclose();
    free(program.stmts);

    system("nasm -felf64 out.asm");

    char* ldStart = "ld -o ";
    //TODO: ADD STDLIB HERE
    char* ldEnd = " out.o";
    char ldBuf[strlen(ldStart) + strlen(argv[2]) + strlen(ldEnd) + 1];

    strcpy(ldBuf, ldStart);
    strcpy(ldBuf + strlen(ldStart), argv[2]);
    strcpy(ldBuf + strlen(ldStart) + strlen(argv[2]), ldEnd);

    system(ldBuf);

    return 0;
}