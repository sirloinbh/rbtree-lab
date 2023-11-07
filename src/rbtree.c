#include "rbtree.h"

#include <stdlib.h>
/*
 * 새로운 RB트리를 생성하는 함수
 * 새로운 레드-블랙 트리를 할당하고 메모리를 0으로 초기화.
 * 메모리 할당에 실패하면 NULL을 반환합니다.
 * 센티넬(nil) 노드에 대한 메모리를 할당.
 */
rbtree *new_rbtree(void) {
    // rbtree 구조체에 대한 메모리를 할당
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    // 메모리 할당에 실패하면 NULL을 반환
    if (!p) {
        return NULL;
    }

    // 'nil'노드를 위한 메모리를 할당
    p->nil = (node_t *)calloc(1, sizeof(node_t));
    // 실패 시 트리 구조체의 메모리를 해제하고 NULL반환
    if (!p->nil) {
        free(p);
        return NULL;
    }
    // 'nil'노드를 초기화하고 검은색으로 설정
    p->nil->color = RBTREE_BLACK;

    // 'nil' 노드의 parent, left, right 포인터는 자기 자신을 가리키도록 설정
    p->nil->parent = p->nil;
    p->nil->left = p->nil;
    p->nil->right = p->nil;
    // 트리의 루트 노드를 'nil'노드로 초기화
    p->root = p->nil;

    return p;
}
/*
 * 각 노드를 재귀적으로 방문하여 메모리를 해제하는 도우미 함수
 */
void free_rbtree_nodes(node_t *node, node_t *nil) {
    if (node == nil) {
        return; // nil 센티널에 도달했으면 반환
    }

    // 왼쪽 서브트리를 재귀적으로 해제
    free_rbtree_nodes(node->left, nil);
    // 오른쪽 서브트리를 재귀적으로 해제
    free_rbtree_nodes(node->right, nil);
    // 현재 노드를 해제
    free(node);
}

/*
 * RB트리를 해제하는 함수
 * 모든 노드들의 메모리를 해제
 * 마지막으로 'nil'노드의 메모리를 해제
 * 트리 자체의 메모리를 해제
 */
void delete_rbtree(rbtree *t) {
    // nil 센티널을 먼저 확인
    if (t->nil != NULL) {
        // 모든 노드를 해제
        free_rbtree_nodes(t->root, t->nil);
        // nil 센티널 노드도 해제
        free(t->nil);
    }

    // RB-트리 구조체를 해제
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
