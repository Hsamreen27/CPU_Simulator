#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "instructions.h" // For executing instructions
#include <stdlib.h>
#include <ctype.h>
#include "debug.h"

int call_depth = 0;
uint32_t params[10] = {0};
int param_count = 0;

// Initialize the CPU
void init_cpu(CPU *cpu) {
    memset(cpu->registers, 0, sizeof(cpu->registers)); // Clear all registers

    // Clear all flags
    cpu->flags.z = 0;
    cpu->flags.n = 0;
    cpu->flags.o = 0;

    cpu->pc = CODE_START;                              // Set PC to start of code segment
    printf("Initial PC: %08X\n", cpu->pc);

    cpu->sp = STACK_END;                               // Set SP to the top of the stack
    cpu->heap_pointer = HEAP_START;                    // Set heap pointer to start of heap
    memset(cpu->memory, 0, MEMORY_SIZE);               // Clear memory
    cpu->halted = false;                               // Ensure CPU is not halted
}

// Reset the CPU
void reset_cpu(CPU *cpu) {
    memset(cpu->registers, 0, sizeof(cpu->registers)); // Clear all registers

    // Clear all flags
    cpu->flags.z = 0;
    cpu->flags.n = 0;
    cpu->flags.o = 0;

    cpu->pc = CODE_START;                              // Reset PC
    cpu->sp = STACK_END;                               // Reset SP
    cpu->heap_pointer = HEAP_START;                    // Reset heap pointer
    memset(cpu->memory, 0, MEMORY_SIZE);               // Clear memory
    cpu->halted = false;                               // Ensure CPU is not halted
}



// Fetch an instruction from memory
static uint32_t fetch_instruction(CPU *cpu) {
    if (cpu->pc < CODE_START || cpu->pc > CODE_END) {
        fprintf(stderr, "Error: PC out of memory bounds at %08X.\n", cpu->pc);
        cpu->halted = true;
        return 0;
    }
    return *((uint32_t *)&cpu->memory[cpu->pc]);
}


int compile_c_file(const char *c_file) {
    printf("Compiling %s...\n", c_file);
    char command[256];
    snprintf(command, sizeof(command), "gcc -o a.out %s", c_file);
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Error: Compilation failed for %s\n", c_file);
        return -1;
    }
    printf("Compilation successful. Binary: a.out\n");
    return 0;
}


// Run the CPU (fetch-decode-execute loop)

void run_cpu(CPU *cpu) {
    printf("Initial CPU state:\n");
    display_memory_segments(cpu); // Display initial memory layout

    while (!cpu->halted) {
        printf("\nExecuting instruction at PC: %08X\n", cpu->pc);
        uint32_t old_pc = cpu->pc; // Save PC before execution
        uint32_t raw_instruction = fetch_instruction(cpu);
        Instruction instruction = decode_instruction(raw_instruction);

        // Execute instruction
        execute_instruction(cpu, instruction);

        // Display memory and register changes
        printf("\nUpdated Memory Segments:\n");
        display_memory_segments(cpu);
        printf("\nUpdated Registers:\n");
        display_registers(cpu);

        if (!cpu->halted) {
            // Only advance PC if it wasn't changed by a jump instruction
            if (cpu->pc == old_pc) {
                cpu->pc += sizeof(uint32_t);
            }
        }
    }

    printf("\nCPU halted at PC: %08X\n", cpu->pc);
    display_memory_segments(cpu);
    printf("\nFinal CPU state:\n");
    display_registers(cpu);
}




void display_memory_dump(uint8_t *memory, size_t size) {
    printf("Address     Hexadecimal Values                        ASCII\n");
    printf("---------------------------------------------------------------\n");
    for (size_t i = 0; i < size; i += 16) {
        printf("%08zX  ", i);
        for (size_t j = 0; j < 16; ++j) {
            if (i + j < size)
                printf("%02X ", memory[i + j]);
            else
                printf("   ");
        }
        printf(" ");
        for (size_t j = 0; j < 16; ++j) {
            if (i + j < size && isprint(memory[i + j]))
                printf("%c", memory[i + j]);
            else
                printf(".");
        }
        printf("\n");
    }
}




void display_memory_segments(const CPU *cpu) {
    printf("\nMemory Layout:\n");

    // Code Segment
    printf("Code Segment (Instructions):\n");
    for (uint32_t addr = CODE_START; addr < CODE_END; addr += 16) {
        printf("0x%08X: ", addr);
        for (int i = 0; i < 16 && addr + i < CODE_END; i++) {
            printf("%02X ", cpu->memory[addr + i]);
        }
        printf("\n");
    }

    // Data Segment
    printf("\nData Segment (Global/Static Variables):\n");
    for (uint32_t addr = DATA_START; addr < DATA_END; addr += 16) {
        printf("0x%08X: ", addr);
        for (int i = 0; i < 16 && addr + i < DATA_END; i++) {
            printf("%02X ", cpu->memory[addr + i]);
        }
        printf("\n");
    }

    // Stack
    printf("\nStack Segment:\n");
    for (uint32_t addr = STACK_START; addr < cpu->sp; addr += 16) {
        printf("0x%08X: ", addr);
        for (int i = 0; i < 16 && addr + i < STACK_END; i++) {
            printf("%02X ", cpu->memory[addr + i]);
        }
        printf("\n");
    }

    // Heap
    printf("\nHeap Segment:\n");
    for (uint32_t addr = HEAP_START; addr < cpu->heap_pointer; addr += 16) {
        printf("0x%08X: ", addr);
        for (int i = 0; i < 16 && addr + i < HEAP_END; i++) {
            printf("%02X ", cpu->memory[addr + i]);
        }
        printf("\n");
    }
}



void display_stack(CPU *cpu) {
    printf("Stack (SP: %08X):\n", cpu->sp);
    for (uint32_t addr = STACK_END; addr >= cpu->sp; addr -= 4) {
        printf("  %08X: %08X\n", addr, read_memory(cpu->memory, addr));
    }
}

void log_function_call(const char *func_name, int call_depth, uint32_t *params, int param_count) {
    printf("Function Call: %s (Depth: %d)\n", func_name, call_depth);
    printf("Parameters:\n");
    for (int i = 0; i < param_count; ++i) {
        printf("  Param %d: %08X\n", i + 1, params[i]);
    }
}






