#pragma once
#include "utils/marked_ptrs.h"
#include <stddef.h>
#include <stdlib.h>

#define MULT_HASH_MULTVAL (37)


unsigned long mult_hash(const char *s, size_t object_size);

typedef struct hash_table_tree_node hash_table_tree_node;


struct hash_table_tree_node{
  hash_table_tree_node* left;
  hash_table_tree_node* right;
  unsigned long hashed_key;
  char* key;
  char* value;
};

//hash_table_tree_node* new_binary_tree_node(size_t data_size);

hash_table_tree_node* make_hash_tree_node(char* key_object, size_t key_size, 
                                        char* value_object, size_t value_size);

hash_table_tree_node* binary_search_htt(hash_table_tree_node* search_root, char* key_object, size_t key_size);


