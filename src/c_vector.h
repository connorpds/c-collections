#pragma once 

#include "collections.h"
#include <stdint.h>

//////////////////////////////////////// SIMPLE S ////////////////////////////////////

 void vec_update_end(vector_t* vec);



//be careful with this. 
 void vec_incr_num_elements(vector_t* vec, int count);


///////////////////////////////////////  CONSTRUCTORS   /////////////////////////////////// 
vector_t* vector(template_type_arg_t constr_arg);
void bind_vector_methods(vector_t* vec);


//////////////////////////////////////   OPERATIONS     //////////////////////////////////
obj_t* vec_index(vector_t* vec, int idx);

int vec_bounds_check(vector_t* vec, obj_t* ptr);//check a ptr to make sure it's within the vector and aligned with an obj_tect 
void vec_write(vector_t* vec, unsigned idx, obj_t* data_loc);

void vec_emplace_back(vector_t* vec, obj_t* element_loc);
void vec_push_back(vector_t* vec, obj_t* element_loc);
void vec_clear(vector_t* vec);
void vec_erase_region(vector_t* vec, obj_t* region_start, obj_t* region_end);
void vec_erase(vector_t* vec, obj_t* victim_loc);
obj_t* vec_erase_idx(vector_t* vec, int idx);


obj_t* vec_insert(vector_t* vec, obj_t* element_loc, unsigned idx);
obj_t* vec_find(vector_t* vec, obj_t* query); 
obj_t* vec_remove(vector_t* vec, obj_t* query);

size_t vec_size(vector_t* vec);

void vec_print(vector_t* vec);

void vec_clear_free(vector_t* vec); //if vec holds ptrs, free those.


//TODO: fill these out, then for every type rip 
void char_vec_push_back(vector_t* vec, char c);
void char_vec_insert(vector_t* vec, char c, unsigned idx);
void char_vec_find(vector_t* vec, char c);
void char_vec_remove(vector_t* vec, char c);





