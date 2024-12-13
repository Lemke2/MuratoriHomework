#include <stdio.h>
#include <stdlib.h>
#include "shared/sim86_shared.h"
#include "sim86_text.h"

static u8* LoadMemoryFromFile(const char *filename, long int *filesize) {
    // Open the file in binary mode
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "ERROR: Unable to open file %s.\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *filesize = ftell(file);
    rewind(file);

    u8 *buffer = (u8*)malloc(*filesize);
    if (!buffer) {
        fprintf(stderr, "ERROR: Unable to allocate memory for file %s.\n", filename);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *filesize, file);
    fclose(file);

    return buffer;
}


int main(int argCount, char **Args)
{   
    u16 register_values[8] = {}; 
    const char* register_names[8] = {"ax", "bx", "cx", "dx", "sp", "bp", "si", "di"};

    char *filename = Args[1];
    long int filesize = 0;
    u8* buffer = LoadMemoryFromFile(filename, &filesize);
    
    printf("filesize: %d\n", filesize);
    printf("Size of buffer: %zu bytes\n", sizeof(*buffer));
    FILE *file = fopen("output.txt", "w");

    u32 offset = 0;
    while(offset < filesize)
    {
        instruction instruction;
        Sim86_Decode8086Instruction(sizeof(buffer) - offset, buffer + offset, &instruction);
        if(instruction.Op)
        {
            offset += instruction.Size;
            instruction_operand operand1 = instruction.Operands[0];
            instruction_operand operand2 = instruction.Operands[1];

            const u16 ind = operand1.Register.Index;
            const u16 val = operand2.Immediate.Value;

            register_values[ind - 1] = val;
        }
        else
        {
            printf("Unrecognized instruction\n");
            break;
        }

    }

    for(int i = 0; i < 8; i++){
        printf(register_names[i]);
        printf(" = %d\n", register_values[i]);
    }

    fclose(file);
    return 0;
}
