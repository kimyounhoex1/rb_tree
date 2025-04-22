#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t* nil = (node_t*)malloc(sizeof(node_t));
  nil->color = RBTREE_BLACK;

  nil->left = nil;
  nil->right = nil;
  nil->parent = nil;
  // 이거 추가 안하니까 segment fault남.

  p->nil = nil;
  p->root = nil;
  // TODO: initialize struct if needed
  return p;
}
void free_node(rbtree *t, node_t* node){
  if(node == t->nil)
    return;
  free_node(t, node->left);
  free_node(t, node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free_node(t, t->root);
  free(t);
  // 노드를 전부다 삭제하는 과정이 필요하다.
}

// for insert procedure
//---------------------------------------------------------
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *node = (node_t*)malloc(sizeof(node_t));
  node->key = key;
  node_t* cur = t->root;
  node_t* prev = t->nil;

  while(cur != t->nil){
    prev = cur;
    if(key < cur->key)
      cur = cur->left;
    else
      cur = cur->right;
  }
  node->parent = prev;
  if(prev == t->nil){
    // printf("new node insert : %d\n", key);
    t->root = node;
  }
  else if(prev->key > node->key){
    // printf("new node left insert : %d\n", key);
    prev->left = node;
  }
  else{
    // printf("new node right insert : %d\n", key); 
    prev->right = node;
  }
  node->left = t->nil;
  node->right = t->nil;

  rb_insert_fixup(t, node);
    // case1: parent가 B이면 상관 없다. -> rb_insert에서 걸러진다.
  // TODO: implement insert  
  
  return node;
}
void right_rotate(rbtree* t, node_t *x){
  node_t *y = x->left;
  if(y->right != t->nil){
    y->right->parent = x;
  }
  x->left = y->right;
  y->right = x;
  if(x == t->root)
    t->root = y;
  else if(x == x->parent->left)
    x->parent->left = y;
  else 
    x->parent->right = y;
  
  y->parent = x->parent;
  x->parent = y;
}

void left_rotate(rbtree* t, node_t *x){
  node_t* y = x->right;
  x->right = y->left;
  if(y->left != t->nil)
    y->left->parent = x;
  y->left = x;
  if(x == t->root)
    t->root = y;
  else if(x == x->parent->left){
    x->parent->left = y;
  } else
    x->parent->right = y;
  y->parent = x->parent;  
  x->parent = y;
}

// case2: parent가 R인 경우
void rb_insert_fixup(rbtree *t, node_t *node){
  node_t* p = node->parent;
  node_t* u = t->nil;
  if(node == t->root){
    node->color = RBTREE_BLACK;
    return;
  }
  // i) Uncle의 색이 R인 경우
  if(p->color == RBTREE_RED){
    if(p == p->parent->right){
      u = p->parent->left;
      if(u->color == RBTREE_RED){
        // printf("%d, case 1, Red Red \n",node->key);
        p->parent->color = RBTREE_RED;
        p->color = RBTREE_BLACK;
        u->color = RBTREE_BLACK;
        rb_insert_fixup(t, node->parent->parent);
      }
      // ii) Uncle 색이 B인 경우
      else{
        // ii-1) 삽입한 노드가 RL케이스인 경우  
        if(node == node->parent->left){
        // printf("%d, case 2, RL Case\n",node->key);
        node = node->parent;
        right_rotate(t, node);
        }
        // printf("%d, case 3, RR Case\n",node->key);
        // ii-1) 삽입한 노드가 RR케이스인 경우
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        left_rotate(t, node->parent->parent);
      }
    }

    else {
      u = p->parent->right;
      if(u->color == RBTREE_RED){
        p->parent->color = RBTREE_RED;
        p->color = RBTREE_BLACK;
        u->color = RBTREE_BLACK;
        rb_insert_fixup(t, node->parent->parent);
        }
      
      else {
        if(node == node->parent->right){
          node = node->parent;
          left_rotate(t, node);
          }
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        right_rotate(t, node->parent->parent);
      }
    } 
  }
}
//---------------------------------------------------------

node_t *rbtree_find(const rbtree *t, const key_t key) {  
  // TODO: implement find
  node_t* cur = t->root;
  while (cur != t->nil) {
    if (key == cur->key) return cur;
    else if (key < cur->key) cur = cur->left;
    else cur = cur->right;
  }
  return NULL;
}



node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur != t->nil){
    cur = cur->left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur != t->nil){
    cur = cur->right;
  }

  return cur;
}
// for deletion procedure
//---------------------------------------------------------
node_t* tree_minimun(rbtree* t, node_t* node){
  node_t* cur = rbtree_find(t, node->key);
  if(cur->right == t->nil){
    return cur;
  }
  cur = cur->right;
  node_t* prev;
  while(cur != t->nil){
    prev = cur;
    cur = cur->left;
  }
  return prev;
}

void rb_transplant(rbtree *t, node_t *x, node_t *y){
  if(x->parent == t->nil)
    t->root = y;
  else if(x == x->parent->left){
    x->parent->left = y;
  } else
    x->parent->right = y;
  
  y->parent = x->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t* rladbsgh = p;
  // color_t saved_color = rladbsgh->color;
  // 1. 2 child -> change minimun node in right subtree and then deletion
  if(p->left != t->nil && p->right != t->nil){
    rladbsgh = tree_minimun(t, p);
    // saved_color = rladbsgh->color;
    rb_transplant(t, p, rladbsgh);
  }

  // 2. 1 child -> current node deletion and insertion child node

  // 3. 0 child -> just deletion


  // if(p->left == t->nil || p->right == t->nil){
  //   if(p->left == t->nil && p->right == t->nil){
  //     if(p = p->parent->left)
  //       p->parent->left = t->nil;
  //     else
  //       p->parent->right = t->nil;
  //   }
  // }
  // else {
  //   node_t* max = tree_minimun(t, p);
  // }
  // 만약, p의 자식이 2개이상인 경우
  
  // TODO: implement erase
  return 0;
}




//---------------------------------------------------------

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

void print_tree(rbtree *t, node_t* node){
  if(node == t->nil) return;

  printf("%d, %s, parent = %d\n", node->key, node->color == RBTREE_BLACK ? "BLACK" : "RED", node->parent->key);
  print_tree(t, node->left);
  print_tree(t, node->right);
}
// int main() {
//   rbtree *t = new_rbtree();
//   //10, 5, 8, 34, 67, 23, 156, 24, 2, 12
//   rbtree_insert(t, 10);
//   rbtree_insert(t, 5);
//   rbtree_insert(t, 8);
//   rbtree_insert(t, 34);
//   rbtree_insert(t, 67);
//   rbtree_insert(t, 23);
//   rbtree_insert(t, 156);
//   rbtree_insert(t, 24);
//   rbtree_insert(t, 2);
//   rbtree_insert(t, 12);

//   print_tree(t, t->root);

//   return 0;
// }