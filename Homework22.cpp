#include "stdio.h"
#include "stdlib.h"
#include "shared/sim86_shared.h"

static u8 *LoadMemoryFromFile(const char *filename, u32 *filesize)
{   
    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    *filesize = ftell(file);
    rewind(file);

    u8 *buffer = (u8*)malloc(*filesize);
    fread(buffer, 1, *filesize, file);

    fclose(file);
    return buffer;
}

int main(int argCount, char **Args)
{
    u16 registers[8] = {};
    const char* register_names[8] = {"ax", "bx", "cx", "dx", "sp", "bp", "si", "di"};
    bool zeroflag = false;
    bool signflag = false;

    char *filename = Args[1];
    u32 filesize = 0;
    u8 *buffer = LoadMemoryFromFile(filename, &filesize);

    u32 offset = 0;
    while(offset < filesize)
    {
        instruction instruction;
        Sim86_Decode8086Instruction(filesize - offset, buffer + offset, &instruction);
        offset += instruction.Size;
        instruction_operand operand1 = instruction.Operands[0];
        instruction_operand operand2 = instruction.Operands[1];
        const u16 ind = operand1.Register.Index;
        
        if (instruction.Op == Op_mov)
        {   
            if (operand2.Type == Operand_Immediate)
            {
                const u16 val = operand2.Immediate.Value;
                registers[ind - 1] = val;
            }
            else if (operand2.Type == Operand_Register)
            {
                const u16 ind2 = operand2.Register.Index;
                registers[ind-1] = registers[ind2-1];
            }
        }

        else if (instruction.Op == Op_add)
        {
            if (operand2.Type == Operand_Immediate)
            {
                const u16 val = operand2.Immediate.Value;
                registers[ind - 1] += val;
            }
            else if (operand2.Type == Operand_Register)
            {
                const u16 ind2 = operand2.Register.Index;
                registers[ind-1] += registers[ind2-1];
            }

            if (registers[ind-1] == 0) zeroflag = true;
            else zeroflag = false;

            if ((registers[ind-1] & (1 << 15)) != 0) signflag = true;
            else signflag = false;
        }
        
        else if (instruction.Op == Op_sub)
        {
            if (operand2.Type == Operand_Immediate)
            {
                const u16 val = operand2.Immediate.Value;
                registers[ind - 1] -= val;
            }
            else if (operand2.Type == Operand_Register)
            {
                const u16 ind2 = operand2.Register.Index;
                registers[ind-1] -= registers[ind2-1];
            }

            if (registers[ind-1] == 0) zeroflag = true;
            else zeroflag = false;

            if ((registers[ind-1] & (1 << 15)) != 0) signflag = true;
            else signflag = false;
        }
        
        else if (instruction.Op == Op_cmp)
        {
            u16 temp = 0;
            if (operand2.Type == Operand_Immediate)
            {
                const u16 val = operand2.Immediate.Value;
                temp = registers[ind - 1] - val;

            }
            else if (operand2.Type == Operand_Register)
            {
                const u16 ind2 = operand2.Register.Index;
                temp = registers[ind-1] - registers[ind2-1];
            }
            if (temp == 0) zeroflag = true;
            else zeroflag = false;

            if ((temp & (1 << 15)) != 0) signflag = true;
            else signflag = false; 
        }
        
        for(int i = 0; i < 8; i++){
            printf(register_names[i]);
            printf(" = %d\n", registers[i]);
        }
        printf("Zeroflag: %d\n", zeroflag);
        printf("Signflag: %d\n", signflag);
        printf("\n");
    }

    

    return 0;
}