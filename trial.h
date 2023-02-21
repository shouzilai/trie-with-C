#ifndef __TRIAL_H__
#define __TRIAL_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


struct task_trial;
struct trie_data;

typedef struct task_trial task_trial_t, *task_trial_p;
typedef struct trie_data trie_data_t, *trie_data_p;


struct task_trial {
    void*   (*trie_data_init)(void *arg); // 返回数据列表数组
    int     (*trie_data_deinit)(void *arg);
    int     (*trie_data_add)(void *arg);
    int     (*trie_data_substruct)(void *arg);
    void    (*trie_data_show_list)(void *arg);
} ;


struct trie_data {
    uint8_t* message;
    uint8_t cmd;
} ;

void* trie_data_init(void *arg); // 返回数据列表数组

int trie_data_deinit(void *arg);

int trie_data_add(void *arg);

int trie_data_substruct(void *arg);

void trie_data_show_list(void *arg);

#endif