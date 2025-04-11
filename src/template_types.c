#include "template_types.h"

pod_type_t pod_type(template_type_arg_t type){
  pod_type_t outtype = NOT_POD;

  outtype = 
    strcmp(type.type_str4, "char")   ?  CHAR__   :
    strcmp(type.type_str5, "float")  ?  FLOAT__  :
    strcmp(type.type_str6, "double") ?  DOUBLE__ :
    strcmp(type.type_str4, "bool")   ? BOOL__    :
    strcmp(type.type_str3, "int")    ? INT32__   :
    strcmp(type.type_str5, "short")  ? INT16__   :
    (strcmp(type.type_str7, "uint8_t")  || strcmp(type.type_str7, "int8_t"))  ? INT8__  : 
    (strcmp(type.type_str8, "uint16_t") || strcmp(type.type_str8, "int16_t")) ? INT16__ :
    (strcmp(type.type_str8, "uint32_t") || strcmp(type.type_str8, "int32_t")) ? INT32__ :
    (strcmp(type.type_str8, "uint64_t") || strcmp(type.type_str8, "int64_t")) ? INT64__ :
    (strcmp(type.type_str5, "void*")    || strcmp(type.type_str6, "obj_t*") || strcmp(type.type_str3, "ptr")) ? PTR__ :
    NOT_POD;

  return outtype;
}

