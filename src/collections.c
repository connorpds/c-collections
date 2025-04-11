#include "collections.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////    CONSTRUCTION   /////////////////////////////////////
////////////////////////////////////////////////////////////////
void set_key_print_fun(coll_t* coll, print_fun_t fun){
  coll->template_key_print = fun;
}

void set_value_print_fun(coll_t* coll, print_fun_t fun){
  coll->template_value_print = fun;
}

void set_template_value_destructor(coll_t* coll, delete_fun_t fun){
  coll->template_value_destructor = fun;
}

void set_template_key_destructor(coll_t* coll, delete_fun_t fun){
  coll->template_key_destructor = fun;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////   HELPERS   //////////////////////////////////////////
////////////////////////////////////////////////////////////////

bool bytes_equal(obj_t* obj_t0, obj_t* obj_t1, size_t obj_t_size){
  char* obj_t0_bytes = (char*)obj_t0;
  char* obj_t1_bytes = (char*)obj_t1;
  for (int byte_ct = 0; byte_ct < obj_t_size; byte_ct++){
    if (obj_t0_bytes[byte_ct] != obj_t1_bytes[byte_ct]) return false;
  }
  return true;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////  BOUND METHOD WRAPPERS  /////////////////////////////
/////////////////////////////////////////////////////////////////

 obj_t* index_(coll_t* coll, int idx){
  return coll->index(coll, idx);
}

 obj_t* lookup(coll_t* coll, obj_t* query){
  return coll->lookup(coll, query);
}

 void emplace_back(coll_t* coll, obj_t* element_loc){
  coll->emplace_back(coll, element_loc);
}

 void push_back(coll_t* coll, obj_t* element_loc){
  coll->push_back(coll, element_loc);
}

 void clear(coll_t* coll){
  coll->clear(coll);
}

 void clear_free(coll_t* coll){
  coll->clear_free(coll);
}

 obj_t* insert(coll_t* coll, obj_t* arg2, obj_t* arg3){
  return coll->insert(coll, arg2, arg3);
}

 void write(coll_t* coll, obj_t* idx, obj_t* element_loc){
  coll->write(coll, idx, element_loc);
}

 void erase_region(coll_t* coll, obj_t* region_start, obj_t* region_end){
  coll->erase_region(coll, region_start, region_end);
}

 void erase(coll_t* coll, obj_t* victim_loc){
  coll->erase(coll, victim_loc);
}

 obj_t* erase_idx(coll_t* coll, int idx){
  return coll->erase_idx(coll, idx);
}

 obj_t* find(coll_t* coll, obj_t* query){
  return coll->find(coll, query);
}

 obj_t* remove_(coll_t* coll, obj_t* query){
  return coll->remove(coll, query);
}

 size_t size(coll_t* coll){
  return coll->size(coll);
}

void print_coll(coll_t* coll){
  coll->print_coll(coll);
}
