#include "instructions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"


int translate_hll_to_assembly(const char *hll_code, const char *output_file) {
    // Open the output file for writing
    FILE *out = fopen(output_file, "w");
    if (!out) {
        fprintf(stderr, "Error: Cannot open output file %s for writing.\n", output_file);
        return -1;
    }

    printf("Creating assembly file: %s\n", output_file);

    // Tokenize the HLL code line by line
    char *code_copy = strdup(hll_code);
    char *line = strtok(code_copy, "\n");
    int label_counter = 0;

    while (line) {
        // Skip empty lines or comments
        if (line[0] == '\0' || line[0] == '#') {
            line = strtok(NULL, "\n");
            continue;
        }

        // Debug output
        printf("Processing HLL line: %s\n", line);

        // Parse and generate assembly instructions
        if (strstr(line, "=")) {
            // Handle expressions and assignments
            char var[16], expr[32];
            sscanf(line, "%s = %[^\n]", var, expr);

            char op1[16], op2[16];
            if (sscanf(expr, "%s + %s", op1, op2) == 2) {
                // Handle addition
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "ADD R2, R0, R1\n");
                fprintf(out, "STORE R2, %s\n", var);
            } else if (sscanf(expr, "%s - %s", op1, op2) == 2) {
                // Handle subtraction
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "SUB R2, R0, R1\n");
                fprintf(out, "STORE R2, %s\n", var);
            } else if (sscanf(expr, "%s * %s", op1, op2) == 2) {
                // Handle multiplication
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "MUL R2, R0, R1\n");
                fprintf(out, "STORE R2, %s\n", var);
            } else if (sscanf(expr, "%s / %s", op1, op2) == 2) {
                // Handle division
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "DIV R2, R0, R1\n");
                fprintf(out, "STORE R2, %s\n", var);
            } else {
                // Handle simple assignments
                fprintf(out, "LOAD R0, %s\n", expr);
                fprintf(out, "STORE R0, %s\n", var);
            }
        } else if (strstr(line, "if")) {
            // Handle if conditions
            char condition[32];
            sscanf(line, "if (%[^)])", condition);

            char op1[16], op2[16];
            if (strstr(condition, ">")) {
                sscanf(condition, "%s > %s", op1, op2);
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "GT R2, R0, R1\n");
                fprintf(out, "JZ Else_%d\n", label_counter);
            } else if (strstr(condition, "<")) {
                sscanf(condition, "%s < %s", op1, op2);
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "LT R2, R0, R1\n");
                fprintf(out, "JZ Else_%d\n", label_counter);
            } else if (strstr(condition, "==")) {
                sscanf(condition, "%s == %s", op1, op2);
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "EQ R2, R0, R1\n");
                fprintf(out, "JZ Else_%d\n", label_counter);
            } else if (strstr(condition, "!=")) {
                sscanf(condition, "%s != %s", op1, op2);
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "NEQ R2, R0, R1\n");
                fprintf(out, "JZ Else_%d\n", label_counter);
            }
        } else if (strstr(line, "else")) {
            // Handle else statement
            fprintf(out, "JUMP EndIf_%d\n", label_counter);
            fprintf(out, "Else_%d:\n", label_counter);
        } else if (strstr(line, "print")) {
            // Handle print statements
            char var[16];
            sscanf(line, "print(%[^)]", var);
            fprintf(out, "LOAD R0, %s\n", var);
            fprintf(out, "OUT R0\n");
        } else if (strstr(line, "halt")) {
            // Handle halt
            fprintf(out, "HALT\n");
        } else if (strstr(line, "}")) {
            // Handle end of if blocks
            fprintf(out, "EndIf_%d:\n", label_counter++);
        } else if (strstr(line, "while")) {
            // Handle while loops
            fprintf(out, "WhileStart_%d:\n", label_counter);
            char condition[32];
            sscanf(line, "while (%[^)])", condition);

            char op1[16], op2[16];
            if (strstr(condition, ">")) {
                sscanf(condition, "%s > %s", op1, op2);
                fprintf(out, "LOAD R0, %s\n", op1);
                fprintf(out, "LOAD R1, %s\n", op2);
                fprintf(out, "GT R2, R0, R1\n");
                fprintf(out, "JZ EndWhile_%d\n", label_counter);
            }
        } else if (strstr(line, "endwhile")) {
            // Handle end of while loops
            fprintf(out, "JUMP WhileStart_%d\n", label_counter);
            fprintf(out, "EndWhile_%d:\n", label_counter++);
        }

        // Move to the next line
        line = strtok(NULL, "\n");
    }

    // Cleanup
    free(code_copy);
    fclose(out);

    printf("Assembly file created: %s\n", output_file);
    return 0;
}





// Decode a 32-bit binary instruction into an Instruction struct
Instruction decode_instruction(uint32_t raw) {
    Instruction instr;
    instr.opcode = (Opcode)((raw >> 24) & 0xFF);        // Extract opcode (bits 31-24)
    instr.operands[0] = (raw >> 16) & 0xFF;             // Extract first operand (bits 23-16, 8 bits)
    instr.operands[1] = (raw >> 8) & 0xFF;              // Extract second operand (bits 15-8, 8 bits)
    instr.operands[2] = raw & 0xFF;                     // Extract third operand (bits 7-0, 8 bits)
    // Set addressing modes based on instruction semantics (not encoded in binary)
    // LOAD: dest=REGISTER, src=IMMEDIATE
    // Arithmetic: all=REGISTER
    // Jump: target=IMMEDIATE
    if (instr.opcode == LOAD) {
        instr.modes[0] = REGISTER;
        instr.modes[1] = IMMEDIATE;
        instr.modes[2] = IMMEDIATE;
    } else if (instr.opcode == JUMP || instr.opcode == JZ || instr.opcode == JNZ || instr.opcode == CALL) {
        instr.modes[0] = IMMEDIATE;
        instr.modes[1] = IMMEDIATE;
        instr.modes[2] = IMMEDIATE;
    } else {
        // Default: all operands are registers
        instr.modes[0] = REGISTER;
        instr.modes[1] = REGISTER;
        instr.modes[2] = REGISTER;
    }
    return instr;
}

// Display the decoded instruction for debugging
void display_instruction(Instruction instruction) {
    printf("Opcode: %02X\n", instruction.opcode);
    printf("Operands: %u, %u, %u\n",
           instruction.operands[0],
           instruction.operands[1],
           instruction.operands[2]);
}

// Resolve operand based on addressing mode (moved out of execute_instruction)
uint32_t resolve_operand(const CPU *cpu, uint32_t operand, AddressingMode mode) {
    switch (mode) {
        case IMMEDIATE:
            return operand;
        case REGISTER:
            if (operand >= NUM_REGISTERS) {
                fprintf(stderr, "Error: Register index %d out of bounds.\n", operand);
                return 0; // Return zero to avoid further issues
            }
            return cpu->registers[operand];
        case MEMORY:
            return read_memory(cpu->memory, operand);
        case INDIRECT: {
            uint32_t address = read_memory(cpu->memory, operand);
            return read_memory(cpu->memory, address);
        }
        case INDEXED: {
            uint32_t base = cpu->registers[operand >> 4]; // Upper nibble for base register
            uint32_t offset = operand & 0xF;             // Lower nibble for offset
            return read_memory(cpu->memory, base + offset);
        }
        default:
            fprintf(stderr, "Error: Unknown addressing mode.\n");
            return 0;
    }
}

void execute_instruction(CPU *cpu, Instruction instruction) {
    printf("Executing instruction: Opcode=%02X Operands=%u, %u, %u\n",
           instruction.opcode,
           instruction.operands[0],
           instruction.operands[1],
           instruction.operands[2]);

    // uint32_t *reg = cpu->registers; // Shortcut to registers

    // Instruction Execution
    switch (instruction.opcode) {
        // Arithmetic Operations
        case ADD: {
            uint32_t src1 = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t src2 = resolve_operand(cpu, instruction.operands[2], instruction.modes[2]);
            cpu->registers[instruction.operands[0]] = alu_add(cpu, src1, src2);
            printf("Added %08X and %08X, result in R%d (%08X)\n", src1, src2, instruction.operands[0],
                   cpu->registers[instruction.operands[0]]);
            break;
        }
        case SUB: {
            uint32_t src1 = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t src2 = resolve_operand(cpu, instruction.operands[2], instruction.modes[2]);
            cpu->registers[instruction.operands[0]] = alu_sub(cpu, src1, src2);
            break;
        }
        case MUL: {
            uint32_t src1 = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t src2 = resolve_operand(cpu, instruction.operands[2], instruction.modes[2]);
            cpu->registers[instruction.operands[0]] = src1 * src2;
            break;
        }
        case DIV: {
            uint32_t src1 = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t src2 = resolve_operand(cpu, instruction.operands[2], instruction.modes[2]);
            if (src2 == 0) {
                fprintf(stderr, "Error: Division by zero.\n");
                cpu->halted = true;
            } else {
                cpu->registers[instruction.operands[0]] = src1 / src2;
            }
            break;
        }

        // Logical Operations
        case AND: {
            uint32_t src1 = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t src2 = resolve_operand(cpu, instruction.operands[2], instruction.modes[2]);
            cpu->registers[instruction.operands[0]] = src1 & src2;
            break;
        }
        case OR: {
            uint32_t src1 = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t src2 = resolve_operand(cpu, instruction.operands[2], instruction.modes[2]);
            cpu->registers[instruction.operands[0]] = src1 | src2;
            break;
        }
        case XOR: {
            uint32_t src1 = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t src2 = resolve_operand(cpu, instruction.operands[2], instruction.modes[2]);
            cpu->registers[instruction.operands[0]] = src1 ^ src2;
            break;
        }
        case NOT: {
            uint32_t src = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            cpu->registers[instruction.operands[0]] = ~src;
            break;
        }

        // Shift Operations
        case SHL: {
            uint32_t value = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t shift = instruction.operands[2];
            cpu->registers[instruction.operands[0]] = value << shift;
            break;
        }
        case SHR: {
            uint32_t value = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            uint32_t shift = instruction.operands[2];
            cpu->registers[instruction.operands[0]] = value >> shift;
            break;
        }

        // Memory / Value Load
        case LOAD: {
            // For this ISA and assembler, operands for LOAD are encoded as values
            // (immediates, register contents, or addresses) via resolve_operand.
            // Use the resolved value directly instead of treating it as a memory address.
            uint32_t value = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            cpu->registers[instruction.operands[0]] = value;
            break;
        }
        case STORE: {
            uint32_t value = resolve_operand(cpu, instruction.operands[0], instruction.modes[0]);
            uint32_t address = resolve_operand(cpu, instruction.operands[1], instruction.modes[1]);
            write_memory(cpu->memory, address, value);
            break;
        }

        // Control Flow
        case JUMP:{
            cpu->pc = resolve_operand(cpu, instruction.operands[0], instruction.modes[0]);
            break;}

        case JZ:{
            if (cpu->flags.z) // Zero flag is set
                cpu->pc = resolve_operand(cpu, instruction.operands[0], instruction.modes[0]);
            break;}

        case JNZ:{
            if (!cpu->flags.z) // Zero flag is not set
                cpu->pc = resolve_operand(cpu, instruction.operands[0], instruction.modes[0]);
            break;}


        case CALL: {
            printf("Function Call at PC: %08X\n", cpu->pc);

            // Log function call
            log_function_call("recursive_function", call_depth++, params, param_count);

            // Push current PC onto the stack
            cpu->sp -= 4;
            write_memory(cpu->memory, cpu->sp, cpu->pc);

            // Display the updated stack
            display_stack(cpu);

            // Jump to the function address
            cpu->pc = cpu->registers[instruction.operands[0]];
            break;
        }

        case RET: {
            printf("Returning from Function at PC: %08X\n", cpu->pc);
            cpu->pc = read_memory(cpu->memory, cpu->sp); // Pop return address from the stack
            cpu->sp += 4;
            display_stack(cpu);
            break;
        }


        // Stack Operations
        case PUSH:{
            cpu->sp -= 4;
            write_memory(cpu->memory, cpu->sp, resolve_operand(cpu, instruction.operands[0], instruction.modes[0]));
            break;}
        case POP:{
            cpu->registers[instruction.operands[0]] = read_memory(cpu->memory, cpu->sp);
            cpu->sp += 4;
            break;}

        // System Operations
        case HALT:{
            printf("HALT instruction executed. Stopping CPU.\n");
            cpu->halted = true;
            break;}

        case OUT: {
            uint32_t reg_index = instruction.operands[0];
            if (reg_index >= NUM_REGISTERS) {
                fprintf(stderr, "Error: Invalid register index %u for OUT.\n", reg_index);
                cpu->halted = true;
            } else {
                printf("OUT: R%d = %08X\n", reg_index, cpu->registers[reg_index]);
            }
            break;
        }

        default:{
            fprintf(stderr, "Error: Invalid opcode %02X\n", instruction.opcode);
            cpu->halted = true;
            break;}
    }
}