#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// for insert procedure
//---------------------------------------------------------
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert  
  return t->root;
  "test"
}
qwerqwer
void right_rotate(rbtree* t, node_t *x){
}

void left_rotate(rbtree* t, node_t *x){
}

void rb_insert_fixup(rbtree *t, node_t *node){
}

//---------------------------------------------------------

node_t *rbtree_find(const rbtree *t, const key_t key) {  
  // TODO: implement find
  return t->root;
}



node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
