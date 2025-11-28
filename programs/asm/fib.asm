; fib.asm - Fibonacci sequence
; Prints: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 (first 12 Fibonacci numbers)
;
; Strategy: Use subtraction to shift values
; After computing next = a + b:
;   new_a = next - a = b  (since next = a+b)
;   new_b = next
;
; But we need to save old_a before overwriting it!
; Solution: compute new_a first, then new_b

        LOAD 0, 0          ; R0 = 0 (a)
        LOAD 1, 1          ; R1 = 1 (b)
        LOAD 3, 10         ; R3 = 10 (counter - will iterate 10 times)
        OUT 0              ; Print 0
        OUT 1              ; Print 1
LOOP:
        ADD 2, 0, 1        ; R2 = a + b (next)
        OUT 2              ; Print next
        ; Shift: a=b, b=next
        ; Since next=a+b, we have: new_a = next-a = b
        SUB 0, 2, 0        ; R0 = R2 - R0 = next - old_a = b (a = b now)
        ; Now R0 = b (new a), R1 = b (old b), R2 = next
        ; We want R1 = next, but R1 still has old b
        ; new_b = next = old_a + old_b, but old_a is gone
        ; Actually: new_b = new_a + old_b = b + old_b... no
        ; Let's use: new_b = R2 (we already have next in R2!)
        ; But to "move" R2 to R1, we need: R1 = R2 - 0 or R1 = R2 + 0
        ; But 0 is not a register! We need a register with value 0.
        ; Use R3 after decrementing? No, R3 starts at 3.
        ; Create a zero register first!
        SUB 1, 1, 1        ; R1 = 0 (clear R1)
        ADD 1, 2, 1        ; R1 = R2 + 0 = R2 (b = next)
        ; Decrement counter
        SUB 2, 2, 2        ; R2 = 0 (reuse R2 as temp zero)
        LOAD 2, 1          ; R2 = 1 (constant)
        SUB 3, 3, 2        ; R3 = R3 - 1
        JNZ LOOP
        HALT
