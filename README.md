Replicate the 32 core functions of the RISK-XVII. The program takes in instructions in bits and outputs according to its pre-determined functions.


• 0x0000 - 0x3ff: Instruction Memory - Contains 210 of bytes for text segment.
• 0x0400 - 0x7ff: Data Memory - Contains 210 of bytes for global variables, and function stack.
• 0x0800 - 0x8ff: Virtual Routines - Accesses to these address will cause special operations to be called.
• 0xb700 +: Heap Banks - Hardware managed 128 x 64 bytes banks of dynamically allocate-able memory.


There are in total 33 instructions defined in RISK-XVII, they can be classified into three groups by their functionality:
1. Arithmetic and Logic Operations - e.g. add, sub, and, or, slt, sll
2. Memory Access Operations - e.g. sw, lw, lui
3. Program Flow Operations - e.g. jal, beq, blt

Instructions in the RISK-XVII instruction set architecture are encoded into 4 bytes of data. Since each instruction can access different parts of the system, six types of encoding formats were designed to best utilize the 32 bits of data to represent the operations specified by each instruction: R, I, S, SB, U, UJ. The exact binary format of each encoding type can be found in the table below.:

<img width="660" alt="Screenshot 2024-03-28 at 9 53 09 pm" src="https://github.com/KimberlieFu/VM_RiskXVII/assets/89679326/73b42524-cce9-4804-a7ce-b819845a378b">

• opcode - used in all encoding to differentiate the operation, and even the encoding type itself.
• rd, rs1, rs2 - register specifiers. rs1 and rs2 specify registers to be used as the source operand, while rd specifies the target register. Note that since there are 32 registers in total, all register specifiers are 5 bits in length.
• func3, func7 - these are additional opcodes that specify the operation in more detail. For example, all arithmetic instructions may use the same opcode, but the actual operation, e.g. add, logic shift, are defined by the value of func3.
• imm - immediate numbers. These value can be scrambled within the instruction encoding. For example, in SB, the 11st bit of the actual value was encoded at the 7th bit of the instruction, while the 12rd bit was encoded at the 31th bit.
