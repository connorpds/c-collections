#include "template_types.h"


template_arg_t template_arg(char* type_str){
  /* 0? Check to see if the string happens to be a sizeof of a pod_type or 
   *    just a number. If so, encode the appropriate size_t.  <<<MAYBE LATER>>>
   * 1. Convert the string to an array of 21 letters in the 5 bit format by
   *    shifting along the int. 
   * 2. Mark bit 63 to indicate if this is one of the string arguments 
   *    (I do not expect exabyte object sizes, tbh)
   */
  template_arg_t packed = string_packed_int(type_str);

  return packed;
}


pod_type_t pod_type(template_arg_t type){
  pod_type_t outtype = NOT_POD;
  char type_str[CHARS_PER_ARG];
  unpack_string(type, type_str);

  outtype = 
    strcmp(type_str, "char")      ? CHAR__    :
    strcmp(type_str, "float")     ? FLOAT__   :
    strcmp(type_str, "double")    ? DOUBLE__  :
    strcmp(type_str, "bool")      ? BOOL__    :
    strcmp(type_str, "int")       ? INT32__   :
    strcmp(type_str, "short")     ? INT16__   :
    strcmp(type_str, "uint8_t")   ? UINT8__   :
    strcmp(type_str, "int8_t")    ? INT8__    : 
    strcmp(type_str, "uint16_t")  ? UINT16__  :   
    strcmp(type_str, "int16_t")   ? INT16__   :
    strcmp(type_str, "uint32_t")  ? UINT32__  :
    strcmp(type_str, "int32_t")   ? INT32__   :
    strcmp(type_str, "uint64_t")  ? UINT64__  :
    strcmp(type_str, "int64_t")   ? INT64__   :
    (strcmp(type_str, "void*")    || strcmp(type_str, "obj_t*") || strcmp(type_str, "ptr")) ? PTR__ :
    NOT_POD;

  return outtype;
}

