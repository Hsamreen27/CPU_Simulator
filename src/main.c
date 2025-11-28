#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"
#include "hll_translator.h"
#include "linker.h"
#include "debug.h"


int compile_and_execute_c_file(const char *c_file) {
    char command[256];
    snprintf(command, sizeof(command), "gcc %s -o temp_executable", c_file);
    if (system(command) != 0) {
        fprintf(stderr, "Error: Compilation of '%s' failed.\n", c_file);
        return -1;
    }

    printf("Running compiled program...\n");
    if (system("./temp_executable") != 0) {
        fprintf(stderr, "Error: Execution failed.\n");
        return -1;
    }

    return 0;
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <command> <input_file> [output_file]\n", argv[0]);
        fprintf(stderr, "Commands:\n");
        fprintf(stderr, "  translate <input.hll> <output.asm>   Translate HLL to assembly\n");
        fprintf(stderr, "  assemble <input.asm> <output.bin>   Assemble assembly to binary\n");
        fprintf(stderr, "  run <input.bin>                     Run binary file\n");
        fprintf(stderr, "  compile <input.c>                   Compile C program and run\n");
        return 1;
    }

    const char *command = argv[1];
    const char *input_file = argv[2];
    const char *output_file = (argc >= 4) ? argv[3] : NULL;

    CPU cpu;

    if (strcmp(command, "translate") == 0) {
        // Translate HLL to Assembly
        if (!output_file) {
            fprintf(stderr, "Usage: %s translate <input.hll> <output.asm>\n", argv[0]);
            return 1;
        }

        FILE *hll_fp = fopen(input_file, "r");
        if (!hll_fp) {
            fprintf(stderr, "Error: Cannot open HLL file '%s'.\n", input_file);
            return 1;
        }

        fseek(hll_fp, 0, SEEK_END);
        long length = ftell(hll_fp);
        fseek(hll_fp, 0, SEEK_SET);

        char *hll_code = malloc(length + 1);
        fread(hll_code, 1, length, hll_fp);
        fclose(hll_fp);
        hll_code[length] = '\0';

        if (translate_hll_to_assembly(hll_code, output_file) != 0) {
            fprintf(stderr, "Error: Translation from HLL to assembly failed.\n");
            free(hll_code);
            return 1;
        }

        printf("Successfully translated '%s' to '%s'.\n", input_file, output_file);
        free(hll_code);

    } else if (strcmp(command, "assemble") == 0) {
        // Assemble Assembly to Binary
        if (!output_file) {
            fprintf(stderr, "Usage: %s assemble <input.asm> <output.bin>\n", argv[0]);
            return 1;
        }

        if (assemble(input_file, output_file) != 0) {
            fprintf(stderr, "Error: Assembly to binary conversion failed for '%s'.\n", input_file);
            return 1;
        }

        printf("Successfully assembled '%s' to '%s'.\n", input_file, output_file);

    }else if (strcmp(command, "compile") == 0) {
        if (compile_and_execute_c_file(input_file) != 0) {
            return 1;
        }
    } else if (strcmp(command, "run") == 0) {
        // Run Binary File
        init_cpu(&cpu);
        display_memory_segments(&cpu);

        if (load_binary_program(&cpu, input_file) != 0) {
            fprintf(stderr, "Error: Failed to load binary file '%s'.\n", input_file);
            return 1;
        }

        run_cpu(&cpu);

    } else if (strcmp(command, "compile") == 0) {
        // Compile C Program and Run
        char command[256];
        snprintf(command, sizeof(command), "gcc -o a.out %s", input_file);

        printf("Compiling '%s'...\n", input_file);
        if (system(command) != 0) {
            fprintf(stderr, "Error: Compilation of '%s' failed.\n", input_file);
            return 1;
        }

        printf("Running a.out...\n");
        if (system("./a.out") != 0) {
            fprintf(stderr, "Error: Execution of 'a.out' failed.\n");
            return 1;
        }
    } else if (strcmp(command, "run_c") == 0) {
        // Run C Program
        if (argc < 3) {
            fprintf(stderr, "Usage: %s run_c <input.c>\n", argv[0]);
            return 1;
        }

        const char *c_file = input_file; // The C source file

        // Compile the C program to an executable binary
        printf("Compiling '%s'...\n", c_file);
        if (compile_c_file(c_file) != 0) {
            fprintf(stderr, "Error: Compilation failed for '%s'.\n", c_file);
            return 1;
        }

        printf("Simulating '%s'...\n", c_file);

        // Assume the binary has been compiled and loaded
        init_cpu(&cpu);                      // Initialize the CPU
        display_memory_segments(&cpu);       // Show initial memory layout

        // Simulate the execution of the C program
        if (load_binary_program(&cpu, "a.out") != 0) {
            fprintf(stderr, "Error: Failed to load compiled binary 'a.out'.\n");
            return 1;
        }

        run_cpu(&cpu);                       // Execute the CPU
    }
    else {
            fprintf(stderr, "Error: Unknown command '%s'.\n", command);
            return 1;
    }

    return 0;
}
