; factorial.asm - Recursive factorial calculation
; Calculates factorial of 5 (5! = 120)
;
; This program demonstrates:
; - Function calls using CALL/RET
; - Recursion (function calling itself)
; - Stack usage for saving return addresses and parameters
; - Base case handling (factorial(1) = 1)
;
; Memory Layout:
; R0 = parameter n (input to factorial function)
; R1 = return value (result of factorial)
; R2 = temporary for comparisons
; R3 = temporary for arithmetic
; Stack = stores return addresses and saved values
;
; Algorithm:
;   factorial(n):
;     if n <= 1: return 1
;     else: return n * factorial(n-1)
;
; Expected output: 120 (which is 5! = 5*4*3*2*1)
LOAD 0, 5
CALL FACTORIAL
OUT 1
HALT
FACTORIAL:
LOAD 2, 1
SUB 2, 0, 2
JZ BASE_CASE
PUSH 0
LOAD 2, 1
SUB 0, 0, 2
CALL FACTORIAL
POP 0
MUL 1, 0, 1
RET
BASE_CASE:
LOAD 1, 1
RET
