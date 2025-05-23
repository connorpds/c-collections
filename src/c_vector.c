#include "c_vector.h"
#include "collections.h"
#include "utils/marked_ptrs.h"
#include "utils/string_packed_ints.h"
#include "utils/template_types.h"
#include <stdint.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////  S  /////////////////////////////////////////
///////////////////////////////////////////

void vec_update_end(vector_t* vec){
  vec->end_ptr = (char*)vec->begin_ptr + vec->element_size * vec->num_elements;
}



//be careful with this. 
void vec_incr_num_elements(vector_t* vec, int count){
  //apply offset to element ct 
  vec->num_elements += count; 
  //update end ptr (d)
  vec_update_end(vec);
}

//////////////////////////////////////////////////////////////////////////////////////////
////////////////////// CONSTRUCTION  /////////////////////////////////////
////////////////////////////////////////////

//basic constructor 
vector_t* vector(template_arg_t T){
  
  
  //allocate struct space 
  vector_t* vec = calloc(1, sizeof(vector_t));
  //set the element size 
  //set the type to vector 
  vec->type = VECTOR;

  if (!is_packed(T)){
    vec->element_size = (size_t)T;  
  }
  else{
    pod_type_t type = pod_type(T);
    vec->element_size = pod_size(type);
  }

  bind_vector_methods(vec);
  vec->value_mask = create_typemask(vec->element_size);
  

  return vec; 
}

void bind_vector_methods(vector_t* vec){
  vec->index = vec_index;
  vec->emplace_back = vec_emplace_back;
  vec->push_back = vec_push_back;
  vec->clear = vec_clear;
  vec->insert = (obj_t* (*)(coll_t*, obj_t*, obj_t*))vec_insert;
  vec->write = (void (*)(coll_t*, obj_t*, obj_t*))vec_write; 
  vec->erase_region = vec_erase_region;
  vec->erase = vec_erase;
  vec->erase_idx = vec_erase_idx;
  vec->find = vec_find;
  vec->remove = vec_remove;
  vec->size = vec_size;
  vec->print_coll = vec_print;

}


//////////////////////////////////////////////////////////////////////////////////////////
////////////////////// CONTENT OPERATIONS  /////////////////////////////////////
/////////////////////////////////////////////////////


obj_t* vec_index(vector_t* vec, int idx){
  size_t offset_bytes = idx * vec->element_size;
  obj_t* idx_loc = (char*)vec->begin_ptr + offset_bytes;
  return idx_loc;
}


void vec_emplace_back(vector_t* vec, obj_t* element_loc){
  memmove(vec->end_ptr, element_loc, vec->element_size);
  free(element_loc);
  vec->num_elements++;
  vec_update_end(vec);
}



//push a copy of the element at element_loc onto the back of the vector 
void vec_push_back(vector_t* vec, obj_t* element_loc){
  size_t element_size = vec->element_size;
  size_t num_allocated = vec->num_allocated;

  //if no allocation yet, let's allocate some heap space for the first obj_tect 
  if (!num_allocated){ 
    vec->begin_ptr = malloc(element_size);  
    vec->num_allocated++;
  }
  else if (vec->num_elements >= num_allocated){
    //allocate new, larger region 
    vec->begin_ptr = realloc(vec->begin_ptr, element_size * num_allocated * 2);
    vec->num_allocated *= 2;
  }
  //update end to reflect new begin 
  vec_update_end(vec);
  //move element to end_ptr because this is push_back 
  //printf("Element_loc: %p. Unmarked: %p.\n", element_loc, unmarked_ptr(element_loc));
  memmove(vec->end_ptr, unmarked_ptr(element_loc), element_size); // <- intel syntax, gross!
  free_if_marked(element_loc);
  vec->num_elements++;
  vec_update_end(vec);
}



obj_t* vec_insert(vector_t* vec, obj_t* element_loc, unsigned idx){
  size_t num_elements = vec->num_elements;
  size_t num_allocated = vec->num_allocated;
  size_t element_size = vec->element_size;
  
  if (idx > num_elements) return NULL;
  else if (num_elements + 1 > num_allocated){ // need to allocate more space!
    vec->begin_ptr = realloc(vec->begin_ptr, element_size * num_allocated * 2);
    vec->num_allocated *= 2;
    vec_update_end(vec);
  }
  obj_t* insert_loc = vec_index(vec, idx);
  //shift post-idx obj_tects to higher indices
  memmove(insert_loc + element_size, insert_loc, (char*)vec->end_ptr - (char*)insert_loc);
  //copy obj_tect to insert_loc
  memmove(insert_loc, unmarked_ptr(element_loc), element_size);
  free_if_marked(element_loc);
  //increment element count post-insertion
  vec->num_elements++;
  //update end_ptr 
  vec_update_end(vec);
  return insert_loc;
}


void vec_write(vector_t* vec, unsigned idx, obj_t* data_loc){
  obj_t* write_ptr = vec_index(vec, idx);
  memmove(write_ptr, unmarked_ptr(data_loc), vec->element_size);
  free_if_marked(data_loc);
}


void vec_erase_region(vector_t* vec, obj_t* region_start, obj_t* region_end){
  size_t end_diff = (char*)vec->end_ptr - (char*)region_end;
  size_t bytes_del = (char*)region_end - (char*)region_start; 
  obj_t* new_end = (char*)vec->end_ptr - bytes_del;
  
  if (bytes_del % vec->element_size) return;
  //move data post-end to start ptr 
  memmove(region_start, region_end, end_diff);
  //zero data between old end and old end - bytes_del
  memset(new_end, 0, bytes_del);
  vec->num_elements -= bytes_del / vec->element_size;
}

void vec_erase(vector_t* vec, obj_t* victim_loc){
  size_t element_size = vec->element_size;
  obj_t* new_end = (char*)vec->end_ptr - element_size;

  size_t bytes_to_move = (uintptr_t)(vec->end_ptr) - (uintptr_t)victim_loc - element_size;

  //move data post-loc 
  memmove(victim_loc, (char*)victim_loc + element_size, bytes_to_move);
  //0 the duplicate 
  memset(new_end, 0, element_size);
  vec_incr_num_elements(vec, -1);
}

obj_t* vec_erase_idx(vector_t* vec, int idx){
  size_t element_size = vec->element_size;
  obj_t* victim_loc = (char*)vec->begin_ptr + element_size * idx;
  obj_t* new_end = (char*)vec->end_ptr - element_size;
  size_t bytes_to_move = (uintptr_t)(vec->end_ptr) - (uintptr_t)victim_loc - element_size;
  //move data post-idx to earlier addrs 
  memmove(victim_loc, (char*)victim_loc + element_size, bytes_to_move);
  memset(new_end, 0, element_size);
  vec_incr_num_elements(vec, -1);
  return victim_loc;
}

obj_t* vec_find(vector_t* vec, obj_t* query_data){
  for (int i = 0; i < vec->num_elements; i++){
    //first get the obj_t 
    obj_t* obj_t_at_i = vec_index(vec, i);
    if (bytes_equal(obj_t_at_i, unmarked_ptr(query_data), vec->element_size)){
      free_if_marked(query_data);
      return obj_t_at_i;
    }
  }
  free_if_marked(query_data);
  return vec->end_ptr;
}

obj_t* vec_remove(vector_t* vec, obj_t* query_data){
  obj_t* findres = vec_find(vec, query_data);
  if (findres != vec->end_ptr){
    vec_erase(vec, findres);
  }
  return findres;
}



//////////////////////////////////////////////////////////////////////////////////////////
/////////////// HELPERS  /////////////////////////////////////
/////////////////////////////////////////////////

size_t vec_size(vector_t* vec){
  return vec->num_elements;
}

void vec_print(vector_t* vec){
  printf("{");
  for (int i = 0; i < vec->num_elements; i++){
    if (i > 0) printf(", ");
    vec->template_value_print(vec->index(vec, i));
  }
  printf("}\n");
}

int vec_bounds_check(vector_t* vec, obj_t* ptr){
  int begin_diff = (char*)ptr - (char*)vec->begin_ptr;
  bool aligned = !(begin_diff % vec->element_size);
  bool ptr_ge_begin = (begin_diff >= 0);
  bool ptr_l_end = ((char*)vec->end_ptr - (char*) ptr) > 0;
  
  return (!aligned)          ?  -1  : //PTR NOT ALIGNED TO OBJECT SIZE 
         (!ptr_ge_begin)     ?  1   : //PTR BEFORE START REGION 
         (!ptr_l_end)        ?  2   : //PTR AFTER END REGION 
                                0   ; //PTR IS WITHIN BOUNDS 
}

void vec_clear(vector_t* vec){
  free(vec->begin_ptr);
  vec->begin_ptr = NULL;
  vec->end_ptr = NULL;
  vec->num_elements = 0;
  vec->num_allocated = 0;
}


void vec_clear_free(vector_t* vec){
  for (int i = 0; i < vec->num_elements; i++){
    obj_t* element_loc = vec_index(vec, i);
    uint64_t* element = *(uint64_t**)element_loc;
    free((obj_t*)element);
  }
  vec_clear(vec);
}



/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////  TYPE-SPECIFIC FUNCTIONS ////////////// 
///////////////////////////////////////////////////////////
void pod_vec_push_back(vector_t* vec, wide_pod_t val){
  val = typemask_value(vec, val);
  vec_push_back(vec, (obj_t*)&val);
}
//todo: packed_ptr_t


obj_t* pod_vec_insert(vector_t* vec, wide_pod_t val, unsigned idx){
  val = typemask_value(vec, val);
  return vec_insert(vec, (obj_t*)&val, idx);
}

obj_t* pod_vec_find(vector_t* vec, wide_pod_t val){
  val = typemask_value(vec, val);
  return vec_find(vec, (obj_t*)&val);
}

obj_t* pod_vec_remove(vector_t* vec, wide_pod_t val){
  val = typemask_value(vec, val);
  return vec_remove(vec, (obj_t*)&val);
}
