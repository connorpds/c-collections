#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>


typedef __uint128_t packed_int_t; 
typedef uintptr_t packed_ptr_t;
#define COMPRESSED_CHAR_BITWIDTH 6
#define CHARS_PER_ARG sizeof(packed_int_t) * 8 / COMPRESSED_CHAR_BITWIDTH
typedef char compressed_char_t;

compressed_char_t compress_char(char c);
char translate_compressed_char(compressed_char_t cc);
char decompress_char(packed_int_t cmp_str, int idx);
void mark_packed(packed_int_t* just_packed);
void mark_not_packed(packed_int_t* not_packed);

bool is_packed(packed_int_t val);
packed_int_t string_packed_int(const char* str);

void unpack_string(packed_int_t packed, char unpacked[CHARS_PER_ARG]);
void print_compressed_string(packed_int_t packed);
void test_print_compressed_string(char* test_str);



