#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialinewNodee struct if needed
  node_t *nilNode = (node_t*)calloc(1, sizeof(node_t));
  nilNode -> color = RBTREE_BLACK;
  p -> nil = nilNode;
  p -> root = p -> nil;
  return p;
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x -> right;
  x -> right = y -> left;
  if (y -> left != t -> nil){ //nil이어도 해야 되는 거 아닌가??
    y -> left -> parent = x;
  }
  y -> parent = x -> parent;
  if (x -> parent == t -> nil){
    t -> root = y;
  } else if (x == x -> parent -> left){
    x -> parent -> left = y;
  } else {
    x -> parent -> right = y;
  }
  y -> left = x;
  x -> parent = y;
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x -> left;
  x -> left = y -> right;
  if (y -> right != t -> nil){
    y -> right -> parent = x;
  }
  y -> parent = x -> parent;
  if (x -> parent -> left == x){
    x -> parent -> left = y;
  } else {
    x -> parent -> right = y;
  }
  y -> right = x;
  x -> parent = y;
}


void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
  t -> root = NULL;
  t -> nil = NULL;
  t = NULL;
}

void rbtree_insert_fixup(rbtree *t, node_t *newNode){
  while (newNode -> parent -> color == RBTREE_RED){
    if (newNode -> parent == newNode -> parent -> parent -> left){
      node_t *y = newNode -> parent -> parent -> right;
      if (y -> color == RBTREE_RED){
        newNode -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        newNode -> parent -> parent -> color = RBTREE_RED;
        newNode = newNode -> parent -> parent;
      }
      else if (newNode == newNode -> parent -> right){
        newNode = newNode -> parent;
        left_rotate(t, newNode);
        newNode -> parent -> color = RBTREE_BLACK;
        newNode -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, newNode->parent->parent);
      }
    else { // 오른쪽에 몰려있을 때
      node_t *y = newNode->parent->parent->left;
      if (y -> color == RBTREE_RED){
        newNode -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        newNode -> parent -> parent -> color = RBTREE_RED;
        newNode = newNode -> parent -> parent;
      }
      else if (newNode == newNode -> parent -> left){
        newNode = newNode -> parent;
        right_rotate(t, newNode);
        newNode -> parent -> color = RBTREE_BLACK;
        newNode -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, newNode->parent->parent);
      } 
    }
    }
  }
  t -> root -> color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *newNode = (node_t*)calloc(1, sizeof(node_t));
  newNode -> key = key;
  node_t *y = t -> nil;
  node_t *x = t -> root;
  while(x != t -> nil){
    y = x;
    if (newNode->key < x->key){
      x = x -> left;
    } else {
      x = x -> right;
    }
  }
  newNode -> parent = y;
  if (y == t -> nil){
    t -> root = newNode;
  } else if (newNode->key < y->key){
    y -> left = newNode;
  } else{
    y -> right = newNode;
  }
  newNode -> left = t -> nil;
  newNode -> right = t -> nil;
  newNode -> color = RBTREE_RED;
  rbtree_insert_fixup(t, newNode);
  // return t->root;
  return t -> root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *temp = t -> root;
  while (temp != t -> nil && temp -> key != key){
    if (temp -> key > key){
      temp = temp -> left;
    } else {
      temp = temp -> right;
    }
  }
  if (temp -> key == key){
    return temp;
  } else{
    return NULL;
  }
  // return t->root; // 어떠케 리턴...
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t -> root;
  node_t *p = t -> nil;
  while (temp != t -> nil){
    p = temp;
    temp = temp -> left;
  }
  return p;
  // return t->root;
}

node_t *rbtree_subtree_min(rbtree *t){
  node_t *temp = t -> root;
  node_t *p = t->nil;
  while (temp != t -> nil){
    p = temp;
    temp = temp -> left;
  }
  return p;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t -> root;
  node_t *p = t -> nil;
  while (temp != t -> nil){
    p = temp;
    temp = temp -> right;
  }
  return p;
  // return t->root;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if (u -> parent == t -> nil){
    t -> root = v;
  } else if (u == u -> parent -> left){
    u -> parent -> left = v;
  } else {
    u -> parent -> right = v;
  }
  v -> parent = u -> parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *x){
  while (x != t -> root && x -> color == RBTREE_BLACK){
    if (x == x -> parent -> left){
      node_t *w = x -> parent -> right; // uncle
      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent);
        w = x -> parent -> right;
      }
      if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        x = x -> parent;
      } else if (w -> right -> color == RBTREE_BLACK){
        w -> left -> color = RBTREE_BLACK;
        w -> color = RBTREE_RED;
        right_rotate(t, w);
        w = x -> parent -> right;
        
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t -> root;
      }
    } else { //if x == x->parent=>left
      node_t *w = x -> parent -> left; // uncle
      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, x -> parent);
        w = x -> parent -> left;
      }
      if (w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        x = x -> parent;
      } else if (w -> left -> color == RBTREE_BLACK){
        w -> right -> color = RBTREE_BLACK;
        w -> color = RBTREE_RED;
        left_rotate(t, w);
        w = x -> parent -> left;

        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> left -> color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t -> root;
      }
    }
  }
  x -> color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p;
  color_t y_original_color = y -> color;
  node_t *x = NULL;
  if (p -> left == t -> nil){
    x = p -> right;
    rbtree_transplant(t, p, p -> right);
  } else if (p -> right == t -> nil){
    x = p -> left;
    rbtree_transplant(t, p, p -> left);
  } else{
    //subtree의 min값 찾기
    rbtree *s = new_rbtree();
    s -> root = p -> right;
    s -> nil = t -> nil;
    y = rbtree_subtree_min(s);
    //여기까지 min값 찾기였음
    y_original_color = y -> color;
    x = y -> right;
    if (y -> parent == p){
      x -> parent = y;
    } else {
      rbtree_transplant(t, y, y->right);
      y -> right = p -> right;
      y -> right -> parent = y;
    }
    rbtree_transplant(t, p, y);
    y -> left = p -> left;
    y -> left -> parent = y;
    y -> color = p -> color;
  }
  if (y_original_color == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

  return 0;
}
