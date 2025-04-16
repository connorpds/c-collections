#include "vector_test.h"
#include "collections.h"
#include "utils/template_types.h"


//have a union type for the constructor - can either have a switch statement for strings or 
//just interpret as sizeof.


int construct_test(){
  //let's make an int vector!
  vector_t* intv_0 = vector(sizeof(int));
  if (intv_0->type == VECTOR) return 0;
  else return 1;
}

int push_back_test(){
  //let's make an int vector!
  vector_t* intv_0 = vector(template_arg("int"));
  int tmp42 = 42; 
  push_back(intv_0, &tmp42);
  int* i0 = intv_0->begin_ptr;
  if (*i0 == 42) return 0;
  else return 1;
}

int testf(){
  char joe = 'j';
  int mama = 1;
  double shrek = 69.0;
  return mama + joe + shrek;
}



void int_print(obj_t* val){
  printf("%d", *(int*)val);
}

void print_test(){
  printf("Testing printing one-element vector:");
  vector_t* intv_0 = vector(sizeof(int));
  set_value_print_fun(intv_0, int_print);
  int tmp42 = 42; 
  push_back(intv_0, &tmp42);
  print_coll(intv_0);
}

int index_test(){
  vector_t* intv_0 = vector(sizeof(int));
  set_value_print_fun(intv_0, int_print);
  int tmp42 = 42; 
  push_back(intv_0, &tmp42);
  int* new42 = index_(intv_0, 0);
  return *new42;
}


vector_t* int_vector_basic(){
  vector_t* intv_0 = vector(sizeof(int));
  set_value_print_fun(intv_0, int_print);
  return intv_0;
}


vector_t* int_vector_pod(){
  vector_t* intv_0 = vector(template_arg("int"));
  set_value_print_fun(intv_0, int_print);
  return intv_0;
}



void fibonacci_printtest(vector_t* vec, int n){
  printf("### TESTING PRINTING A FIBONACCI SEQUENCE ###\n");
  //printf("Entered compound 0!\n");
  int zero = 0; 
  push_back(vec, &zero);
  //printf("Zero pushed back!\n");
  print_coll(vec);
  int one = 1; 
  push_back(vec, &one);
  //printf("One pushed back!\n");
  print_coll(vec);

  for (int i = 2; i < n; i++){
    int* antecedent = index_(vec, i - 1);
    int* preantecedent = index_(vec, i - 2);
    int descendant = *antecedent + *preantecedent;
    push_back(vec, &descendant);
    print_coll(vec);
  }
  printf("### FIBONACCI PRINT TESTING COMPLETE ###\n\n");
}

void fibonacci(vector_t* vec, int n){
  int zero = 0; 
  push_back(vec, &zero);
  int one = 1; 
  push_back(vec, &one);

  for (int i = 2; i < n; i++){
    int* antecedent = index_(vec, i - 1);
    int* preantecedent = index_(vec, i - 2);
    int descendant = *antecedent + *preantecedent;
    //printf("descendant = %d\n", descendant);
    push_back(vec, &descendant);
  }
}

//TODO: change the typing of obj_t* as necessary to make this work correctly.
void pod_fibonacci(vector_t* vec, int n){
  push_back(vec, 0);
  push_back(vec, 1);
  for (int i = 2; i < n; i++){
    int antecedent = *index_(vec, i - 1);
    int preantecedent = *index_(vec, i - 2);
    push_back(vec, antecedent + preantecedent);
  }
}

void erase_1_idx_test(){
  printf("### TESTING SINGLE INDEX ERASURE ###\n");
  vector_t* vec = int_vector_basic();
  fibonacci(vec, 10);
  printf("Size: %lu\n", size(vec));
  print_coll(vec);
  erase_idx(vec, 7);
  printf("Erased index 7!\n");
  printf("Size: %lu\n", size(vec));
  print_coll(vec);
  free(vec);
  printf("### SINGLE INDEX ERASURE TESTING COMPLETE ###\n\n");
}

void clear_test(){
  printf("### TESTING VECTOR CLEARING ###\n");
  vector_t* vec = int_vector_basic();
  printf("Fibonacci indices 0-5:\n");
  fibonacci(vec, 6);
  printf("Size: %lu\n", size(vec));
  print_coll(vec);
  printf("Clearing!\n");
  clear(vec);
  printf("Size: %lu\n", size(vec));
  print_coll(vec);
  printf("Repopulating with Fibonacci indices 0-12:\n");
  fibonacci(vec, 13);
  printf("Size: %lu\n", size(vec));
  print_coll(vec);
  printf("Clearing!\n");
  clear(vec);
  printf("Size: %lu\n", size(vec));
  print_coll(vec);
  printf("### VECTOR CLEARING TESTING COMPLETE ###\n\n");
}



int main(){
  printf("Construction test result: %d (0 = SUCCESS)\n", construct_test());
  printf("Push_back test result: %d (0 = SUCCESS)\n", push_back_test());
  print_test();
  printf("Index result: %d\n", index_test()); 
  vector_t* fib0 = int_vector_basic();
  fibonacci_printtest(fib0, 10);
  erase_1_idx_test();
  clear_test();

  return 0;// + push_back_test();
}


