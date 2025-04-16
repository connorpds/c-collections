#include "string_packed_ints.h"


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
 * SPACE : 40 : 0x28 
 */ 


compressed_char_t compress_char(char c){
  //create a char with just the 6 bits of our format populated 
  compressed_char_t compressed_char = 0x0UL;
  if (c >= 0x41 && c <= 0x5A) //uppercase 
    compressed_char = c - 0x41 + 0xA; 
  else if (c >= 0x61 && c <= 0x7A) //lowercase
    compressed_char = c - 0x61 + 0xA;
  else if (c >= 0x31 && c <= 0x39) //1-9
    compressed_char = c - 0x30;
  else if (c == 0x30) 
    compressed_char = 0x24;
  else if (c == '\0') 
    compressed_char = 0x0;
  else if (c == '_')
    compressed_char = 0x25;
  else if (c == '*')
    compressed_char = 0x27;
  else if (c == ' ')
    compressed_char = 0x28;
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
    // == SPACE ? translate to character ' ' 
    (cc == 0x28)              ? ' '             :
    // == null terminator? translate to character 0 
    0;
  return translated;
}

char decompress_char(packed_int_t cmp_str, int idx){
  if (idx >= CHARS_PER_ARG) return 0x15; //nACK!
  packed_int_t mask = 0x3F;
  mask = mask << (idx * COMPRESSED_CHAR_BITWIDTH);
  packed_int_t masked = cmp_str & mask;
  compressed_char_t cmp_chr = masked >> (idx * COMPRESSED_CHAR_BITWIDTH);
  char decompressed = translate_compressed_char(cmp_chr);
  return decompressed;
} 

void mark_packed(packed_int_t* just_packed){
  packed_int_t MSB_hot = (packed_int_t)0x1 << (sizeof(packed_int_t) * 8 - 1);
  *just_packed = *just_packed | MSB_hot;
}


packed_int_t string_packed_int(const char* str){
  packed_int_t packed = 0x0UL; 
  for (int i = 0; i < CHARS_PER_ARG; i++){
    //get our compressed character 
    compressed_char_t cmpchr = compress_char(str[i]);
    packed |= (packed_int_t)cmpchr << i * COMPRESSED_CHAR_BITWIDTH;
    if (cmpchr == 0) break; //hit the null terminator! done packing.
  }
  mark_packed(&packed);
  return packed;
}

void unpack_string(packed_int_t packed, char unpacked[CHARS_PER_ARG]){
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


void print_compressed_string(packed_int_t packed){
  for (int i = 0; i < CHARS_PER_ARG; i++){
    char to_print = decompress_char(packed, i);
    if (to_print != 0) printf("%c", to_print);
    else break;
  }
}


void test_print_compressed_string(char* test_str){
  packed_int_t packed = string_packed_int(test_str);
  printf("Input string:\n%s\n", test_str);
  for (int i = 0; i < CHARS_PER_ARG; i++){
    char to_print = decompress_char(packed, i);
    if (to_print != 0){ 
      printf("At index %d: %c   ", i, to_print);
      printf("Expected: %c   ", test_str[i]);
      printf("\n");
    }
    else break;
  }
  printf("\n");
}




void test_char_translation(char c){
  compressed_char_t cmprsd = compress_char(c);
  char dcmprsd = translate_compressed_char(cmprsd);
  printf("Input character: %c, Translated: %c\n", c, dcmprsd);
  printf("(int)Input: %d, (int)Translated: %d\n", c, dcmprsd);
}

void test_packed_compressed_print(char* str){
  printf("Input string:\n%s\n", str);
  printf("Compressed and decompressed string:\n");
  packed_int_t potato = string_packed_int(str);
  print_compressed_string(potato);
  printf("\n");
}

void string_packed_int_tests(){
  printf("CHARS_PER_ARG: %d\n", CHARS_PER_ARG);
  test_char_translation('a');
  test_char_translation('A');
  test_char_translation('*');
  test_char_translation('2');
  printf("Nonverbose string packing test(s):\n");
  test_packed_compressed_print("potato");
  printf("Verbose string packing test(s):\n");
  test_print_compressed_string("uint128_t");
  test_print_compressed_string("unsigned long long");

}


int main(){
   string_packed_int_tests();
   return 0;
}
