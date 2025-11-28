#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "cpu.h" // Include CPU definition here

// Function Prototypes

/**
 * Reads a 32-bit value from memory.
 * @param memory - Pointer to the memory array.
 * @param address - Address to read from.
 * @return The 32-bit value stored at the specified address.
 */
uint32_t read_memory(const uint8_t *memory, uint32_t address);

/**
 * Writes a 32-bit value to memory.
 * @param memory - Pointer to the memory array.
 * @param address - Address to write to.
 * @param value - The 32-bit value to write.
 */
void write_memory(uint8_t *memory, uint32_t address, uint32_t value);

/**
 * Loads a program (array of 32-bit instructions) into the code segment.
 * @param memory - Pointer to the memory array.
 * @param program - Pointer to the array of instructions.
 * @param size - Number of instructions in the program.
 */
int load_program(uint8_t *memory, const uint32_t *program, uint32_t size);


int load_binary_program(CPU *cpu, const char *file_path);


/**
 * Displays the contents of memory in hexadecimal or ASCII format.
 * @param memory - Pointer to the memory array.
 * @param start - Starting address.
 * @param end - Ending address.
 * @param format - Format for display ('H' for hex, 'A' for ASCII).
 */
void display_memory(const uint8_t *memory, uint32_t start, uint32_t end, char format);

#endif // MEMORY_H
