#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include "My_VM.c"

int main(int argc, char*agrv[]) 
{
    unsigned char memory[INSTRUCTION_MEM_SIZE + DATA_MEM_SIZE];
  
    FILE* file;
    int PC = 0;
    unsigned int opcode_mask = 0x7f;
    unsigned int func3_mask = 0x7000;
    unsigned int func7_mask = 0xfe000000;
    Register register_set[NO_REGISTER];

    unsigned int opcode_mask_value; 
    unsigned int func3_mask_value; 
    unsigned int func7_mask_value;


    initialize_register(register_set);
    file = fopen(agrv[1], "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
   
    fread(memory, sizeof(memory), INSTRUCTION_MEM_SIZE + DATA_MEM_SIZE, file);
   
    while (PC < 1024) 
    {
        unsigned int current_value = memory[PC] | memory[PC + 1] << 8 | 
                                     memory[PC + 2] << 16 | memory[PC + 3] << 24;
        
        opcode_mask_value = current_value & opcode_mask;
        func3_mask_value = (current_value & func3_mask) >> 12;
        func7_mask_value = (current_value & func7_mask) >> 25;


        if (opcode_mask_value == 0b0110011) // R complete
        {
            unsigned int rd_mask = 0xf80; 
            unsigned int rs1_mask = 0xf8000;
            unsigned int rs2_mask = 0x1f00000;

            unsigned int rd_mask_value = (current_value & rd_mask) >> 7;
            unsigned int rs1_mask_value = (current_value & rs1_mask) >> 15;
            unsigned int rs2_mask_value = (current_value & rs2_mask) >> 20;

            if (rd_mask_value != 0)
            {
                if (func3_mask_value == 0b000 && func7_mask_value == 0b0000000) {
                    R_add(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // add

                else if(func3_mask_value == 0b000 && func7_mask_value == 0b0100000) {
                    R_sub(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // sub

                else if(func3_mask_value == 0b100 && func7_mask_value == 0b0000000) {
                    R_xor(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // oxr

                else if(func3_mask_value == 0b110 && func7_mask_value == 0b0000000) {
                    R_or(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // or

                else if(func3_mask_value == 0b111 && func7_mask_value == 0b0000000) {
                    R_and(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // and
                
                else if(func3_mask_value == 0b001 && func7_mask_value == 0b0000000) {
                    R_sll(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // sll

                else if(func3_mask_value == 0b101 && func7_mask_value == 0b0000000) {
                    R_srl(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // srl

                else if(func3_mask_value == 0b101 && func7_mask_value == 0b0100000) {
                    R_sra(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // sra

                else if(func3_mask_value == 0b010 && func7_mask_value == 0b0000000) {
                    R_slt(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                } // slt

                else if(func3_mask_value == 0b011 && func7_mask_value == 0b0000000) {
                    R_sltu(register_set, rd_mask_value, rs1_mask_value, rs2_mask_value); 
                }
            }
        }

        else if (opcode_mask_value == 0b0010011) // I Register complete
        {
            int bit_shift = 20;
            unsigned int rd_mask = 0xf80; 
            unsigned int rs1_mask = 0xf8000;
            unsigned int imm_mask = 0xfff00000;

            unsigned int rd_mask_value = (current_value & rd_mask) >> 7;
            unsigned int rs1_mask_value = (current_value & rs1_mask) >> 15;
            unsigned int imm_mask_value = (current_value & imm_mask) >> 20;

            if (rd_mask_value != 0)
            {
                if(func3_mask_value == 0b000) {
                    I_addi(register_set, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift); 
                }// addi

                else if(func3_mask_value == 0b100) {
                    I_oxri(register_set, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift); 
                }// oxri    

                else if(func3_mask_value == 0b110) {
                    I_ori(register_set, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift); 
                }// ori

                else if(func3_mask_value == 0b111) {
                    I_andi(register_set, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift); 
                }// andi

                else if(func3_mask_value == 0b010) {
                    I_slti(register_set, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift); 
                }// slti

                else if(func3_mask_value == 0b111) {
                    I_sltiu(register_set, rd_mask_value, rs1_mask_value, imm_mask_value); 
                }// slti
            }
        }

        else if (opcode_mask_value == 0b1100111) // I PC complete
        {
            int bit_shift = 20;
            unsigned int rd_mask = 0xf80; 
            unsigned int rs1_mask = 0xf8000;
            unsigned int imm_mask = 0xfff00000;

            unsigned int rd_mask_value = (current_value & rd_mask) >> 7;
            unsigned int rs1_mask_value = (current_value & rs1_mask) >> 15;
            signed int imm_mask_value = (current_value & imm_mask) >> 20;
            
            if(func3_mask_value == 0b000) {
                I_jalr(register_set, &PC, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift); 
            }// jalr
            continue;
        }

        else if (opcode_mask_value == 0b0000011) // I memory complete
        {
            int bit_shift = 20;
            unsigned int rd_mask = 0xf80; 
            unsigned int rs1_mask = 0xf8000;
            unsigned int imm_mask = 0xfff00000;

            unsigned int rd_mask_value = (current_value & rd_mask) >> 7;
            unsigned int rs1_mask_value = (current_value & rs1_mask) >> 15;
            unsigned int imm_mask_value = (current_value & imm_mask) >> 20;

            uint32_t value_to_VR = 0;
            unsigned int memory_address = (register_set[rs1_mask_value].value + to_signed(imm_mask_value, bit_shift));


            if (memory_address >= 0x0800)
            {
                int code = Virtual_Routine(memory_address, value_to_VR, register_set, rd_mask_value, &PC);

                if (code == 1){
                    break;
                }
                PC += PC_INCREMENT;
                continue;
            }

            if (rd_mask_value != 0)
            {
                if(func3_mask_value == 0b000) {
                    I_lb(memory, register_set, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift);
                }
                else if (func3_mask_value == 0b001) {
                    I_lh(memory, register_set, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift);
                }
                else if (func3_mask_value == 0b010) {
                    I_lw(memory, register_set, rd_mask_value, rs1_mask_value, imm_mask_value, bit_shift);
                }
                else if (func3_mask_value == 0b100) {
                    I_lbu(memory, register_set, rd_mask_value, rs1_mask_value, imm_mask_value);
                }
                else if (func3_mask_value == 0b101) {
                    I_lhu(memory, register_set, rd_mask_value, rs1_mask_value, imm_mask_value);
                }
            }
        }

        else if (opcode_mask_value == 0b0100011) // S complete
        {
            int bit_shift = 20;
            unsigned int rs1_mask = 0xf8000;
            unsigned int rs2_mask = 0x1f00000;

            unsigned int first_order_mask = 0xf80;
            unsigned int second_order_mask = 0xfe000000;

            unsigned int first_order_mask_value = (current_value & first_order_mask) >> 7;
            unsigned int second_order_mask_value = (current_value & second_order_mask) >> 20;
            unsigned int rs1_mask_value = (current_value & rs1_mask) >> 15;
            unsigned int rs2_mask_value = (current_value & rs2_mask) >> 20;
            unsigned int imm_mask_value = (first_order_mask_value + second_order_mask_value);

            uint32_t value_to_VR = register_set[rs2_mask_value].value;
            unsigned int memory_address = (register_set[rs1_mask_value].value + to_signed(imm_mask_value, bit_shift));

            if (memory_address >= 0x0800)
            {
                int code = Virtual_Routine(memory_address, value_to_VR, register_set, rs2_mask_value, &PC); // ignore register args

                if (code == 1){
                    break;
                }
                PC += PC_INCREMENT;
                continue;
            }

            if(func3_mask_value == 0b000) {
                S_sb(memory, register_set, rs1_mask_value, rs2_mask_value, imm_mask_value, bit_shift); 
            } //sb

            else if(func3_mask_value == 0b001) {
                S_sh(memory, register_set, rs1_mask_value, rs2_mask_value, imm_mask_value, bit_shift); 
            } //sh

            else if(func3_mask_value == 0b010) {
                S_sw(memory, register_set, rs1_mask_value, rs2_mask_value, imm_mask_value, bit_shift); 
            } //sw
        }

        else if (opcode_mask_value == 0b1100011) //SB complete
        {
            int bit_shift = 19;
            unsigned int rs1_mask = 0xf8000;
            unsigned int rs2_mask = 0x1f00000;

            unsigned int first_order_mask = 0xf00;
            unsigned int second_order_mask = 0x7e000000;
            unsigned int third_order_mask = 0x80;
            unsigned int fourth_order_mask = 0x80000000;

            unsigned int first_order_mask_value = (current_value & first_order_mask) >> 7;
            unsigned int second_order_mask_value = (current_value & second_order_mask) >> 20;
            unsigned int third_order_mask_value = (current_value & third_order_mask) << 4;
            unsigned int fourth_order_mask_value = (current_value & fourth_order_mask) >> 19;
            unsigned int rs1_mask_value = (current_value & rs1_mask) >> 15;
            unsigned int rs2_mask_value = (current_value & rs2_mask) >> 20;
            unsigned int imm_mask_value = (first_order_mask_value + second_order_mask_value + third_order_mask_value + fourth_order_mask_value);
        
            if(func3_mask_value == 0b000) {
                int code = SB_beq(register_set, rs1_mask_value, rs2_mask_value, &PC, imm_mask_value, bit_shift); 
                if (code == 1) {
                    continue;
                }
            }// beq

            else if(func3_mask_value == 0b001) {
                int code = SB_bne(register_set, rs1_mask_value, rs2_mask_value, &PC, imm_mask_value, bit_shift);
                if (code == 1) {
                    continue;
                }
            }// bne

            else if(func3_mask_value == 0b100) {
                int code = SB_blt(register_set, rs1_mask_value, rs2_mask_value, &PC, imm_mask_value, bit_shift); 
                if (code == 1) {
                    continue;
                }
            }// blt

            else if(func3_mask_value == 0b110) {
                int code = SB_bltu(register_set, rs1_mask_value, rs2_mask_value, &PC, imm_mask_value); 
                if (code == 1) {
                    continue;
                }
            }// bltu

            else if(func3_mask_value == 0b101) {
                int code = SB_bge(register_set, rs1_mask_value, rs2_mask_value, &PC, imm_mask_value, bit_shift); 
                if (code == 1) {
                    continue;
                }
            }// bge

            else if(func3_mask_value == 0b111) {
                int code = SB_bgeu(register_set, rs1_mask_value, rs2_mask_value, &PC, imm_mask_value); 
                if (code == 1) {
                    continue;
                }
            }// bgeu
        }
        
        else if (opcode_mask_value == 0b0110111) //U complete
        {
            unsigned int rd_mask = 0xf80; 
            unsigned int imm_mask = 0xfffff000;

            unsigned int imm_mask_value = (current_value & imm_mask);
            unsigned int rd_mask_value = (current_value & rd_mask) >> 7;

            U_lui(register_set, rd_mask_value, imm_mask_value);
        }

        else if (opcode_mask_value == 0b1101111) // UJ complete
        {
            int bit_shift = 12;
            unsigned int rd_mask = 0xf80; 
            unsigned int first_order_mask = 0x7fe00000;
            unsigned int second_order_mask = 0x100000;
            unsigned int third_order_mask = 0xff000;
            unsigned int fourth_order_mask = 0x80000000;

            unsigned int first_order_mask_value = (current_value & first_order_mask) >> 21;
            unsigned int second_order_mask_value = (current_value & second_order_mask) >> 10;
            unsigned int third_order_mask_value = (current_value & third_order_mask) >> 1;
            unsigned int fourth_order_mask_value = (current_value & fourth_order_mask) >> 12;
            unsigned int rd_mask_value = (current_value & rd_mask) >> 7;

            unsigned int imm_mask_value = (first_order_mask_value + second_order_mask_value + third_order_mask_value + fourth_order_mask_value);

            UJ_jal(register_set, rd_mask_value, &PC, imm_mask_value, bit_shift);
            continue;
        }

        PC += PC_INCREMENT;
    }
}