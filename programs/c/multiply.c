/*
 * multiply.c - Recursive multiplication using addition
 * 
 * This program demonstrates recursion by implementing multiplication
 * using only addition. It shows how complex operations can be built
 * from simpler ones.
 * 
 * multiply(a, b) = a + a + a + ... (b times)
 * 
 * Example: multiply(3, 4) = 3 + 3 + 3 + 3 = 12
 */

#include <stdio.h>

/**
 * multiply - Recursively multiplies two numbers using addition
 * @a: First number (multiplicand)
 * @b: Second number (multiplier)
 * 
 * Base case: multiply(a, 0) = 0
 *            multiply(a, 1) = a
 * Recursive case: multiply(a, b) = a + multiply(a, b-1)
 * 
 * Return: The product of a and b
 */
int multiply(int a, int b) {
    // Base case: anything multiplied by 0 is 0
    if (b == 0) {
        printf("  Base case: multiply(%d, 0) = 0\n", a);
        return 0;
    }
    
    // Base case: anything multiplied by 1 is itself
    if (b == 1) {
        printf("  Base case: multiply(%d, 1) = %d\n", a, a);
        return a;
    }
    
    // Recursive case: a * b = a + (a * (b-1))
    printf("  Computing: multiply(%d, %d) = %d + multiply(%d, %d)\n", 
           a, b, a, a, b-1);
    int result = a + multiply(a, b - 1);
    printf("  Returning: multiply(%d, %d) = %d\n", a, b, result);
    
    return result;
}

/**
 * main - Driver program for multiplication calculation
 * 
 * Demonstrates function calls and recursion by multiplying
 * two numbers using only addition.
 * 
 * Return: 0 on success
 */
int main() {
    int num1 = 6;
    int num2 = 4;
    
    printf("=== Recursive Multiplication Demo ===\n");
    printf("Calculating %d * %d using recursion\n\n", num1, num2);
    
    int result = multiply(num1, num2);
    
    printf("\nFinal Result: %d * %d = %d\n", num1, num2, result);
    printf("Expected: 6 * 4 = 24\n");
    
    return 0;
}
