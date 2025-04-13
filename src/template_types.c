#include "template_types.h"


//////////////////////// Compress Char Table //////////////////////////////
/* NULL : '\0' : 0x0 
 * 1 : 1 : 0x1 
 * 2 : 2 : 0x2 
 * ...
 * 8 : 8 : 0x8 
 * 9 : 9 : 0x9 
 * A : a : 10 : 0xA  
 * B : b : 11 : 0xB
 * C : c : 12 : 0xC
 * D : d : 13 : 0xD 
 * E : e : 14 : 0xE
 * F : f : 15 : 0xF
 * G : g : 16 : 0x10 
 * H : h : 17 : 0x11
 * I : i : 18 : 0x12
 * J : j : 19 : 0x13
 * K : k : 20 : 0x14
 * L : l : 21 : 0x15 
 * M : m : 22 : 0x16
 * N : n : 23 : 0x17
 * O : o : 24 : 0x18
 * P : p : 25 : 0x19 
 * Q : q : 26 : 0x1A 
 * R : r : 27 : 0x1B
 * S : s : 28 : 0x1C
 * T : t : 29 : 0x1D 
 * U : u : 30 : 0x1E 
 * V : v : 31 : 0x1F
 * W : w : 32 : 0x20 
 * X : x : 33 : 0x21
 * Y : y : 34 : 0x22
 * Z : z : 35 : 0x23
 * 0 : 36 : 0x24
 * _ : 37 : 0x25
 * INVALID : 38 : 0x26
 * * : 39 : 0x27
 */ 


compressed_char_t compress_char(char c){
  //create a char with just the 6 bits of our format populated 
  compressed_char_t compressed_char = 0x0UL;
  if (c >= 0x41 && c <= 0x5A) //uppercase 
    compressed_char = c - 0x40 + 0xA; 
  else if (c >= 0x61 && c <= 0x7A) //lowercase
    compressed_char = c - 0x60 + 0xA;
  else if (c >= 0x31 && c <= 0x39) //1-9
    compressed_char = c - 0x30;
  else if (c == 0x30) 
    compressed_char = 0x24;
  else if (c == '\0') 
    compressed_char = 0x0;
  else if (c == '*')
    compressed_char = 0x27;
  else 
    perror("Character not supported!\n");

  compressed_char &= (char)0x3F; //0xE0 = 0b 0011 1111  --- 6-bit format
  return compressed_char;
} 

char translate_compressed_char(compressed_char_t cc){
  char translated = 
    //alpha? translate to lower case 
    (0xA <= cc && cc <= 0x23) ? cc - 0xA + 0x61 :
    //numeric? translate to decimal character 
    (0x1 <= cc && cc <= 0x9)  ? cc - 0x1 + 0x31 :
    // == '0'? translate to character '0' 
    (cc == 0x24)              ? '0'             :
    // == '_'? translate to character '_'
    (cc == 0x25)              ? '_'             :
    // INVALID? trasnlate to character 0x15 - nACK
    (cc == 0x26)              ? 0x15            :
    // == '*' ? translate to character '*'
    (cc == 0x27)              ? '*'             :
    // == null terminator? translate to character 0 
    0;
  return translated;
}

char decompress_char(template_arg_t cmp_str, int idx){
  if (idx >= CHARS_PER_ARG) return 0x15; //nACK!

  template_arg_t mask = 0x3FULL << (idx * COMPRESSED_CHAR_BITWIDTH);
  template_arg_t masked = cmp_str & mask;
  compressed_char_t cmp_chr = masked >> (idx * COMPRESSED_CHAR_BITWIDTH);
  char decompressed = translate_compressed_char(cmp_chr);
  return decompressed;
} 

void mark_packed(template_arg_t* just_packed){
  template_arg_t MSB_hot = 0x1ULL << (sizeof(template_arg_t) * 8 - 1);
  *just_packed = *just_packed | MSB_hot;
}


template_arg_t pack_templ_arg_str(char* type_str){
  template_arg_t packed = 0x0UL; 
  for (int i = 0; i < CHARS_PER_ARG; i++){
    //get our compressed character 
    compressed_char_t cmpchr = compress_char(type_str[i]);
    packed |= (template_arg_t)cmpchr << i * COMPRESSED_CHAR_BITWIDTH;
    if (cmpchr == 0) break; //hit the null terminator! done packing.
  }
  mark_packed(&packed);
  return packed;
}

void unpack_templ_arg_str(template_arg_t packed, char unpacked[CHARS_PER_ARG]){
  for (int i = 0; i < CHARS_PER_ARG; i++){
    char decompressed = decompress_char(packed, i);
    unpacked[i] = decompressed;
    if (decompressed == 0) break;
    else if (decompressed == 0x15){
      fprintf(stderr, "Character decompression error at index %d.\n", i);
      abort();
    }
  }
}


void print_compressed_str(template_arg_t packed){
  for (int i = 0; i < CHARS_PER_ARG; i++){
    char to_print = decompress_char(packed, i);
    printf("%c",to_print);
  }
  printf("\n");
}


template_arg_t template_arg(char* type_str){
  /* 0? Check to see if the string happens to be a sizeof of a pod_type or 
   *    just a number. If so, encode the appropriate size_t.  <<<MAYBE LATER>>>
   * 1. Convert the string to an array of 21 letters in the 5 bit format by
   *    shifting along the int. 
   * 2. Mark bit 63 to indicate if this is one of the string arguments 
   *    (I do not expect exabyte object sizes, tbh)
   */
  template_arg_t packed = pack_templ_arg_str(type_str);

  return packed;
}


pod_type_t pod_type(template_arg_t type){
  pod_type_t outtype = NOT_POD;
  char type_str[CHARS_PER_ARG];
  unpack_templ_arg_str(type, type_str);

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

