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
}

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
}

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
}

void del_node(rbtree *t, node_t *n){
  if (n != t -> nil){
    del_node(t, n -> left);
    del_node(t, n -> right);
    free(n);
    n = NULL;
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  del_node(t, t -> root);
  free(t->nil);
  t -> nil = NULL;
  free(t);
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
}

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
  if (temp == t -> nil){
    return NULL; //temp->key == key부터 하면 find(0) 했을 때 null 아니고 nil return 함
  }
  return temp;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t -> root;
  while (temp -> left != t -> nil){
    temp = temp -> left;
  }
  return temp;
  // return t->root;
}

node_t *rbtree_subtree_min(rbtree *t){
  node_t *temp = t -> root;
  while (temp -> left!= t -> nil){
    temp = temp -> left;
  }
  return temp;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t -> root;
  while (temp -> right != t -> nil){
    temp = temp -> right;
  }
  return temp;
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

void rbtree_erase_fixup(rbtree *t, node_t *x){ //이 주석달린 버전은 윤찬이꺼
    node_t *w;                                                                         
    while ((x != t->root) && (x->color == RBTREE_BLACK)) {                            // 삭제한 노드의 자식이 루트가 아니고 색깔이 BLACK일 때, 반복
        if (x == x->parent->left) {                                                   // 경우 1. 삭제한 노드의 자식이 삭제한 노드의 부모의 왼쪽 자식일 때
            w = x->parent->right;                                                       // w는 x의 오른쪽 형제
            if (w->color == RBTREE_RED) {                                               // w의 색깔이 RED일 때
                w->color = RBTREE_BLACK;                                                // w의 색깔이 BLACK으로 변경
                x->parent->color = RBTREE_RED;                                          // x의 부모의 색을 RED로 변경
                left_rotate(t, x->parent);                                            // x의 부모를 기준으로 좌회전
                w = x->parent->right;                                                   // w는 형제
            }                                                                           // 경우 1을 해결할 경우 경우 2, 3, 4중에 발생
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {  // 경우 2. w의 왼쪽 자식과 오른쪽 자식이 BLACK일 때
                w->color = RBTREE_RED;                                                  // w의 색깔을 RED로 변경 
                x = x->parent;                                                          // x는 x의 부모로 변경
            }  else {
                if (w->right->color == RBTREE_BLACK) {                                // 경우 3. 경우 2가 아니고, w의 오른쪽 색이 BLACK일 때
                    w->left->color = RBTREE_BLACK;                                      // w의 왼쪽 색이 BLACK으로 변경
                    w->color = RBTREE_RED;                                              // w의 색은 RED로 변경
                    right_rotate(t, w);                                               // w를 기준으로 우회전
                    w = x->parent->right;                                               // w는 x의 부모의 오른쪽 자식
                }
                w->color = x->parent->color;                                          // 경우 4. 경우 2가 아닐 때, w의 오른쪽 자식이 RED일 때
                x->parent->color = RBTREE_BLACK;                                        // x의 색은 x의 부모의 색을 BLACK으로 변경
                w->right->color = RBTREE_BLACK;                                         // w의 오른쪽 자식 색을 BLACK으로 변경
                left_rotate(t, x->parent);                                            // x의 부모를 기준으로 좌회전
                x = t->root;                                                            // x는 트리의 루트
            }
        } else {                                                                      // 삭제한 노드의 자식이 삭제한 노드의 부모의 오른쪽 자식일 때, 왼쪽일 때의 반대
            w = x->parent->left;
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotate(t, x->parent);
                w = x ->parent->left;
            }
            if (w->right ->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left -> color = RBTREE_BLACK;
                right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;                                                        // 트리의 루트를 BLACK으로 변경
    return;
}
//원래 내꺼
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
}

// int rbtree_erase(rbtree *t, node_t *p) {
//   node_t *x;                              // 노드 x
//   node_t *y = p;                          // y는 삭제할 노드
//   color_t y_color = y->color;             // y_color는 y의 색
//   if (p->left == t->nil) {                // 삭제할 노드의 왼쪽이 nil일때
//     x = p->right;                         // x는 삭제할 노드의 오른쪽
//     rbtree_transplant(t, p, p->right);    // 삭제할 노드의 부모와 삭제할 노드의 오른쪽을 연결 
//   } else if (p->right == t->nil) {        // 삭제할 노드의 오른쪽이 nill일때
//     x = p->left;                          // x는 삭제할 노드의 왼쪽
//     rbtree_transplant(t, p, p->left);     // 삭제할 노드의 부모와 삭제할 노드의 왼쪽을 연결
//   } else {                                // 삭제할 노드의 왼쪽, 오른쪽 자식이 둘다 있을 때
//     y = rbtree_successor(t, p->right);    // y는 직후 원소
//     y_color = y->color;                   // y_color는 직후 원소의 색
//     x = y->right;                         // x는 y의 오른쪽 자식
//     if (y->parent == p) {                 // y의 부모가 삭제할 노드일 때
//       x->parent = y;                      // x의 부모는 y
//     } else {                              // y의 부모가 삭제할 노드가 아닐 때
//       rbtree_transplant(t, y, y->right);  // y의 부모와 y의 오른쪽 자식을 연결
//       y->right = p->right;                // y의 오른쪽 자식은 삭제할 노드의 오른쪽 자식
//       y->right->parent = y;               // y의 오른쪽 자식의 부모는 y
//     }
//     rbtree_transplant(t, p, y);           // 삭제할 노드 부모와 y를 연결
//     y->left = p->left;                    // y의 왼쪽 자식은 삭제할 노드의 왼쪽 자식
//     y->left->parent = y;                  // y의 왼쪽 자식의 부모는 y
//     y->color = p->color;                  // y의 색은 삭제할 노드의 색
//   }
//   free(p);                                // 삭제한 노드가 가리키는 공간 삭제
//   p = NULL;                               // 할당 해제 후 삭제한 노드값을 NULL로 초기화
//   if (y_color == RBTREE_BLACK) {          // y_color가 BLACK 일 때, 즉 삭제한 노드의 색이 BLACK일 때(특성 5 위반)
//     rb_erase_fixup(t, x);                // 노드의 색을 바꿈
//   }
//   return 0;
// }

node_t *rbtree_successor(rbtree *t, node_t *x){
  node_t *y = x;
  while (y -> left != t -> nil){
    y = y -> left;
  }
  return y;
}


int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
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
  free(z); //이게 있었네...
  z = NULL;
  if (y_original_color == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }
  return 0;
} 


//subtree의 min값 찾기 -> 이렇게 하면 같은 tree가 아니라서 안됨...?
//     rbtree *s = new_rbtree();
//     s -> root = z -> right;
//     s -> nil = t -> nil;
//     y = rbtree_subtree_min(s);
//     //여기까지 min값 찾기였음

void subtree_to_array(const rbtree *t, key_t *arr, const size_t n, node_t *p, size_t *cnt){
    // if (p == t -> nil){
    //   return;
    // }
    // subtree_to_array(t, arr, n, p -> left, cnt);
    // if (cnt < n){
    //   arr[cnt]
    // }

  }

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  // 중위순회 함수
  


  // node_t *temp = t -> root;
  // inorder_array(t, arr, temp, 0);
  return 0;
}
