#include "template_types.h"
#include <stdint.h>


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



wide_pod_t create_typemask(size_t type_size){
  wide_pod_t mask = 0;
  for (int i = 0; i < type_size; i++)
    mask |= (0xFF << i * sizeof(char));
  return mask;
}


size_t pod_size(pod_type_t type){
  size_t retsize = 0;
  switch (type){
    case NOT_POD:
      retsize = 0;
      break;
    case CHAR__:
      retsize = sizeof(char);
      break;
    case INT__:
      retsize = sizeof(int);
      break;
    case LONG__:
      retsize = sizeof(long);
      break;
    case SHORT__:
      retsize = sizeof(short);
      break;
    case INT8__:
      retsize = sizeof(int8_t);
      break;
    case UINT8__:
      retsize = sizeof(uint8_t);
      break;
    case INT16__:
      retsize = sizeof(int16_t);
      break;
    case UINT16__:
      retsize = sizeof(uint16_t);
      break;
    case INT32__:
      retsize = sizeof(int32_t);
      break;
    case UINT32__:
      retsize = sizeof(uint32_t);
      break;
    case INT64__:
      retsize = sizeof(int64_t);
      break;
    case UINT64__:
      retsize = sizeof(uint64_t);
      break;
    case BOOL__:
      retsize = sizeof(bool);
      break;
    case FLOAT__:
      retsize = sizeof(float);
      break;
    case DOUBLE__:
      retsize = sizeof(double);
      break;
    case PTR__:
      retsize = sizeof(void*);
      break;
    default:
      retsize = 0;
      break;
  }

  return retsize;

}
