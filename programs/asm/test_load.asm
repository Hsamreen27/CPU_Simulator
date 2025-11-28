; test_load.asm - Test LOAD and OUT instructions
; Should print: R0=5, R1=10, R2=15

LOAD 0, 5
OUT 0
LOAD 1, 10
OUT 1
LOAD 2, 15
OUT 2
HALT
