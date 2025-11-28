#include "linker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Opcode to binary mapping
uint8_t get_opcode_binary(const char *opcode) {
    if (strcmp(opcode, "ADD") == 0) return 0x00;
    if (strcmp(opcode, "SUB") == 0) return 0x01;
    if (strcmp(opcode, "MUL") == 0) return 0x02;
    if (strcmp(opcode, "DIV") == 0) return 0x03;
    if (strcmp(opcode, "AND") == 0) return 0x04;
    if (strcmp(opcode, "OR") == 0) return 0x05;
    if (strcmp(opcode, "XOR") == 0) return 0x06;
    if (strcmp(opcode, "NOT") == 0) return 0x07;
    if (strcmp(opcode, "SHL") == 0) return 0x08;
    if (strcmp(opcode, "SHR") == 0) return 0x09;
    if (strcmp(opcode, "EQ") == 0) return 0x0A;
    if (strcmp(opcode, "NEQ") == 0) return 0x0B;
    if (strcmp(opcode, "GT") == 0) return 0x0C;
    if (strcmp(opcode, "LT") == 0) return 0x0D;
    if (strcmp(opcode, "GE") == 0) return 0x0E;
    if (strcmp(opcode, "LE") == 0) return 0x0F;
    if (strcmp(opcode, "LOAD") == 0) return 0x10;
    if (strcmp(opcode, "STORE") == 0) return 0x11;
    if (strcmp(opcode, "JUMP") == 0) return 0x12;
    if (strcmp(opcode, "JZ") == 0) return 0x13;
    if (strcmp(opcode, "JNZ") == 0) return 0x14;
    if (strcmp(opcode, "CALL") == 0) return 0x15;
    if (strcmp(opcode, "RET") == 0) return 0x16;
    if (strcmp(opcode, "PUSH") == 0) return 0x17;
    if (strcmp(opcode, "POP") == 0) return 0x18;
    if (strcmp(opcode, "HALT") == 0) return 0x19;
    if (strcmp(opcode, "OUT") == 0) return 0x1A;

    fprintf(stderr, "Error: Unknown opcode '%s'.\n", opcode);
    exit(EXIT_FAILURE);
}

// Label handling
typedef struct {
    char label[32];
    uint32_t address;
} LabelEntry;

#define MAX_LABELS 100
LabelEntry label_table[MAX_LABELS];
int label_count = 0;

void add_label(const char *label, uint32_t address) {
    if (label_count >= MAX_LABELS) {
        fprintf(stderr, "Error: Too many labels.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(label_table[label_count].label, label);
    label_table[label_count].address = address;
    label_count++;
}

uint32_t resolve_label(const char *label) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(label_table[i].label, label) == 0) {
            return label_table[i].address;
        }
    }
    fprintf(stderr, "Error: Undefined label '%s'.\n", label);
    exit(EXIT_FAILURE);
}

static void trim_whitespace(char *s) {
    if (!s) return;
    // Trim leading spaces
    while (*s && isspace((unsigned char)*s)) {
        memmove(s, s + 1, strlen(s));
    }
    // Trim trailing spaces
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[--len] = '\0';
    }
}

// Translate a single assembly line to binary

uint32_t translate_assembly_line_to_binary(const char *line) {
    char opcode[16] = "";
    char operand1[32] = "", operand2[32] = "", operand3[32] = "";
    int operands[3] = {0}; // Holds numeric values of operands
    int operand_count = 0; // Number of operands for the instruction
    uint32_t binary_instruction = 0;

    // Debug print
    printf("Processing line: %s\n", line);

    // Remove inline comments (everything after ';')
    char clean_line[256];
    strncpy(clean_line, line, sizeof(clean_line));
    char *semicolon_pos = strchr(clean_line, ';');
    if (semicolon_pos) {
        *semicolon_pos = '\0'; // Truncate the line before the comment
    }

    // Parse the instruction into opcode and operands directly from the cleaned line.
    // NOTE: Do NOT use strtok with space as a delimiter here, because that would
    // split off the opcode and discard the rest of the operands before sscanf runs.
    // Instead, let sscanf handle splitting on spaces and commas.
    if (sscanf(clean_line, "%15s %31[^,], %31[^,], %31s", opcode, operand1, operand2, operand3) < 1) {
        fprintf(stderr, "Error: Failed to parse instruction line: '%s'\n", line);
        exit(EXIT_FAILURE);
    }

    // Match opcode to its binary value and determine expected operand count
    if (strcmp(opcode, "ADD") == 0) {
        binary_instruction |= 0x00 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "SUB") == 0) {
        binary_instruction |= 0x01 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "MUL") == 0) {
        binary_instruction |= 0x02 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "DIV") == 0) {
        binary_instruction |= 0x03 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "AND") == 0) {
        binary_instruction |= 0x04 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "OR") == 0) {
        binary_instruction |= 0x05 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "XOR") == 0) {
        binary_instruction |= 0x06 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "NOT") == 0) {
        binary_instruction |= 0x07 << 24;
        operand_count = 2;
    } else if (strcmp(opcode, "SHL") == 0) {
        binary_instruction |= 0x08 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "SHR") == 0) {
        binary_instruction |= 0x09 << 24;
        operand_count = 3;
    } else if (strcmp(opcode, "LOAD") == 0) {
        binary_instruction |= 0x10 << 24;
        operand_count = 2;
    } else if (strcmp(opcode, "STORE") == 0) {
        binary_instruction |= 0x11 << 24;
        operand_count = 2;
    } else if (strcmp(opcode, "JUMP") == 0 || strcmp(opcode, "JZ") == 0 || strcmp(opcode, "JNZ") == 0) {
        binary_instruction |= get_opcode_binary(opcode) << 24;
        operand_count = 1;
    } else if (strcmp(opcode, "RET") == 0) {
        binary_instruction |= 0x16 << 24;
        operand_count = 0;
    } else if (strcmp(opcode, "PUSH") == 0 || strcmp(opcode, "POP") == 0) {
        binary_instruction |= get_opcode_binary(opcode) << 24;
        operand_count = 1;
    } else if (strcmp(opcode, "HALT") == 0) {
        binary_instruction |= 0x19 << 24;
        operand_count = 0;
    } else if (strcmp(opcode, "OUT") == 0) {
        binary_instruction |= 0x1A << 24;
        operand_count = 1;
    } else {
        fprintf(stderr, "Error: Unknown opcode '%s'.\n", opcode);
        exit(EXIT_FAILURE);
    }

    // Parse and encode operands (simple encoding without explicit mode bits)
    // Addressing modes are implicit based on instruction semantics
    if (operand_count > 0) {
        trim_whitespace(operand1);
        if (isalpha((unsigned char)operand1[0])) {
            operands[0] = resolve_label(operand1);
        } else {
            sscanf(operand1, "%d", &operands[0]);
        }
        binary_instruction |= (operands[0] & 0xFF) << 16;  // Operand 0 (8 bits)
    }

    if (operand_count > 1) {
        trim_whitespace(operand2);
        if (isalpha((unsigned char)operand2[0])) {
            operands[1] = resolve_label(operand2);
        } else {
            sscanf(operand2, "%d", &operands[1]);
        }
        binary_instruction |= (operands[1] & 0xFF) << 8;    // Operand 1 (8 bits)
    }

    if (operand_count > 2) {
        trim_whitespace(operand3);
        if (isalpha((unsigned char)operand3[0])) {
            operands[2] = resolve_label(operand3);
        } else {
            sscanf(operand3, "%d", &operands[2]);
        }
        binary_instruction |= (operands[2] & 0xFF);          // Operand 2 (8 bits)
    }

    // Debug print the parsed result
    printf("Opcode: %s, Operand1: %s, Operand2: %s, Operand3: %s\n",
           opcode, operand1, operand2, operand3);
    printf("Binary Instruction: %08X\n", binary_instruction);

    return binary_instruction;
}



// Main assembler function
int assemble(const char *asm_file, const char *bin_file) {
    FILE *input = fopen(asm_file, "r");
    FILE *output = fopen(bin_file, "wb");

    if (!input || !output) {
        fprintf(stderr, "Error: Cannot open file(s).\n");
        return -1;
    }

    char line[256];
    uint32_t address = 0; // Current memory address (instruction index)

    // First Pass: Build the label table
    while (fgets(line, sizeof(line), input)) {
        char *trimmed_line = strtok(line, "\n\r\t "); // Trim whitespace and newlines

        if (!trimmed_line || trimmed_line[0] == ';') {
            continue; // Skip empty lines and comments
        }

        // Check if the line ends with ':' (it's a label)
        if (strchr(trimmed_line, ':')) {
            trimmed_line[strlen(trimmed_line) - 1] = '\0'; // Remove trailing ':'
            add_label(trimmed_line, address);
        } else {
            address += sizeof(uint32_t); // Increment address for each instruction
        }
    }

    rewind(input); // Reset file pointer for second pass

    // Second Pass: Translate instructions into binary
    while (fgets(line, sizeof(line), input)) {
        char opcode[16];
        sscanf(line, "%s", opcode);

        if (opcode[0] == ';' || strchr(line, ':')) {
            continue; // Skip comments and label definitions in second pass
        }

        uint32_t binary_instruction = translate_assembly_line_to_binary(line);
        fwrite(&binary_instruction, sizeof(binary_instruction), 1, output);
    }

    fclose(input);
    fclose(output);

    printf("Assembly complete: %s\n", bin_file);
    return 0;
}
