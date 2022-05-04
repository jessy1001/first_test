#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t*)calloc(1, sizeof(node_t));
  nilNode -> color = RBTREE_BLACK;
  p -> nil = nilNode;
  p -> root = p -> nil;
  return p;
} //문제없음

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x -> right;
  x -> right = y -> left;
  if (y -> left != t -> nil){ //nil이어도 해야 되는 거 아닌가??아니 안해도됨
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
  return;
} //문제없음

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x -> left;
  x -> left = y -> right;
  if (y -> right != t -> nil){
    y -> right -> parent = x;
  }
  y -> parent = x -> parent;
  if (x -> parent == t -> nil){
    t -> root = y;
  } else if (x == x -> parent -> right){
    x -> parent -> right = y;
  } else {
    x -> parent -> left = y;
  }
  y -> right = x;
  x -> parent = y;
  return;
} //문제없음

void del_node(rbtree *t, node_t *n){
  if (n != t -> nil){
    del_node(t, n -> left);
    del_node(t, n -> right);
    free(n);
    n = NULL;
  }
} //문제없음

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  del_node(t, t -> root);
  free(t->nil);
  t -> nil = NULL;
  free(t);
  t = NULL;
} //문제없음

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
      else {
        if (newNode == newNode -> parent -> right){
          newNode = newNode -> parent;
          left_rotate(t, newNode);
          }
        newNode -> parent -> color = RBTREE_BLACK;
        newNode -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, newNode->parent->parent);
      }
    }
    else { // 오른쪽에 몰려있을 때
      node_t *y = newNode->parent->parent->left;
      if (y -> color == RBTREE_RED){
        newNode -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        newNode -> parent -> parent -> color = RBTREE_RED;
        newNode = newNode -> parent -> parent;
      }
      else {
        if (newNode == newNode -> parent -> left){
          newNode = newNode -> parent;
          right_rotate(t, newNode);
          }
        newNode -> parent -> color = RBTREE_BLACK;
        newNode -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, newNode->parent->parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;
  return;
} //문제없음


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t*)calloc(1, sizeof(node_t));
  z -> key = key;
  node_t *y = t -> nil;
  node_t *x = t -> root;
  while(x != t -> nil){ //빈자리 찾을 때까지
    y = x;
    if (key < x->key){
      x = x -> left;
    } else { //같은 키값 있을 때도 right에 넣어주면 됨
      x = x -> right;
    }
  }
  z -> parent = y; 
  if (y == t -> nil){
    t -> root = z;
  } else if (key < y->key){
    y -> left = z;
  } else{
    y -> right = z;
  }
  z -> left = t -> nil;
  z -> right = t -> nil;
  z -> color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return t -> root;
} //문제없음


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
  if (temp == t -> nil){
    return NULL;
  } 
  return temp;
  }
  // 문제없음

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t -> root;
  while (temp -> left != t -> nil){
    temp = temp -> left;
  }
  return temp;
  // 문제없음
}

node_t *rbtree_subtree_min(rbtree *t){
  node_t *temp = t -> root;
  while (temp -> left!= t -> nil){
    temp = temp -> left;
  }
  return temp;
} //문제없음...?

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t -> root;
  while (temp -> right != t -> nil){
    temp = temp -> right;
  }
  return temp;
} //문제없음

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if (u -> parent == t -> nil){
    t -> root = v;
  } else if (u == u -> parent -> left){
    u -> parent -> left = v;
  } else {
    u -> parent -> right = v;
  }
  v -> parent = u -> parent;
  return;
} //윤찬이랑 똑같음

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
      } else {
        if (w -> right -> color == RBTREE_BLACK){
          w -> left -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          right_rotate(t, w);
          w = x -> parent -> right;
        }
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t -> root;
      
      }
    }
    else { //if x == x->parent=>right
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
      } else {
        if (w -> left -> color == RBTREE_BLACK){
          w -> right -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          left_rotate(t, w);
          w = x -> parent -> left;
        }
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> left -> color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t -> root;
      }
    }
  }
  x -> color = RBTREE_BLACK;
  return;
} //문제없음

node_t *rbtree_successor(rbtree *t, node_t *x) {
  node_t *y = x;              
  while (y->left != t->nil) { 
    y = y->left;
  }
  return y;                   
} //문제없음


int rbtree_erase(rbtree *t, node_t *z) {
  if (z == t -> nil){
    return 0;
  } //이부분이 책에는 없음
  node_t *y = z; //색이 지워질 대상(z 자신 혹은 successor)
  color_t y_original_color = y -> color;
  node_t *x = NULL; //굳이 NULL로 초기화해야 할까? (색이 빠진 자리를 대신할 node)
  //왼쪽이 비었을 때
  if (z -> left == t -> nil){
    x = z -> right;
    rbtree_transplant(t, z, z -> right);
  } 
  //오른쪽이 비었을 때
  else if (z -> right == t -> nil){
    x = z -> left;
    rbtree_transplant(t, z, z -> left);
  }
  //자식이 둘일 때
  else{
    y = rbtree_successor(t, z -> right);
    y_original_color = y -> color;
    x = y -> right;
    if (y -> parent == z){
      x -> parent = y;
    } else {
      rbtree_transplant(t, y, y->right);
      y -> right = z -> right;
      y -> right -> parent = y;
    }
    rbtree_transplant(t, z, y);
    y -> left = z -> left;
    y -> left -> parent = y;
    y -> color = z -> color;
  } 
  if (y_original_color == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }
  free(z); //이게 있었네...
  z = NULL;
  return 0;
} //문제없음


int rbtree_inorder(node_t *root, node_t *nil, key_t *arr, const size_t n, int index) {
  if (root == nil || index > n) return index;   //arr들어가기 전에 입구컷 당해서 ++못해서 index > n 될 일이 없지 않나?                              
  
  index = rbtree_inorder(root->left, nil, arr, n, index);
  if(index < n) {                                           
    arr[index++] = root->key;  //후위연산자!!! 마지막에 arr[n-1]에 넣고 다음 index는 n됨 -> 다음에 돌 때 arr에 안 들어감              //  
  }
  index = rbtree_inorder(root->right, nil, arr, n, index);  
  return index;        //우리가 index 여기까지 썼다 알려주는 용, 중위순회 끝날때까지 계속 return n함                                     
}

// 트리를 이용하여 오름차순 구현
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) { //arr라는 주소에 중위순회로 node_t들을 넣겠다
  
  if (t->root == t->nil) {
    return 0;
  }
  rbtree_inorder(t->root, t->nil, arr, n, 0); // 중위 순회
  return 0;
} 
