/*
 * factorial.c - Recursive factorial calculation
 * 
 * This program demonstrates recursion by calculating factorial.
 * Factorial of n (n!) = n * (n-1) * (n-2) * ... * 1
 * 
 * Example: 5! = 5 * 4 * 3 * 2 * 1 = 120
 */

#include <stdio.h>

/**
 * factorial - Recursively calculates factorial of n
 * @n: The number to calculate factorial for
 * 
 * Base case: factorial(0) = 1, factorial(1) = 1
 * Recursive case: factorial(n) = n * factorial(n-1)
 * 
 * Return: The factorial of n
 */
int factorial(int n) {
    // Base case: 0! = 1 and 1! = 1
    if (n <= 1) {
        printf("  Base case reached: factorial(%d) = 1\n", n);
        return 1;
    }
    
    // Recursive case: n! = n * (n-1)!
    printf("  Computing: factorial(%d) = %d * factorial(%d)\n", n, n, n-1);
    int result = n * factorial(n - 1);
    printf("  Returning: factorial(%d) = %d\n", n, result);
    
    return result;
}

/**
 * main - Driver program for factorial calculation
 * 
 * Demonstrates function calls and recursion by calculating
 * factorial of 5.
 * 
 * Return: 0 on success
 */
int main() {
    int num = 5;
    
    printf("=== Factorial Calculation Demo ===\n");
    printf("Calculating factorial of %d\n\n", num);
    
    int result = factorial(num);
    
    printf("\nFinal Result: %d! = %d\n", num, result);
    printf("Expected: 5! = 5 * 4 * 3 * 2 * 1 = 120\n");
    
    return 0;
}
