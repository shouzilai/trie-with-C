#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#define DOUBLE_LINEAR_LIST_DATA_SIZE  128
#define TIRE_MAX_NODE          6
#define TRIE_CHILD_SET_SIZE    6

struct binary_tree;
struct trie;

typedef struct binary_tree binary_tree_t, *binary_tree_p;
typedef struct trie trie_t, *trie_p;
typedef struct sets sets_t, *sets_p;

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

    sets_p sets;
} ;

struct sets {
    uint8_t level;                  // 集合的   层次级别
    uint8_t sets_count;             // 集合的   引用情况

    trie_p* child_sets;             // 集合的   具体容器

} ;


int trie_child_sets_show();

trie_p trie_init(trie_p b_trie_p);

int trie_deinit(trie_p b_trie_p);

int trie_add(trie_p b_trie_p, char* string_, uint8_t str_len);


#endif // __BINARY_TREE_H__

