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
  node_t* prev;
  while(cur != t->nil){
    prev = cur;
    cur = cur->left;
  }
  return prev;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  node_t *prev;
  while(cur != t->nil){
    prev = cur;
    cur = cur->right;
  }

  return prev;
}
// for deletion procedure
//---------------------------------------------------------
node_t* tree_minimun(rbtree* t, node_t* node){
  node_t* cur = rbtree_find(t, node->key);

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

int rbtree_erase(rbtree *t, node_t *p) { //화가나서 gpt한테 물어봄
  node_t *y = p;
  // color_t y_color = y->color;
  node_t *x;
  if(p->left == t->nil){
    x = p->right;
    rb_transplant(t, p, p->right);
  }
  else if(p->right == t->nil){
    x = p->right;
    rb_transplant(t, p, p->left);
  }
  else{
    y = tree_minimun(t, p->right);
    // y_color = y->color;
    x = y->right;
    if(y != p->right){
      rb_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;  
    }
    else 
      x->parent = y;

    rb_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if(y->color == RBTREE_BLACK)
    rb_deletion_fixup(t, x);
  // TODO: implement erase
  return 0;
}

void rb_deletion_fixup(rbtree *t, node_t* x){
  node_t* sibling;
  // if(x->parent == t->nil){
  //   x = t->root;
  // }
  if(x == t->root){
    return;
  }
  // doubly node is left
  if(x == x->parent->left){
    sibling = x->parent->right;
    // case1, sibling is red
    // change color and rotate
    if(sibling->color == RBTREE_RED){
      sibling->color = RBTREE_BLACK;
      // sibling->parent->color = RBTREE_RED;
      x->parent->color = RBTREE_RED;
      // left_rotate(t, sibling->parent);
      left_rotate(t, x->parent);
      sibling = x->parent->right;
    }
    // case2, sibling is black
    if(sibling->left->color == RBTREE_BLACK 
      && sibling->right->color == RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        //----------------------------------
        x = x->parent;
        //----------------------------------
      }
    else{
      if(sibling->right->color == RBTREE_BLACK){
        sibling->left->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        // right_rotate(t, sibling->parent);
        right_rotate(t, sibling);
        sibling = x->parent->right;
      }
      sibling->color = x->parent->color;
      x->right->color = RBTREE_BLACK;
      sibling->right->color = RBTREE_BLACK;
      left_rotate(t, x->parent);
      x = t->root;
    }
  }
  
  // doubly node is right
  else {
    sibling = x->parent->left;
    // case1, sibling is red
    // change color and rotate
    if(sibling->color == RBTREE_RED){
      sibling->color = RBTREE_BLACK;
      // sibling->parent->color = RBTREE_RED;
      x->parent->color = RBTREE_RED;
      // left_rotate(t, sibling->parent);
      right_rotate(t, x->parent);
      sibling = x->parent->left;
    }
    // case2, sibling is black
    if(sibling->right->color == RBTREE_BLACK 
      && sibling->left->color == RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        //----------------------------------
        x = x->parent;
        //----------------------------------
      }
    else{
      if(sibling->left->color == RBTREE_BLACK){
        sibling->right->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        // right_rotate(t, sibling->parent);
        left_rotate(t, sibling);
        sibling = x->parent->left;
      }
      sibling->color = x->parent->color;
      x->left->color = RBTREE_BLACK;
      sibling->left->color = RBTREE_BLACK;
      right_rotate(t, x->parent);
      x = t->root;
    }
  }

  x->color = RBTREE_BLACK;
}
//node_t* sibling;
// if(x->parent == t->nil){
//   x = t->root;
// }
// // printf("delete_node = %d\n",x->key);
// if(x == t->root){
//   return;
// }
// // doubly node is left
// if(x == x->parent->left){
//   sibling = x->parent->right;
//   // case1, sibling is red
//   // change color and rotate
//   if(sibling->color == RBTREE_RED){
//     sibling->color = RBTREE_BLACK;
//     // sibling->parent->color = RBTREE_RED;
//     x->parent->color = RBTREE_RED;
//     // left_rotate(t, sibling->parent);
//     left_rotate(t, x->parent);
//     sibling = x->parent->right;
//   }
//   // case2, sibling is black
//   if(sibling->color == RBTREE_BLACK){
//     if(sibling->left->color == RBTREE_BLACK 
//       && sibling->right->color == RBTREE_BLACK){
//         sibling->color = RBTREE_RED;
//         //----
//         x = x->parent;
//         //----
//         rb_deletion_fixup(t, sibling->parent);
//       }
//     else{
//       if(sibling->right->color == RBTREE_RED){
//         sibling->left->color = RBTREE_BLACK;
//         sibling->color = RBTREE_RED;
//         // right_rotate(t, sibling->parent);
//         right_rotate(t, sibling);
//         sibling = x->parent->right;
//       }
//       sibling->color = x->parent->color;
//       x->right->color = RBTREE_BLACK;
//       sibling->right->color = RBTREE_BLACK;
//       left_rotate(t, x->parent);
//     }
//   }
//   // else {
//   //   sibling->color = RBTREE_BLACK;
//   //   sibling->parent->color = RBTREE_RED;
//   //   left_rotate(t, sibling->parent);
//   // }
// }
// // doubly node is right
// else {
//   sibling = x->parent->left;

//   if (sibling->color == RBTREE_RED) {
//     sibling->color = RBTREE_BLACK;
//     x->parent->color = RBTREE_RED;
//     right_rotate(t, x->parent);
//     sibling = x->parent->left;
//   }

//   if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) {
//     sibling->color = RBTREE_RED;
//     rb_deletion_fixup(t, x->parent);  // 재귀 호출
//   }
//   else {
//     if (sibling->left->color == RBTREE_BLACK) {
//       sibling->right->color = RBTREE_BLACK;
//       sibling->color = RBTREE_RED;
//       left_rotate(t, sibling);
//       sibling = x->parent->left;
//     }

//     sibling->color = x->parent->color;
//     x->parent->color = RBTREE_BLACK;
//     sibling->left->color = RBTREE_BLACK;
//     right_rotate(t, x->parent);
//   }
// }

// x->color = RBTREE_BLACK;
//---------------------------------------------------------
//node_t* sibiling;
  // printf("delete_node = %d\n",x->key);
//   if(x == t->root)
//     return;
//   if(x->color == RBTREE_RED)
//     return;
//   if(x == x->parent->left){
//     sibiling = x->parent->right;
    
//     if(sibiling->color == RBTREE_BLACK){
//       if(sibiling->left->color == RBTREE_BLACK 
//         && sibiling->right->color == RBTREE_BLACK){
//           sibiling->color = RBTREE_RED;
//           rb_deletion_fixup(t, sibiling->parent);
//         }
//       else{
//         if(sibiling->left->color == RBTREE_RED){
//           sibiling->color = RBTREE_RED;
//           sibiling->left->color = RBTREE_BLACK;
//           sibiling = sibiling->left;
//           right_rotate(t, sibiling->parent);
//         }
//         sibiling->color = sibiling->parent->color;
//         sibiling->right->color = RBTREE_BLACK;
//         sibiling->parent->color = RBTREE_BLACK;
//         left_rotate(t, sibiling->parent);
//       }
//     }
//     else {
//       sibiling->color = RBTREE_BLACK;
//       sibiling->parent->color = RBTREE_RED;
//       left_rotate(t, sibiling->parent);
//       rb_deletion_fixup(t, x);
//     }
//   }
//   else{
//     sibiling = x->parent->left;
//     if(sibiling->color == RBTREE_BLACK){
//       if(sibiling->left->color == RBTREE_BLACK 
//         && sibiling->right->color == RBTREE_BLACK){
//           sibiling->color = RBTREE_RED;
//           rb_deletion_fixup(t, sibiling->parent);
//         }
//       else{
//         if(sibiling->right->color == RBTREE_RED){
//           sibiling->color = RBTREE_RED;
//           sibiling->right->color = RBTREE_BLACK;
//           sibiling = sibiling->right;
//           left_rotate(t, sibiling->parent);
//         }
//         sibiling->color = sibiling->parent->color;
//         sibiling->left->color = RBTREE_BLACK;
//         sibiling->parent->color = RBTREE_BLACK;
//         right_rotate(t, sibiling->parent);
//       }
//     }
  
//     if(sibiling->color == RBTREE_RED){
//       sibiling->color = RBTREE_BLACK;
//       sibiling->parent->color = RBTREE_RED;
//       right_rotate(t, sibiling->parent);
//       rb_deletion_fixup(t, x);
//     }
//   }
// }

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
//   node_t * rm_node = rbtree_find(t, 24);
//   // printf("find_item = %d ", rm_node->key);

//   rbtree_erase(t, rm_node);

//   print_tree(t, t->root);

//   return 0;
// }

// int rbtree_erase(rbtree *t, node_t *p) {
  // y는 temp 역할
  // node_t* y = p;
  // node_t* saved_color = y;
  // // 2 child -> change minimun node in right subtree and then deletion
  // node_t* x;
  // if(p->left != t->nil && p->right != t->nil){
    
  //   y = tree_minimun(t, p->right);
  //   p->color = y->color;
  //   y->color = saved_color->color;
  //   rb_transplant(t, p, y);
  // }
  // // 0 child -> just deletion
  // if(p->left == t->nil && p->right == t->nil){
  //   x = p;
  //   rb_transplant(t, p, p->right);
  //   y->color = saved_color->color;
  // }
  //   // 1 child -> current node deletion and insertion child node
  // else if(p->left != t->nil){
  //   x = p->left;
  //   rb_transplant(t, p, p->left);
  // }
  // else{
  //   x = p->right;
  //   rb_transplant(t, p, p->right);
  // }

  // if(saved_color->color == RBTREE_RED){
  //   return 1;
  // }
  // else{
  //   rb_deletion_fixup();
  //   return 1;
  // }
  


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
// }