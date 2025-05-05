#pragma once 
#include "utils/template_types.h"
#include "utils/marked_ptrs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef enum {
  no_type,
  VECTOR,
  MAP,
  SET,
  UNORDERED_MAP,
  LIST
} collection_typename_t;


typedef enum {KEY, VALUE} key_val;

//for printing collection contents 
typedef void (*print_fun_t)(obj_t*);
//for destructing obj_tects  
typedef void (*delete_fun_t)(obj_t*);

typedef struct coll_t coll_t;
typedef struct coll_t vector_t;

struct coll_t{  
  //collection type 
  collection_typename_t type;
  //content type size 
  size_t element_size;
  size_t key_size;
  size_t value_size;

  //Methods!
  obj_t* (*index)(coll_t*, int);
  obj_t* (*lookup)(coll_t*, obj_t*);
  void (*emplace_back)(coll_t*, obj_t*);
  void (*push_back)(coll_t*, obj_t*);
  void (*clear)(coll_t*);
  void (*clear_free)(coll_t*);
  obj_t* (*insert)(coll_t*, obj_t*, obj_t*);
  void (*write)(coll_t*, obj_t*, obj_t*);
  void (*erase_region)(coll_t*, obj_t*, obj_t*);
  void (*erase)(coll_t*, obj_t*);
  obj_t* (*erase_idx)(coll_t*, int);
  obj_t* (*find)(coll_t*, obj_t*);
  obj_t* (*remove)(coll_t*, obj_t*);
  size_t (*size)(coll_t*);
  void (*print_coll)(coll_t*);

  //how many elements resident?
  size_t num_elements;
  //how many elements allocated?
  size_t num_allocated;

  print_fun_t template_value_print;
  print_fun_t template_key_print;

  wide_pod_t key_mask;
  wide_pod_t value_mask;

  delete_fun_t template_value_destructor;
  delete_fun_t template_key_destructor;
  
  //begin_ptr and end_ptr for vector_find(). 
  //also good for realloc() calls and frees 
  obj_t* begin_ptr;
  obj_t* end_ptr;

 };

///////////////////////// HELPERS ///////////////////////////////
bool bytes_equal(obj_t* obj_t0, obj_t* obj_t1, size_t obj_t_size);
wide_pod_t typemask_key(coll_t* coll, wide_pod_t key);
wide_pod_t typemask_value(coll_t* coll, wide_pod_t val);

///////////////////////// BOUND METHOD WRAPPERS //////////////// 

 obj_t* index_(coll_t* coll, int idx);

 obj_t* lookup(coll_t* coll, obj_t* query);

 void emplace_back(coll_t* coll, obj_t* element_loc);

 void push_back(coll_t* coll, obj_t* element_loc);

 void clear(coll_t* coll);

 void clear_free(coll_t* coll);

 obj_t* insert(coll_t* coll, obj_t* arg2, obj_t* arg3);

 void write(coll_t* coll, obj_t* idx, obj_t* element_loc);

 void erase_region(coll_t* coll, obj_t* region_start, obj_t* region_end);

 void erase(coll_t* coll, obj_t* victim_loc);

 obj_t* erase_idx(coll_t* coll, int idx);

 obj_t* find(coll_t* coll, obj_t* query);

 obj_t* remove_(coll_t* coll, obj_t* query);

 size_t size(coll_t* coll);

 void print_coll(coll_t* coll);


////////////////////////  METHOD BINDERS  /////////////////////
void set_value_print_fun(coll_t* coll, print_fun_t fun);
void set_key_print_fun(coll_t* coll, print_fun_t fun);

///////////////////////  PASSING WRAPPING  ///////////////////
obj_t* pod(wide_pod_t val);
