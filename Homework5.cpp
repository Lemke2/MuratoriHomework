#include <stdio.h>
#include <stdlib.h>
#include "shared/sim86_shared.h"

static u8 *LoadMemoryFromFile(char* filename, u32 *filesize)
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
    u8 memory[1024*1024] = {};

    char *filename = Args[1];
    u32 filesize = 0;
    u8 *buffer = LoadMemoryFromFile(filename, &filesize);

    u32 offset = 0;
    printf("filesize: %d\n", filesize);
    while(offset < filesize)
    {   
        printf("IP at: %d\n", offset);
        instruction instruction;
        Sim86_Decode8086Instruction(filesize - offset, buffer + offset, &instruction);
        offset += instruction.Size;
        printf("Offset: %d\n", offset);
        instruction_operand operand1 = instruction.Operands[0];
        instruction_operand operand2 = instruction.Operands[1];

        if (instruction.Op == Op_mov)
        {   
            if (operand1.Type == Operand_Register)
            {
                u16 ind = operand1.Register.Index;
                if (operand2.Type == Operand_Immediate)
                {
                    const u16 val = operand2.Immediate.Value;
                    registers[ind - 1] = val;
                }
                else if (operand2.Type == Operand_Register)
                {
                    const u16 ind2 = operand2.Register.Index;
                    registers[ind - 1] = registers[ind2 - 1];
                }
                else if (operand2.Type == Operand_Memory)
                {   
                    const u32 val = operand2.Address.Displacement;
                    u16 regval3 = 0;
                    u16 regval4 = 0;
                    if (operand2.Address.Terms[0].Register.Index != 0) regval3 = operand2.Address.Terms[0].Register.Index - 1;
                    if (operand2.Address.Terms[1].Register.Index != 0) regval4 = operand2.Address.Terms[1].Register.Index - 1;
                    
                    registers[ind - 1] = memory[val + registers[regval3] + registers[regval4]];
                }
            }

            else if (operand1.Type == Operand_Memory)
            {   
                const u32 memval = operand1.Address.Displacement;
                u16 regval1 = 0;
                u16 regval2 = 0;
                if (operand1.Address.Terms[0].Register.Index != 0) regval1 = operand1.Address.Terms[0].Register.Index - 1;
                if (operand1.Address.Terms[1].Register.Index != 0) regval2 = operand1.Address.Terms[1].Register.Index - 1;

                if (operand2.Type == Operand_Immediate)
                {
                    const u16 val = operand2.Immediate.Value;
                    memory[registers[regval1] + registers[regval2] + memval] = val;
                }
                else if (operand2.Type == Operand_Register)
                {
                    const u16 ind2 = operand2.Register.Index;
                    memory[registers[regval1] + registers[regval2] + memval] = registers[ind2 - 1];
                }
                else if (operand2.Type == Operand_Memory)
                {
                    const u32 val = operand2.Address.Displacement;
                    u16 regval3 = 0;
                    u16 regval4 = 0;
                    if (operand2.Address.Terms[0].Register.Index != 0) regval3 = operand2.Address.Terms[0].Register.Index - 1;
                    if (operand2.Address.Terms[1].Register.Index != 0) regval4 = operand2.Address.Terms[1].Register.Index - 1;

                    memory[registers[regval1] + registers[regval2] + memval] = memory[val + registers[regval3] + registers[regval4]];
                }
            }
        }

        else if (instruction.Op == Op_add)
        {
            const u16 ind = operand1.Register.Index;
            if (operand2.Type == Operand_Immediate)
            {
                const u16 val = operand2.Immediate.Value;
                registers[ind - 1] += val;
            }
            else if (operand2.Type == Operand_Register)
            {
                const u16 ind2 = operand2.Register.Index;
                registers[ind - 1] += registers[ind2 - 1];
            }

            if (registers[ind - 1] == 0) zeroflag = true;
            else zeroflag = false;

            if ((registers[ind - 1] & (1 << 15)) != 0) signflag = true;
            else signflag = false;
        }
        
        else if (instruction.Op == Op_sub)
        {
            const u16 ind = operand1.Register.Index;
            if (operand2.Type == Operand_Immediate)
            {
                const u16 val = operand2.Immediate.Value;
                registers[ind - 1] -= val;
            }
            else if (operand2.Type == Operand_Register)
            {
                const u16 ind2 = operand2.Register.Index;
                registers[ind - 1] -= registers[ind2 - 1];
            }

            if (registers[ind - 1] == 0) zeroflag = true;
            else zeroflag = false;

            if ((registers[ind-1] & (1 << 15)) != 0) signflag = true;
            else signflag = false;
        }
        
        else if (instruction.Op == Op_cmp)
        {
            const u16 ind = operand1.Register.Index;
            u16 temp = 0;
            if (operand2.Type == Operand_Immediate)
            {
                const u16 val = operand2.Immediate.Value;
                temp = registers[ind - 1] - val;

            }
            else if (operand2.Type == Operand_Register)
            {
                const u16 ind2 = operand2.Register.Index;
                temp = registers[ind - 1] - registers[ind2 - 1];
            }
            if (temp == 0) zeroflag = true;
            else zeroflag = false;

            if ((temp & (1 << 15)) != 0) signflag = true;
            else signflag = false; 
        }

        else if (instruction.Op == Op_jne)
        {   
            if (!zeroflag)
            {
                const s16 jmp_value = operand1.Immediate.Value;
                offset += jmp_value;
            }
            
        }

        // print state of all registers and flags and memory
        for(int i = 0; i < 8; i++){
            printf(register_names[i]);
            printf(" = %d\n", registers[i]);
        }
        printf("Zeroflag: %d\n", zeroflag);
        printf("Signflag: %d\n", signflag);
        for(int i = 1000; i < 1010; i++)
        {
            printf("Memory[%d", i);
            printf("] = %d\n", memory[i]);
        }
        printf("\n");

    }

    

    return 0;
}