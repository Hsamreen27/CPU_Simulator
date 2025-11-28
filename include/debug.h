#ifndef DEBUG_H
#define DEBUG_H

#include "cpu.h"
#include "instructions.h" // Include for the Instruction type
// Function Prototypes

/**
 * Displays the contents of all registers.
 * @param cpu - Pointer to the CPU structure.
 */
void display_registers(const CPU *cpu);

/**
 * Displays the current state of memory in a specified range.
 * @param cpu - Pointer to the CPU structure.
 * @param start - Starting memory address.
 * @param end - Ending memory address.
 * @param format - Format for display ('H' for hex, 'A' for ASCII).
 */
void display_memory_state(const CPU *cpu, uint32_t start, uint32_t end, char format);

/**
 * Displays the current state of the CPU.
 * @param cpu - Pointer to the CPU structure.
 */
void display_cpu_state(const CPU *cpu);

/**
 * Displays the decoded instruction for debugging purposes.
 * @param instruction - Instruction to display.
 */
void display_instruction_debug(const Instruction *instruction);




void display_memory_segments(const CPU *cpu);


#endif // DEBUG_H
