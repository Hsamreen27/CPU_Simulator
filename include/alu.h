#ifndef ALU_H
#define ALU_H

#include <stdint.h>
#include "cpu.h" // Include CPU structure for flag updates

// ALU Function Prototypes

/**
 * Performs addition and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return Result of addition.
 */
int32_t alu_add(CPU *cpu, int32_t a, int32_t b);

/**
 * Performs subtraction and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return Result of subtraction.
 */
int32_t alu_sub(CPU *cpu, int32_t a, int32_t b);

/**
 * Performs multiplication and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return Result of multiplication.
 */
int32_t alu_mul(CPU *cpu, int32_t a, int32_t b);

/**
 * Performs division and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - Dividend.
 * @param b - Divisor.
 * @return Result of division.
 */
int32_t alu_div(CPU *cpu, int32_t a, int32_t b);

/**
 * Performs bitwise AND operation and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return Result of bitwise AND.
 */
int32_t alu_and(CPU *cpu, int32_t a, int32_t b);

/**
 * Performs bitwise OR operation and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return Result of bitwise OR.
 */
int32_t alu_or(CPU *cpu, int32_t a, int32_t b);

/**
 * Performs bitwise XOR operation and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return Result of bitwise XOR.
 */
int32_t alu_xor(CPU *cpu, int32_t a, int32_t b);

/**
 * Performs bitwise NOT operation and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - Operand.
 * @return Result of bitwise NOT.
 */
int32_t alu_not(CPU *cpu, int32_t a);

/**
 * Performs left shift operation and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - Operand.
 * @param shift - Number of positions to shift.
 * @return Result of left shift.
 */
int32_t alu_shl(CPU *cpu, int32_t a, int32_t shift);

/**
 * Performs right shift operation and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - Operand.
 * @param shift - Number of positions to shift.
 * @return Result of right shift.
 */
int32_t alu_shr(CPU *cpu, int32_t a, int32_t shift);

/**
 * Compares two values for equality and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return 1 if equal, 0 otherwise.
 */
int32_t alu_eq(CPU *cpu, int32_t a, int32_t b);

/**
 * Compares two values for inequality and updates the CPU flags.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return 1 if not equal, 0 otherwise.
 */
int32_t alu_neq(CPU *cpu, int32_t a, int32_t b);

/**
 * Checks if the first operand is greater than the second.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return 1 if a > b, 0 otherwise.
 */
int32_t alu_gt(CPU *cpu, int32_t a, int32_t b);

/**
 * Checks if the first operand is less than the second.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return 1 if a < b, 0 otherwise.
 */
int32_t alu_lt(CPU *cpu, int32_t a, int32_t b);

/**
 * Checks if the first operand is greater than or equal to the second.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return 1 if a >= b, 0 otherwise.
 */
int32_t alu_ge(CPU *cpu, int32_t a, int32_t b);

/**
 * Checks if the first operand is less than or equal to the second.
 * @param cpu - Pointer to the CPU structure.
 * @param a - First operand.
 * @param b - Second operand.
 * @return 1 if a <= b, 0 otherwise.
 */
int32_t alu_le(CPU *cpu, int32_t a, int32_t b);

#endif // ALU_H
