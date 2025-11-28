#include "alu.h"
#include <stdio.h>

// Helper function to update CPU flags
static void update_flags(CPU *cpu, int32_t result, int overflow) {
    // Clear all flags
    cpu->flags.z = 0;
    cpu->flags.n = 0;
    cpu->flags.o = 0;

    // Update Zero flag
    if (result == 0) {
        cpu->flags.z = 1; // Set Zero flag (Z)
    }

    // Update Negative flag
    if (result < 0) {
        cpu->flags.n = 1; // Set Negative flag (N)
    }

    // Update Overflow flag
    if (overflow) {
        cpu->flags.o = 1; // Set Overflow flag (O)
    }
}


// Arithmetic Operations
int32_t alu_add(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = a + b;

    // Detect signed overflow
    int overflow = ((a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result > 0));

    update_flags(cpu, result, overflow);
    return result;
}

int32_t alu_sub(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = a - b;

    // Detect signed overflow
    int overflow = ((a > 0 && b < 0 && result < 0) || (a < 0 && b > 0 && result > 0));

    update_flags(cpu, result, overflow);
    return result;
}

int32_t alu_mul(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = a * b;

    // Multiplication overflow is not typically handled in simple ALUs
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_div(CPU *cpu, int32_t a, int32_t b) {
    if (b == 0) {
        fprintf(stderr, "Error: Division by zero.\n");
        cpu->halted = true; // Halt the CPU on division by zero
        return 0;
    }
    int32_t result = a / b;
    update_flags(cpu, result, 0);
    return result;
}

// Logical Operations
int32_t alu_and(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = a & b;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_or(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = a | b;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_xor(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = a ^ b;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_not(CPU *cpu, int32_t a) {
    int32_t result = ~a;
    update_flags(cpu, result, 0);
    return result;
}

// Shift Operations
int32_t alu_shl(CPU *cpu, int32_t a, int32_t shift) {
    int32_t result = a << shift;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_shr(CPU *cpu, int32_t a, int32_t shift) {
    int32_t result = (int32_t)((uint32_t)a >> shift); // Perform logical right shift
    update_flags(cpu, result, 0);
    return result;
}

// Comparison Operations
int32_t alu_eq(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = (a == b) ? 1 : 0;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_neq(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = (a != b) ? 1 : 0;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_gt(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = (a > b) ? 1 : 0;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_lt(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = (a < b) ? 1 : 0;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_ge(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = (a >= b) ? 1 : 0;
    update_flags(cpu, result, 0);
    return result;
}

int32_t alu_le(CPU *cpu, int32_t a, int32_t b) {
    int32_t result = (a <= b) ? 1 : 0;
    update_flags(cpu, result, 0);
    return result;
}
