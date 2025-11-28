; test_sub.asm - Test SUB instruction and Zero flag
; Should print: R0=5, R1=3, R2=2 (5-3), R3=0 (3-3)

LOAD 0, 5
LOAD 1, 3
SUB 2, 0, 1
OUT 2
SUB 3, 1, 1
OUT 3
HALT
