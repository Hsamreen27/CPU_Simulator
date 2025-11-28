; test_add.asm - Test ADD instruction
; Should print: R0=3, R1=5, R2=8 (3+5)

LOAD 0, 3
LOAD 1, 5
ADD 2, 0, 1
OUT 0
OUT 1
OUT 2
HALT
