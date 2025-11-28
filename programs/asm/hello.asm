; hello.asm - Hello, World example (numeric output)
; Outputs ASCII codes for "Hello, World" using register 0.
; Expected output: 72, 101, 108, 108, 111, 44, 32, 87, 111, 114, 108, 100
        LOAD 0, 72         ; 'H'
        OUT 0
        LOAD 0, 101        ; 'e'
        OUT 0
        LOAD 0, 108        ; 'l'
        OUT 0
        LOAD 0, 108        ; 'l'
        OUT 0
        LOAD 0, 111        ; 'o'
        OUT 0
        LOAD 0, 44         ; ',' (comma)
        OUT 0
        LOAD 0, 32         ; ' ' (space)
        OUT 0
        LOAD 0, 87         ; 'W'
        OUT 0
        LOAD 0, 111        ; 'o'
        OUT 0
        LOAD 0, 114        ; 'r'
        OUT 0
        LOAD 0, 108        ; 'l'
        OUT 0
        LOAD 0, 100        ; 'd'
        OUT 0
        HALT
