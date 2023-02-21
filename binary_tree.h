#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#define DOUBLE_LINEAR_LIST_DATA_SIZE  128
#define TIRE_MAX_NODE  6

struct trie_task;
struct trie;

typedef struct trie trie_t, *trie_p;
typedef struct trie_task trie_task_t, *trie_task_p;

struct trie_task {
    void*   (*trie_data_init)(void *arg); // 返回数据列表数组
    int     (*trie_data_deinit)(void *arg);
    int     (*trie_data_add)(void *arg);
    int     (*trie_data_substruct)(void *arg);
    void    (*trie_data_show_list)(void *arg);
} ;

struct trie {
    trie_task_p task;
    void* argument;     // 接收数据列表数组

    uint8_t letter;
    EXIST_STATE is_exist;

    uint8_t sets_count;               // 标志子节点集合的 使用情况
    uint8_t sets_level;               // 子节点集合的 长度
    trie_p* child_sets;             // 子节点集合的 具体容器
} ;


int trie_child_sets_show(trie_p b_trie_p);

trie_p trie_init(trie_p b_trie_p, uint8_t level, void* extrnal_data_p);

int trie_deinit(trie_p b_trie_p);

int trie_add(trie_p b_trie_p, char* string_, uint8_t str_len);

int trie_ergodic(trie_p b_trie_p);

int trie_str_is_exist(trie_p b_trie_p, const char* string_, uint8_t str_len);

int trie_subtruct(trie_p b_trie_p, char* string_, uint8_t str_len);

trie_p trie_index(trie_p b_trie_p, char* string_, uint8_t str_len);

#endif // __BINARY_TREE_H__

