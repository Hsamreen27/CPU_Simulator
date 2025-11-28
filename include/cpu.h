#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define NUM_REGISTERS 4 // Update this value to match your design


// Define memory size (0x000 - 0x3FF => 1024 bytes)
#define MEMORY_SIZE 1024

// Define memory segment boundaries within the 0x000 - 0x3FF range
// Code:   0x000 - 0x0FF
// Data:   0x100 - 0x1FF
// Stack:  0x200 - 0x2FF
// Heap:   0x300 - 0x3FF
#define CODE_START 0x000
#define CODE_END   0x100
#define DATA_START 0x100
#define DATA_END   0x200
#define STACK_START 0x200
#define STACK_END   0x300
#define HEAP_START  0x300
#define HEAP_END    0x400



// Define CPU structure
typedef struct {
    uint8_t z; // Zero flag
    uint8_t n; // Negative flag
    uint8_t o; // Overflow flag
} Flags;

typedef struct {
    uint32_t registers[NUM_REGISTERS];   // General-purpose registers
    uint8_t memory[MEMORY_SIZE]; // Memory
    uint32_t pc;             // Program counter
    uint32_t sp;             // Stack pointer
    uint32_t heap_pointer;       // Heap pointer
    Flags flags;             // CPU flags
    bool halted;             // Halted state
} CPU;

// Example global variables
extern int call_depth;
extern uint32_t params[10];
extern int param_count;




// Function prototypes

/**
 * Initializes the CPU structure.
 * - Clears all registers.
 * - Sets PC to the start of the code segment.
 * - Sets SP to the top of the stack segment.
 * - Sets heap_pointer to the start of the heap.
 */
void init_cpu(CPU *cpu);

/**
 * Resets the CPU state.
 * - Clears all registers.
 * - Resets PC and SP to their initial values.
 * - Clears the memory array.
 */
void reset_cpu(CPU *cpu);

/**
 * Displays the current state of the CPU.
 * - Prints registers, flags, and PC.
 */
// void display_cpu_state(const CPU *cpu);

/**
 * Executes the fetch-decode-execute loop.
 * - Fetches instructions from memory.
 * - Decodes and executes them.
 * - Handles HALT instructions gracefully.
 */
void run_cpu(CPU *cpu);


int compile_c_file(const char *c_file);

void display_memory_dump(uint8_t *memory, size_t size);
void display_stack(CPU *cpu);
void log_function_call(const char *func_name, int call_depth, uint32_t *params, int param_count);



#endif // CPU_H
