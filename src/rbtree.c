#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *nil = malloc(sizeof(node_t));

  nil->color = RBTREE_BLACK;
  p->nil = nil;
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void right_rotate(rbtree* t, node_t *x){
  node_t* node = rbtree_find(t, x->key);
  node_t* y = node->left;
  x->left = y->right;
  if(y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil)
    t->root = y;
  else if(x->parent->left == x)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->right = x;
  x->parent = y;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {

  node_t* node = (node_t*)malloc(sizeof(node_t));
  node->color = RBTREE_RED;
  node->key = key;

  return node; // 바꿔야함
  // TODO: implement insert  
  // return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {  
  node_t* cur = t->root;
  
  while(cur != t->nil || cur->key == key){
    if(key < cur->key)
      cur = cur->left;
    else 
      cur = cur->right;
  }
  
  return cur;
  // TODO: implement find
  // return t->root;
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
