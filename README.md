# CPU Simulator Project

A complete software-based CPU simulator implemented in C, featuring a custom ISA, assembler, and three demonstration programs.

## Project Structure
```
CPU_Simulator/
├── include/           # Header files
│   ├── cpu.h         # CPU structure, registers, flags
│   ├── alu.h         # ALU operations
│   ├── memory.h      # Memory management
│   ├── instructions.h # ISA definition
│   ├── debug.h       # Debug utilities
│   └── linker.h      # Assembler/linker
├── src/              # Source files
│   ├── cpu.c         # Fetch-decode-execute loop
│   ├── alu.c         # Arithmetic/logic operations
│   ├── memory.c      # Memory operations
│   ├── instructions.c # Instruction execution
│   ├── debug.c       # Debug output
│   ├── linker.c      # Two-pass assembler
│   └── main.c        # Entry point
├── programs/
│   ├── asm/          # Assembly source files
│   │   ├── timer.asm    # Timer/counter demo
│   │   ├── hello.asm    # Hello, World
│   │   └── fib.asm      # Fibonacci sequence
│   └── bin/          # Compiled binaries
├── build/            # Compiled simulator
└── Makefile          # Build automation
```

---

## CPU Schematic

Below is the CPU architecture used in this project:

<img width="8278" height="1216" alt="image" src="https://github.com/user-attachments/assets/3bc5fd40-f4e2-40c5-9777-4180d183e76a" />

The required demo video for the Fibonacci program is included in the repository at:
**docs/Fib_Cpu_Simulator.mp4**



### Registers
- **General Purpose**: R0, R1, R2, R3 (32-bit)
- **Special Purpose**: 
  - PC (Program Counter)
  - SP (Stack Pointer)
- **Flags**: Z (Zero), N (Negative), O (Overflow)

### Memory Layout (1024 bytes)
| Segment | Address Range | Purpose |
|---------|---------------|---------|
| Code    | 0x000–0x0FF   | Program instructions |
| Data    | 0x100–0x1FF   | Global/static variables |
| Stack   | 0x200–0x2FF   | Function calls, local vars |
| Heap    | 0x300–0x3FF   | Dynamic allocation |

### Instruction Set Architecture (ISA)

**Instruction Format**: 32-bit (8-bit opcode + 3×8-bit operands)

**Supported Instructions**:
- **Arithmetic**: ADD, SUB, MUL, DIV
- **Logical**: AND, OR, XOR, NOT
- **Shift**: SHL, SHR
- **Comparison**: EQ, NEQ, GT, LT, GE, LE
- **Memory**: LOAD, STORE
- **Control Flow**: JUMP, JZ, JNZ, CALL, RET
- **Stack**: PUSH, POP
- **I/O**: OUT
- **System**: HALT

**Addressing Modes** (implicit based on instruction):
- IMMEDIATE: Direct values (e.g., `LOAD R0, 5`)
- REGISTER: Register-to-register (e.g., `ADD R0, R1, R2`)

---

## Building and Running

### Compilation

**Clone the repository**:

```bash
git clone https://github.com/Hsamreen27/CPU_Simulator
cd CPU_Simulator
make
```

### Usage

**Assemble a program**:
```bash
./build/cpu_simulator assemble programs/asm/<program>.asm programs/bin/<program>.bin
```

**Run a program**:
```bash
./build/cpu_simulator run programs/bin/<program>.bin
```

**Filter output** (recommended to avoid excessive debug output):
```bash
./build/cpu_simulator run programs/bin/<program>.bin | grep "OUT:"
```

---

## Demonstration Programs

### 1. Timer Program (`timer.asm`)
**Purpose**: Demonstrates Fetch-Decode-Execute cycle

**Description**: Counts from 0 to 5, showing how each instruction goes through:
- **FETCH**: Read instruction from memory at PC
- **DECODE**: Interpret opcode and operands
- **EXECUTE**: Perform operation and update state

**Output**: `0, 1, 2, 3, 4, 5`

**Key Concepts**:
- Loop control with counter
- Flag-based conditional branching (JNZ)
- Register arithmetic (ADD, SUB)

### 2. Hello World Program (`hello.asm`)
**Purpose**: Basic I/O demonstration

**Description**: Outputs ASCII codes for "Hello, World"

**Output**: 
```
72 (H), 101 (e), 108 (l), 108 (l), 111 (o), 44 (,), 32 ( ),
87 (W), 111 (o), 114 (r), 108 (l), 100 (d)
```

**Key Concepts**:
- Sequential execution
- LOAD immediate values
- OUT instruction for display

### 3. Fibonacci Sequence (`fib.asm`)
**Purpose**: Complex arithmetic and loop control

**Description**: Computes first 12 Fibonacci numbers (0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89)

**Output**: Fibonacci sequence up to 89 (hex: 0x59)

**Key Concepts**:
- Register-only arithmetic (no MOV instruction)
- Value shifting using ADD/SUB
- Loop termination with zero flag
- Multi-register coordination

---

## Critical Implementation Details

### Bug Fixes Applied

1. **ALU Flag Updates**
   - **Issue**: ADD and SUB were doing direct arithmetic without updating CPU flags
   - **Fix**: Changed to call `alu_add()` and `alu_sub()` which properly set Z, N, O flags
   - **Impact**: Enables JNZ/JZ to work correctly for loop control

2. **Jump Instruction PC Handling**
   - **Issue**: PC was incremented by 4 after every instruction, including jumps
   - **Fix**: Check if PC changed during execution; only increment if unchanged
   - **Impact**: Jump instructions now correctly transfer control

3. **Addressing Mode Simplification**
   - **Issue**: Complex bit-packing of addressing modes caused decoding errors
   - **Fix**: Use instruction semantics (LOAD uses immediate, ADD uses registers)
   - **Impact**: Simplified encoding/decoding, more reliable execution

4. **Label Resolution**
   - **Issue**: Trailing whitespace in operands caused "undefined label" errors
   - **Fix**: Trim whitespace before label lookup
   - **Impact**: Labels resolve correctly in all cases

---

## Fetch-Decode-Execute Cycle

Every instruction follows this three-phase cycle:

1. **FETCH**
   - Read 32-bit instruction from memory at address PC
   - Increment PC by 4 (unless modified by jump)

2. **DECODE**
   - Extract opcode (bits 31-24)
   - Extract operands (bits 23-0, three 8-bit values)
   - Determine addressing modes based on instruction type

3. **EXECUTE**
   - Perform operation (arithmetic, memory access, control flow)
   - Update registers and flags
   - For jumps: modify PC directly

---

## Testing and Verification

All three demonstration programs have been tested and verified:

 **Timer**: Correctly counts 0-5 and halts  
 **Hello, World**: Outputs all 12 ASCII codes correctly  
 **Fibonacci**: Computes sequence up to 89 with proper loop termination

**Test Commands**:
```bash
# Timer
./build/cpu_simulator assemble programs/asm/timer.asm programs/bin/timer.bin
./build/cpu_simulator run programs/bin/timer.bin | grep "OUT:"

# Hello, World
./build/cpu_simulator assemble programs/asm/hello.asm programs/bin/hello.bin
./build/cpu_simulator run programs/bin/hello.bin | grep "OUT:"

# Fibonacci
./build/cpu_simulator assemble programs/asm/fib.asm programs/bin/fib.bin
./build/cpu_simulator run programs/bin/fib.bin | grep "OUT:"
```

---

## Project Status

**Completed**:
-  CPU core with registers, flags, and memory
-  ALU with proper flag updates
-  Full ISA implementation
-  Two-pass assembler with label support
-  Three demonstration programs
-  Fetch-decode-execute cycle
-  Loop control and conditional branching
-  Debug output and tracing

**Architecture Highlights**:
- 4 general-purpose registers
- 1024-byte segmented memory
- 32-bit instruction format
- Flag-based conditional execution
- Register-only arithmetic operations

---
