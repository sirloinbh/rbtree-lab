#include "rbtree.h"

#include <stdlib.h>
/*
 * 새로운 레드-블랙 트리를 할당하고 메모리를 0으로 초기화.
 * 메모리 할당에 실패하면 NULL을 반환합니다.
 * 센티넬(nil) 노드에 대한 메모리를 할당.
*/
rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    if (!p) {
        return NULL;
    }

    p->nil = (node_t *)calloc(1, sizeof(node_t));
    if (!p->nil) {
        free(p);
        return NULL;
    }
    p->nil->color = RBTREE_BLACK;
    p->nil->parent = p->nil;
    p->nil->left = p->nil;
    p->nil->right = p->nil;
    p->root = p->nil;

    return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

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
