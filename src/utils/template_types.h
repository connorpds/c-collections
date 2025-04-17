#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "string_packed_ints.h"


////////////  TEMPLATE ARG TYPE + PARAMETERS //////////////// 
typedef packed_int_t template_arg_t; 
typedef packed_int_t wide_pod_t;
typedef uintptr_t ptr_pod_t;


/*  Create Compressed String Format: convert chars to 6 bits each - 10 / int64.
 *  21 / int128
 *  Last 4 bits can be used to encode info.
 *  Create an api for packing/reading strings in that format.
 */



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
size_t pod_size(pod_type_t type);


///////////// Collection Object Types /////////////////
typedef char obj_t;


wide_pod_t create_typemask(size_t type_size);

