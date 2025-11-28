; timer.asm - Timer/Counter example
; Demonstrates Fetch-Decode-Execute cycles with a simple counter
; Expected output: 0, 1, 2, 3, 4, 5
LOAD 0, 0
LOAD 1, 6
LOAD 2, 1
LOOP:
OUT 0
ADD 0, 0, 2
SUB 1, 1, 2
JNZ LOOP
HALT
