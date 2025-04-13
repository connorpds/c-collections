#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>



////////////  TEMPLATE ARG TYPE + PARAMETERS //////////////// 
typedef __uint128_t template_arg_t; 
#define COMPRESSED_CHAR_BITWIDTH 6
#define CHARS_PER_ARG sizeof(template_arg_t) / COMPRESSED_CHAR_BITWIDTH
typedef char compressed_char_t;


/*  Create Compressed String Format: convert chars to 6 bits each - 10 / int64.
 *  21 / int128
 *  Last 4 bits can be used to encode info.
 *  Create an api for packing/reading strings in that format.
 */


compressed_char_t compress_char(char c);
char translate_compressed_char(compressed_char_t cc);
char decompress_char(template_arg_t cmp_str, int idx);
void mark_packed(template_arg_t* just_packed);
template_arg_t pack_templ_arg_str(char* type_str);
void unpack_templ_arg_str(template_arg_t packed, char unpacked[CHARS_PER_ARG]);
void print_compressed_str(template_arg_t packed);

template_arg_t template_arg(char* type_str);


///////////   POD Type Specificiers ///////////// 
typedef enum{
  NOT_POD,
  CHAR__,
  SHORT__,
  INT__,
  LONG__,
  FLOAT__,
  DOUBLE__,
  INT8__,
  INT16__,
  INT32__,
  INT64__,
  UINT8__,
  UINT16__,
  UINT32__,
  UINT64__,
  BOOL__,
  PTR__
} pod_type_t; 

pod_type_t pod_type(template_arg_t type);


///////////// Collection Object Types /////////////////
typedef char obj_t;



