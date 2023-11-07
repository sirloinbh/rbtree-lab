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

/*
 * RB트리를 왼쪽으로 회전하는 함수
 * 기준 노드의 오른쪽 자식의 왼쪽 서브 트리는 기준 노드의 오른쪽 서브트리로 변환
 * 기준 노드의 오른쪽 자식이 기준 노드의 위치를 차지
 * 기준 노드는 새로운 부모 노드의 왼쪽 자식으로 변환
 */
void left_rotate(rbtree *t, node_t *x) {
    node_t *y = x->right; // y는 x의 오른쪽 자식입니다.
    x->right = y->left;   // y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 이동합니다.

    if (y->left != t->nil) { // y의 왼쪽 서브트리가 nil이 아니면, 그 부모를 x로 설정합니다.
        y->left->parent = x;
    }

    y->parent = x->parent; // y의 부모를 x의 부모로 설정합니다.

    if (x->parent == t->nil) { // x가 루트인 경우
        t->root = y;
    } else if (x == x->parent->left) { // x가 부모의 왼쪽 자식인 경우
        x->parent->left = y;
    } else { // x가 부모의 오른쪽 자식인 경우
        x->parent->right = y;
    }

    y->left = x;  // y의 왼쪽 자식을 x로 설정합니다.
    x->parent = y; // x의 부모를 y로 설정합니다.
}
/*
 * RB트리를 오른쪽으로 회전하는 함수
 * 기준 노드의 왼쪽 자식의 오른쪽 서브 트리는 기준 노드의 왼쪽 서브트리로 변환
 * 기준 노드의 왼쪽 자식이 기준 노드의 위치를 차지
 * 기준 노드는 새로운 부모 노드의 오른쪽 자식으로 변환
 */
void right_rotate(rbtree *t, node_t *y) {
    node_t *x = y->left; // x는 y의 왼쪽 자식입니다.
    y->left = x->right;  // x의 오른쪽 서브트리를 y의 왼쪽 서브트리로 이동합니다.

    if (x->right != t->nil) { // x의 오른쪽 서브트리가 nil이 아니면, 그 부모를 y로 설정합니다.
        x->right->parent = y;
    }

    x->parent = y->parent; // x의 부모를 y의 부모로 설정합니다.

    if (y->parent == t->nil) { // y가 루트인 경우
        t->root = x;
    } else if (y == y->parent->right) { // y가 부모의 오른쪽 자식인 경우
        y->parent->right = x;
    } else { // y가 부모의 왼쪽 자식인 경우
        y->parent->left = x;
    }

    x->right = y; // x의 오른쪽 자식을 y로 설정합니다.
    y->parent = x; // y의 부모를 x로 설정합니다.
}
/*
 * RB트리의 균형을 맞추는 함수
 * Case 1: 새 노드의 삼촌 노드가 빨간색인 경우 : 
 * 부모와 삼촌을 검은색으로, 할아버지를 빨간색으로 변경하고, 할아버지 노드에 대해 재귀적으로 수행
 * Case 2: 새 노드의 삼촌이 검은색이고, 새 노드가 부모의 오른쪽 자식인 경우 :
 * 부모 노드를 기준으로 왼쪽 회전을 수행하고, 이 경우를 Case 3로 전환
 * Case 3: 새 노드의 삼촌이 검은색이고, 새 노드의 부모가 왼쪽 자식인 경우 :
 * 부모를 검은색으로, 할아버지를 빨간색으로 변경하고, 할아버지를 기준으로 오른쪽 회전을 수행
 */
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z != t->root && z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right; // 삼촌 노드
      if (y->color == RBTREE_RED) {
        // Case 1: 삼촌이 빨간색
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          // Case 2: 삼촌이 검은색이고 새 노드가 오른쪽 자식
          z = z->parent;
          left_rotate(t, z);
        }
        // Case 3: 삼촌이 검은색이고 새 노드가 왼쪽 자식
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    } else {
      // 삼촌 노드가 왼쪽에 있을 경우를 위한 대칭적인 처리
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        // Case 1
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          // Case 2
          z = z->parent;
          right_rotate(t, z);
        }
        // Case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
    t->root->color = RBTREE_BLACK; // 루트는 항상 검은색이어야 함
  }
}

/*
 * RB-Tree에 새로운 키를 삽입하는 함수.
 * 이진 탐색 트리의 규칙에 맞게 키를 삽입.
 * 삽입 후, 레드-블랙 트리의 규칙을 유지하도록 수정합니다.
 * @param t RB-Tree의 포인터입니다.
 * @param key 삽입할 키입니다.
 * @return 수정된 트리의 루트 노드를 반환합니다.
 */
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 새 노드를 위한 메모리 할당
  node_t *z = (node_t *)malloc(sizeof(node_t));
  if (z == NULL) {
    // 메모리 할당 실패시 NULL 반환
    return NULL;
  }
  
  // 새 노드를 키로 초기화하고 빨간색으로 설정
  z->key = key;
  z->color = RBTREE_RED;
  z->left = z->right = z->parent = t->nil;

  // 삽입 위치를 찾기 위한 포인터
  node_t *y = t->nil; // 새 노드의 부모가 될 포인터
  node_t *x = t->root; // 트리를 순회하는 이터레이터

  // 표준 이진 탐색 트리 삽입
  while (x != t->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  // 새 노드의 부모 설정
  z->parent = y;

  // 새 노드가 부모의 왼쪽 자식인지 오른쪽 자식인지 결정
  if (y == t->nil) {
    // 트리가 비어있으면 새 노드가 루트가 됨
    t->root = z;
  } else if (z->key < y->key) {
    // 새 키가 부모 키보다 작으면 왼쪽 자식
    y->left = z;
  } else {
    // 새 키가 부모 키보다 크면 오른쪽 자식
    y->right = z;
  }

  // 레드-블랙 트리의 속성을 위반했을 경우 수정하는 함수 호출
  rbtree_insert_fixup(t, z);

  // 트리가 재조정되었을 수 있으므로 트리의 루트 반환
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
