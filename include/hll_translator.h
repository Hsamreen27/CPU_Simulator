#ifndef HLL_TRANSLATOR_H
#define HLL_TRANSLATOR_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
/**
 * Translate a high-level language (HLL) program to assembly code.
 * @param hll_code The input HLL program as a string.
 * @param output_file The file to write the translated assembly code.
 * @return 0 on success, -1 on failure.
 */
int translate_hll_to_assembly(const char *hll_code, const char *output_file);

#endif // HLL_TRANSLATOR_H
