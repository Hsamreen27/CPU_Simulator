; test_loop.asm - Test JNZ with a simple countdown loop
; Should print: R0=3, R0=2, R0=1, then halt

LOAD 0, 3
LOOP:
OUT 0
LOAD 1, 1
SUB 0, 0, 1
JNZ LOOP
HALT
