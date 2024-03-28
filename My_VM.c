#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <stdint.h>

#define INSTRUCTION_MEM_SIZE 1024
#define DATA_MEM_SIZE 1024
#define VITUTAL_MEM_SIZE 255
#define REGISTER_SIZE 4
#define NO_REGISTER 32
#define PC_INCREMENT 4

// *---------------Structs---------------*
typedef struct register_made {
    int number;
    uint32_t value;
} Register;


// *---------------General Functions---------------*
void initialize_register(Register register_set[NO_REGISTER]) 
{
    for (int i = 0; i < NO_REGISTER; i++)
    {
        Register register_initial = {i, 0};
        register_set[i] = register_initial;
    }
}

int32_t to_signed(uint32_t unsigned_int, int bit_shift)
{
   int32_t result = (int32_t)unsigned_int;
   result <<= bit_shift;
   result >>= bit_shift;
   return result;
}


// *---------------Functions---------------*

// TYPE R
void R_add (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value + register_set[rs2_mask_value].value;
}

void R_sub (Register register_set[NO_REGISTER], uint32_t rd_mask_value,uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value + register_set[rs2_mask_value].value;
}

void R_xor (Register register_set[NO_REGISTER], uint32_t rd_mask_value,uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value ^ register_set[rs2_mask_value].value;
}

void R_or (Register register_set[NO_REGISTER], uint32_t rd_mask_value,uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value | register_set[rs2_mask_value].value;
}

void R_and (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value & register_set[rs2_mask_value].value;
}

void R_sll (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value << register_set[rs2_mask_value].value;
}

void R_srl (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value >> register_set[rs2_mask_value].value;
}

void R_sra (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = (register_set[rs1_mask_value].value >> register_set[rs2_mask_value].value) | (register_set[rs1_mask_value].value << (32 - register_set[rs2_mask_value].value));
}

void R_slt (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = (register_set[rs1_mask_value].value < register_set[rs2_mask_value].value) ? 1 : 0;
}

void R_sltu (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t rs2_mask_value) {
    register_set[rd_mask_value].value = (register_set[rs1_mask_value].value < register_set[rs2_mask_value].value) ? 1 : 0;
}



// TYPE I
void I_addi (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t imm, int bit_shift) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value + to_signed(imm, bit_shift);
}

void I_oxri (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t imm, int bit_shift) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value ^ to_signed(imm, bit_shift);
}

void I_ori (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t imm, int bit_shift) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value | to_signed(imm, bit_shift);
}

void I_andi (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t imm, int bit_shift) {
    register_set[rd_mask_value].value = register_set[rs1_mask_value].value & to_signed(imm, bit_shift);
}

void I_slti (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t imm, int bit_shift) {
    register_set[rd_mask_value].value = (register_set[rs1_mask_value].value < to_signed(imm, bit_shift)) ? 1 : 0;
}

void I_sltiu (Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, uint32_t immi) {
    register_set[rd_mask_value].value = (register_set[rs1_mask_value].value < immi) ? 1 : 0;
}

void I_jalr (Register register_set[NO_REGISTER], int* PC, uint32_t rd_mask_value, uint32_t rs1_mask_value, int32_t imm, int bit_shift) {
    if (rd_mask_value != 0)
    {
        register_set[rd_mask_value].value = *(PC) + 4; 
    }
    *PC = register_set[rs1_mask_value].value + to_signed(imm, bit_shift);
}

void I_lb (unsigned char* memory, Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, int32_t imm, int bit_shift) {
    unsigned int memory_access = (register_set[rs1_mask_value].value + to_signed(imm, bit_shift));
    int32_t memory_to_register = memory[memory_access];
    memory_to_register <<= 24;
    memory_to_register >>= 24;

    register_set[rd_mask_value].value = memory_to_register;
}

void I_lh (unsigned char* memory, Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, int32_t imm, int bit_shift){
    unsigned int memory_access = (register_set[rs1_mask_value].value + to_signed(imm, bit_shift));
    int32_t memory_to_register = (memory[memory_access]) + (memory[memory_access + 1] << 8); 
    memory_to_register <<= 16;
    memory_to_register >>= 16;

    register_set[rd_mask_value].value = memory_to_register;
}

void I_lw  (unsigned char* memory, Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, int32_t imm, int bit_shift) {
    unsigned int memory_access = (register_set[rs1_mask_value].value + to_signed(imm, bit_shift));
    int32_t memory_to_register = (memory[memory_access]) + ((memory[memory_access + 1] << 8)) + ((memory[memory_access + 2] << 16)) + (memory[memory_access + 3] << 24);

    register_set[rd_mask_value].value = memory_to_register;
}

void I_lbu (unsigned char* memory, Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, int32_t imm) {
    unsigned int memory_access = (register_set[rs1_mask_value].value + imm);
    int32_t memory_to_register = memory[memory_access];

    register_set[rd_mask_value].value = memory_to_register;
}

void I_lhu (unsigned char* memory, Register register_set[NO_REGISTER], uint32_t rd_mask_value, uint32_t rs1_mask_value, int32_t imm) {
    unsigned int memory_access = (register_set[rs1_mask_value].value + imm);
    int32_t memory_to_register = memory[memory_access] + (memory[memory_access + 1] << 8);

    register_set[rd_mask_value].value = memory_to_register;
}






// TYPE S
void S_sb (unsigned char* memory, Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, uint32_t imm, int bit_shift) {
    unsigned int memory_access = (register_set[rs1_mask_value].value + to_signed(imm, bit_shift));
    unsigned int first_8bits = 0xff;

    memory[memory_access] = register_set[rs2_mask_value].value & first_8bits;
}

void S_sh (unsigned char* memory, Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, uint32_t imm, int bit_shift) {
    unsigned int memory_access = (register_set[rs1_mask_value].value + to_signed(imm, bit_shift));
    unsigned int first_8bits = 0xff;
    unsigned int second_8bits = 0xff00;

    memory[memory_access] = register_set[rs2_mask_value].value & first_8bits;
    memory[memory_access + 1] = register_set[rs2_mask_value].value & second_8bits;
}


void S_sw (unsigned char* memory, Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, uint32_t imm, int bit_shift) {
    unsigned int memory_access = (register_set[rs1_mask_value].value + to_signed(imm, bit_shift));
    unsigned int first_8bits = 0xff;
    unsigned int second_8bits = 0xff00;
    unsigned int third_8bits = 0xff0000;
    unsigned int fourth_8bits = 0xff000000;

    memory[memory_access] = register_set[rs2_mask_value].value & first_8bits;
    memory[memory_access + 1] = register_set[rs2_mask_value].value & second_8bits;
    memory[memory_access + 2] = register_set[rs2_mask_value].value & third_8bits;
    memory[memory_access + 3] = register_set[rs2_mask_value].value & fourth_8bits;
}





// TYPE SB
int SB_beq (Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, int* PC, uint32_t imm, int bit_shift) {
    if(register_set[rs1_mask_value].value == register_set[rs2_mask_value].value) {
        *PC = *(PC) + (to_signed(imm, bit_shift));
        return 1;
    }
    return 0;
}

int SB_bne (Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, int* PC, uint32_t imm, int bit_shift) {
    if(register_set[rs1_mask_value].value != register_set[rs2_mask_value].value) {
        *PC = *(PC) + (to_signed(imm, bit_shift));
        return 1;
    }
    return 0;
}

int SB_blt (Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, int* PC, uint32_t imm, int bit_shift) {
     if(register_set[rs1_mask_value].value < register_set[rs2_mask_value].value) {
        *PC = *(PC) + (to_signed(imm, bit_shift));
        return 1;
    }
    return 0;
}

int SB_bltu (Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, int* PC, uint32_t imm) {
     if(register_set[rs1_mask_value].value < register_set[rs2_mask_value].value) {
        *PC = *(PC) + (imm);
        return 1;
    }
    return 0;
}

int SB_bge (Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, int* PC, uint32_t imm, int bit_shift) {
     if(register_set[rs1_mask_value].value >= register_set[rs2_mask_value].value) {
        *PC = *(PC) + (to_signed(imm, bit_shift));
        return 1;
    }
    return 0;
}

int SB_bgeu (Register register_set[NO_REGISTER], uint32_t rs1_mask_value, uint32_t rs2_mask_value, int* PC, uint32_t imm) {
     if(register_set[rs1_mask_value].value >= register_set[rs2_mask_value].value) {
        *PC = *(PC) + (imm);
        return 1;
    }
    return 0;
}




// TYPE U & UJ
void U_lui (Register register_set[NO_REGISTER],  uint32_t rd_mask_value, uint32_t imm) {
    register_set[rd_mask_value].value = imm;
}

void UJ_jal (Register register_set[NO_REGISTER],  uint32_t rd_mask_value, int* PC, uint32_t imm, int bit_shift) {
    register_set[rd_mask_value].value = *(PC) + 4; *PC = *(PC) + (to_signed(imm, bit_shift) << 1);
}













// Virtual Routine
void V_Halt () {
    printf("CPU Halt Requested\n");
}

void V_Console_Write_Character (uint32_t value_written_to) {
    printf("%c", value_written_to);
}

void V_Console_Write_Signed_Integer (uint32_t value_written_to) {
    int32_t signed_int = to_signed(value_written_to, 0);
    printf("%d", signed_int);
}

void V_Console_Write_Unsigned_Integer (uint32_t value_written_to) {
    printf("%x", value_written_to);
}

void V_Console_Read_Character (Register register_set[NO_REGISTER], uint32_t register_to_store) {
    char c;
    c = getchar();
    if (register_to_store != 0) 
    {
        register_set[register_to_store].value = (uint32_t) c;
    }
}
void V_Console_Read_Signed_Integer (Register register_set[NO_REGISTER], uint32_t register_to_store) {
    int i;
    scanf("%d", &i);
    if (register_to_store != 0)
    {
        register_set[register_to_store].value = i;
    }
}

void V_Dump_PC (int* PC) {
    printf("%x\n", *PC);
}

void V_Dump_Register_Banks (Register register_set[NO_REGISTER], int* PC) {
    printf("PC = %x;\n", *PC);
    for (int i = 0; i < NO_REGISTER; i ++) {
        printf("R[%d] = %x;\n", i, register_set[i].value);
    }
}



int Virtual_Routine(uint32_t memory_address, uint32_t value_written_to, Register register_set[NO_REGISTER], uint32_t register_to_store, int* PC) {
    if (memory_address == 0x0800) {
        // Print the value being stored as a single ASCII encoded character to stdout
        V_Console_Write_Character(value_written_to);
    }

    else if (memory_address == 0x0804) {
        // Print the value being stored as a single 32-bit signed integer in decimal format to stdout.
        V_Console_Write_Signed_Integer(value_written_to);
    }

    else if (memory_address == 0x0808) {
        // Print the value being stored as a single 32-bit unsigned integer in lower case hexadecimal format to stdout.
        V_Console_Write_Unsigned_Integer(value_written_to);
    }

    else if (memory_address == 0x080C) {
        // Stop operation
        V_Halt();
        return 1;
    }
    else if (memory_address == 0x0812) {
        // Scan input from stdin and treat the input as an ASCII-encoded character for the memory load result.
        V_Console_Read_Character(register_set, register_to_store);
    }
    else if (memory_address == 0x0816) {
        // Scan input from stdin and parse the input as a signed integer for the memory load result.
        V_Console_Read_Signed_Integer(register_set, register_to_store);
    }
    else if (memory_address == 0x0820) {
        // Print the value of PC in lower case hexadecimal format to stdout.
        V_Dump_PC(PC);
    }
    else if (memory_address == 0x0824) {
        //  Perform an Register Dump. 
        V_Dump_Register_Banks(register_set, PC);
    }
    return 0;

}