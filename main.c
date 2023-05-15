// gcc main.c -o challenge -g -Wl,-z,relro,-z,lazy

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_IN_BUF 200
#define MAX_OUT_BUF 200
#define MEMORY_SIZE 2000

int * memory = 0xdeadbeef;

char input[MAX_IN_BUF] = {1};
char output[MAX_OUT_BUF] = {1};
char FMT_STR[20] = "%s";

int in_pointer = 0;
int out_pointer = 0;

int add(int *args)
{
    memory[args[0]] = memory[args[1]] + memory[args[2]];
    return 3;
}

int sub(int *args)
{
    memory[args[0]] = memory[args[1]] - memory[args[2]];
    return 3;
}

int mul(int *args)
{
    memory[args[0]] = memory[args[1]] * memory[args[2]];
    return 3;
}

int divide(int *args)
{
    if (memory[2])
    {
        return 3;
    }

    memory[args[0]] = memory[args[1]] / memory[args[2]];
    return 3;
}

int rst_in(int *args)
{
    in_pointer = 0;
    return 0;
}

int rst_out(int *args)
{
    out_pointer = 0;
    return 0;
}

int pop_in(int *args)
{
    for (int i = 0; i < args[1]; i++)
        memory[args[0]+i] = input[in_pointer++];
    return 2;
}

int pop_out(int *args)
{
    for (int i = 0; i < args[1]; i++)
        output[out_pointer++] = memory[args[0]+i];
    return 2;
}

int read_buf(int *args)
{
    if (args[0] >= MAX_IN_BUF)
    {
        return 1;
    }

    fgets(input, args[0], stdin);
    return 1;
}

int write_buf(int *args, volatile char ** cnt)
{
    if (args[0] >= MAX_OUT_BUF)
    {
        return 1;
    }

    printf(FMT_STR, *cnt);
    return 1;
}

int len(int *args) {
    //TODO unintended!
    char coso[200];
    int i = 0;
    int * test = &memory[args[0]];
    while(*test != 0) {
        coso[i++] = (char) *test++;
    }
    memory[args[1]] = strlen(coso);
    return 2;
}

int jmp(int *args)
{
    if (args[1] == 0 || memory[args[1]] != 0) {
        return args[0];
    }

    return 2;
}

int njmp(int *args)
{
    if (args[1] == 0 || memory[args[1]] == 0) {
        return args[0];
    }

    return 2;
}


void loop(int *text, int *end)
{
    int *pc = text;
    volatile char * tmp = output;

    // Run if opcode is not `exit` and pc is in boundaries
    while (pc >= text && pc < end && *pc != 0)
    {
        // switch opcode
        switch (*pc)
        {
        case 1:
            pc += add(pc + 1) + 1;
            break;
        case 2:
            pc += sub(pc + 1) + 1;
            break;
        case 3:
            pc += mul(pc + 1) + 1;
            break;
        case 4:
            pc += divide(pc + 1) + 1;
            break;
        case 5:
            pc += rst_in(pc + 1) + 1;
            break;
        case 6:
            pc += rst_out(pc + 1) + 1;
            break;
        case 7:
            pc += pop_in(pc + 1) + 1;
            break;
        case 8:
            pc += pop_out(pc + 1) + 1;
            break;
        case 9:
            pc += read_buf(pc + 1) + 1;
            break;
        case 10:
            pc += write_buf(pc + 1, &tmp) + 1;
            break;
        case 11:
            pc += jmp(pc + 1) + 1;
            break;
        case 12:
            pc += njmp(pc + 1) + 1;
            break;
        case 13:
            pc += len(pc + 1) + 1;
            break;
        default:
            exit(1);
            break;
        }
    }
}

int *readProgram(char *filename, int *size)
{
    FILE *prog = fopen(filename, "r");

    if (prog == NULL)
    {
        puts("Cannot open file!");
        exit(1);
    }

    fseek(prog, 0, SEEK_END);
    *size = ftell(prog);
    fseek(prog, 0, SEEK_SET);

    int *text = malloc(*size);
    fread(text, sizeof(int), *size, prog);

    fclose(prog);
    return text;
}

int *readMemory(char *filename)
{
    FILE *prog = fopen(filename, "r");

    if (prog == NULL)
    {
        puts("Cannot open file!");
        exit(1);
    }

    int * mem = malloc(MEMORY_SIZE * sizeof(int));
    fread(mem, sizeof(int), MEMORY_SIZE, prog);

    fclose(prog);
    return mem;
}

int main(int argv, char **argc)
{
      setbuf(stdin,0x0);
  setbuf(stdout,0x0);
    int size;
    int *text = readProgram(argc[1], &size);
    memory = readMemory(argc[2]);

    loop(text, text + size);

    free(text);
    free(memory);
}
