#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char obj_t;

typedef union{
  size_t size_v;
  unsigned long size_ul;
  char* type_str;
  /*
  char type_str3[4];
  char type_str4[5];
  char type_str5[6];
  char type_str6[7];
  char type_str7[8];
  char type_str8[9];
  */
} template_type_arg_t;

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
  BOOL__,
  PTR__
} pod_type_t; 


pod_type_t pod_type(template_type_arg_t type);

typedef union{
  char c;
  short s;
  bool b;
  int i;
  long l;
  float f;
  double d;
  int8_t i8;
  uint8_t ui8;
  int16_t i16;
  uint16_t ui16;
  int32_t i32;
  uint32_t ui32;
  int64_t i64;
  uint64_t ui64;
  obj_t o;
  obj_t* optr;
  void* v;
} template_data_t;


