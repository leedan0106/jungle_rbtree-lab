#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->root = nil;
  p->nil = nil;
  p->root->parent = nil;

  return p;
}

void delete_rbtree_recur(rbtree *t, node_t *cur) {
  if(cur->left == t->nil && cur->right == t->nil) {
    free(cur);
    return;
  }

  if(cur->left != t->nil) {
    delete_rbtree_recur(t, cur->left);
  }
  if(cur->right != t->nil) {
    delete_rbtree_recur(t, cur->right);
  }
  free(cur);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if(t->root != t->nil) {
    delete_rbtree_recur(t, t->root);
  }
  free(t->nil);  
  free(t);
}

void left_rotation(rbtree *t, node_t *n) {
  node_t *right = n->right;
  n->right = right->left;
  
  if(right->left != t->nil) {
    right->left->parent = n;
  }
  right->parent = n->parent;
  if(n->parent == t->nil) {
    t->root = right;
  }else if(n == n->parent->left) {
    n->parent->left = right;
  }else{
    n->parent->right = right;
  }
  right->left = n;
  n->parent = right;
}

void right_rotation(rbtree *t, node_t *n) {

  node_t *left = n->left;
  n->left = left->right;
  
  if(left->right != t->nil) {
    left->right->parent = n;
  }
  left->parent = n->parent;
  if(n->parent == t->nil) {
    t->root = left;
  }else if(n == n->parent->right) {
    n->parent->right = left;
  }else{
    n->parent->left = left;
  }
  left->right = n;
  n->parent = left;

}

void insert_fixup(rbtree *t, node_t *n) {
  if(t->root == n) {
    t->root->color = RBTREE_BLACK;
    return;
  }

  node_t *grand;
  node_t *parent;
  node_t *uncle;

  while(n->parent->color == RBTREE_RED) {
    grand = n->parent->parent;
    parent = n->parent;
    if(n->parent == grand->left){
      uncle = grand->right;
      if(uncle->color == RBTREE_RED) { // CASE 1
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grand->color = RBTREE_RED;
        n = grand;
        if(n == t->root) {
          n->color = RBTREE_BLACK;
          break;
        }
      }else{
        if(n == parent->right){ // CASE 2
          n = n->parent;
          left_rotation(t, n);
        }
        // CASE 3
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        right_rotation(t, n->parent->parent);
      }
    }else{
      uncle = grand->left;
      if(uncle->color == RBTREE_RED) { // CASE 1
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grand->color = RBTREE_RED;
        n = grand;
        if(n == t->root) {
          n->color = RBTREE_BLACK;
          break;
        }
      }else{
        if(n == parent->left){ // CASE 2
          n = parent;
          right_rotation(t, n);
        }
        
        // CASE 3
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        left_rotation(t, n->parent->parent);
      }
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *current = t->root;
  node_t *parent = t->root->parent;
  while(current != t->nil) {
    if(key < current->key){
      parent = current;
      current = current->left;
    } 
    else{
      parent = current;      
      current = current->right;
    }
  }

  node_t *new_node = (node_t*)malloc(sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->parent = parent;
  new_node->left = new_node->right = t->nil;
  if(parent == t->nil) { // root노드
    t->root = new_node;
  }
  else if(key < parent->key) { 
    parent->left = new_node;
  }else{
    parent->right = new_node;
  }

  if(parent->color == RBTREE_RED || new_node == t->root) {
    insert_fixup(t, new_node);
  }

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *current = t->root;
  while(current != t->nil){
    if(current->key == key)
      return current;    
    if(key < current->key) 
      current = current->left;
    else
      current = current->right;    
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->left != t->nil) {
    current = current->left;
  }
  return current;
  // return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->right != t->nil) {
    current = current->right;
  }
  return current;
  // return t->root;
}

void delete_fixup(rbtree *t, node_t *n) {

  while(n->parent != t->nil && n->color == RBTREE_BLACK) {
    if(n == n->parent->left) {
      node_t *sibling = n->parent->right;
      if(sibling->color == RBTREE_RED) { // CASE 1
        sibling->color = RBTREE_BLACK;
        n->parent->color = RBTREE_RED;
        left_rotation(t, n->parent);
        sibling = n->parent->right;
      }

      if(sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) { // CASE 2
        sibling->color = RBTREE_RED;
        n = n->parent;
      }else{
        if(sibling->right->color == RBTREE_BLACK) { // CASE 3
          sibling->color = RBTREE_RED;
          sibling->left->color = RBTREE_BLACK;
          right_rotation(t, sibling);
          sibling = n->parent->right;
        }
        // CASE 4
        sibling->color = n->parent->color;
        sibling->right->color = RBTREE_BLACK;
        n->parent->color = RBTREE_BLACK;
        left_rotation(t, n->parent) ;
        n = t->root;
      }
    }else{
      node_t *sibling = n->parent->left;
      if(sibling->color == RBTREE_RED) { // CASE 1
        sibling->color = RBTREE_BLACK;
        n->parent->color = RBTREE_RED;
        right_rotation(t, n->parent);
        sibling = n->parent->left;
      }

      if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) { // CASE 2
        sibling->color = RBTREE_RED;
        n = n->parent;
      }else{
        if(sibling->left->color == RBTREE_BLACK) { // CASE 3
          sibling->color = RBTREE_RED;
          sibling->right->color = RBTREE_BLACK;
          left_rotation(t, sibling);
          sibling = n->parent->left;
        }
        // CASE 4
        sibling->color = n->parent->color;
        sibling->left->color = RBTREE_BLACK;
        n->parent->color = RBTREE_BLACK;
        right_rotation(t, n->parent);
        n = t->root;
     }
    }
  }

  n->color = RBTREE_BLACK;
  // // n이 root node인 경우
  // if(n->parent == t->nil) {
  //   n->color = RBTREE_BLACK;
  //   return;
  // }

  // // n의 color가 red인 경우 (red and black)
  // if(n->color == RBTREE_RED) {
  //   n->color = RBTREE_BLACK;
  //   return;
  // }
  
}

void change_node(rbtree *t, node_t *a, node_t *b) {
  if(a->parent == t->nil) {
    t->root = b;
  }else if(a == a->parent->left) {
    a->parent->left = b;
  }else{
    a->parent->right = b;
  }
  b->parent = a->parent;
  // free(a);
}

node_t *find_successor(rbtree *t, node_t *n) {
  while(n->left != t->nil) {
    n = n->left;
  }

  return n;
}

int rbtree_erase(rbtree *t, node_t *p) {

  // TODO: implement erase
  color_t color = p->color;
  node_t *delete_node = p;
  node_t *replace_node;

  if(p->left == t->nil) {
    replace_node = p->right;
    change_node(t, p, p->right);
  }else if(p->right == t->nil) {
    replace_node = p->left;
    change_node(t, p, p->left);
  }else{
    delete_node = find_successor(t, p->right);
    color = delete_node->color;
    replace_node = delete_node->right; // successor은 왼쪽 노드가 없으므로 대체 노드는 오른쪽 노드가 된다.

    change_node(t, delete_node, replace_node);
    p->key = delete_node->key; // p와 delete_node의 Key값만 바꿔주고, delete_node를 삭제한다.
    p = delete_node; // delete_node를 free하기 위해서 p에 delete_node 넣기

    // clsr책의 pseudocode 방식(p와 delete_node를 아예 바꾸는 방식)
    // if(delete_node != p->right){
    //   change_node(t, delete_node, delete_node->right);
    //   delete_node->right = p->right;
    //   delete_node->right->parent = delete_node;
    // }else{ // replace_node가 nil노드이기 때문에 parent연결해 주어야 함
    //   replace_node->parent = delete_node;
    // }
    // change_node(t, p, delete_node);
    // delete_node->left = p->left;
    // delete_node->left->parent = delete_node;
    // delete_node->color = p->color;
  }

  if(color == RBTREE_BLACK) {
    delete_fixup(t, replace_node);
  }
  free(p);

  return 0;
}

int traverse(const rbtree *t, node_t *cur, key_t *arr, const size_t n, int i) {
  if(i >= n) 
    return;
  if(cur->left != t->nil) {
    i = traverse(t, cur->left, arr, n, i);
  }
  *(arr+i) = cur->key;
  if(cur->right != t->nil) {
    i = traverse(t, cur->right, arr, n, i+1);
    return i;
  }
  return i+1;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  traverse(t, t->root, arr, n, 0);
  return 0;
}
