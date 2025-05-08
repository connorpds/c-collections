#include "hash_tables.h"


unsigned long mult_hash(const char* key_object, size_t object_size){
  unsigned long hash_output = 0;
  unsigned const char* key_chars = (unsigned const char*)key_object;
  for (int i = 0; i < object_size; i++){
    hash_output = hash_output * MULT_HASH_MULTVAL + *key_chars;
    key_chars++;
  } 
  return hash_output;
}

hash_table_tree_node* make_hash_tree_node(char* key_object, size_t key_size, 
                                          char* value_object, size_t value_size){
  hash_table_tree_node* node = malloc(sizeof(hash_table_tree_node));
  //hash our key 
  node->hashed_key = mult_hash(key_object, key_size);
  //allocate/copy key 
  node->key = malloc(key_size);
  memcpy(node->key, key_object, key_size);
  //allocate/copy val 
  node->value = malloc(value_size);
  memcpy(node->value, value_object, value_size);

  return node;
}



hash_table_tree_node* htt_binary_search(hash_table_tree_node* search_root, char* key_object, size_t key_size){
  //hash la cle 
  unsigned long hashed = mult_hash(key_object, key_size);
  hash_table_tree_node* curr = search_root;
  while (curr){
    if (curr->hashed_key == hashed) return curr;
    else if (curr->left && (curr->left->hashed_key > hashed)) curr = curr->left; 
    else if (curr->right && (hashed > curr->right->hashed_key)) curr = curr->right;
    else break;
  }
  return NULL;
}

hash_table_tree_node* htt_place_node(hash_table_tree_node* root, char* key_object, size_t key_size){

}
