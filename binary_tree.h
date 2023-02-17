#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#define DOUBLE_LINEAR_LIST_DATA_SIZE  128
#define TIRE_MAX_NODE  6

struct binary_tree;
struct trie;

typedef struct binary_tree binary_tree_t, *binary_tree_p;
typedef struct trie trie_t, *trie_p;

struct binary_tree {
    uint8_t val;
    uint8_t *data;

    binary_tree_p left;
    binary_tree_p right;
} ;

struct trie {
    uint8_t val;
    uint8_t letter;
    EXIST_STATE is_exist;

    uint8_t sets_count;               // 标志子节点集合的 使用情况
    uint8_t sets_level;               // 子节点集合的 长度
    trie_p* child_sets;             // 子节点集合的 具体容器
} ;


int trie_child_sets_show(trie_p b_trie_p);

trie_p trie_init(trie_p b_trie_p, uint8_t sets_level);

int trie_deinit(trie_p b_trie_p);

int trie_add(trie_p b_trie_p, char* string_, uint8_t str_len);

int trie_ergodic(trie_p b_trie_p);

#endif // __BINARY_TREE_H__

