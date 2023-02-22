#include <stdio.h>
#include <assert.h>
#include "trial.h"
#include "common.h"


void* trie_data_init(uint8_t* meg, int cmd)
{
    if (cmd < 0) {
        return NULL;
    }
    trie_data_p* data_ps = NULL;
    trie_data_p data_p = NULL;

    // 1.数据块 （数组）分配
    data_ps = (trie_data_p*)calloc(TRIE_DATA_SIZE, sizeof(trie_data_t));
    assert(data_ps != NULL);

    // 2.首条数据 分配
    data_ps[0] = (trie_data_p)malloc(sizeof(trie_data_t));
    assert(data_ps[0] != NULL);
    data_p = (trie_data_p)data_ps[0];

    // 3.首条数据内容 分配
    data_p->message = (uint8_t*)calloc(TRIE_DATA_MEG_SIZE, sizeof(uint8_t));
    assert(data_p->message != NULL);

    // 4.首条数据 成员初始化
    memcpy(data_p->message, meg, strlen(meg));
    data_p->index = 0;
    data_p->cmd = cmd;

    printf("trie data init --- %s\n", data_p->message);
    return data_ps;
}

int trie_data_deinit(void *arg)
{
    if (arg == NULL) {
        return FAILURE;
    }
    trie_data_p* data_ps = (trie_data_p*)arg;
    trie_data_p data_p = NULL;

    for (int i = 0; i < TRIE_DATA_SIZE; i++) {
        data_p = data_ps[i];
        if (data_p == NULL) {
            continue;
        }
        if (data_p->message != NULL) {
            free(data_p->message);
            data_p->message = NULL;
        }
        free(data_p);
        data_p = NULL;
    }
    free(data_ps);

    printf("trie data deinit\n");
    return SUCCESS;
}

static trie_data_p trie_data_single_init(uint8_t* data, int cmd, int index)
{

}

int trie_data_add(void *arg, uint8_t* data, int cmd)
{
    if (arg == NULL || data == NULL || cmd < 0) {
        return FAILURE;
    }

    trie_data_p data_p = (trie_data_p)arg;



    printf("trie data add\n");
    return SUCCESS;
}

int trie_data_subtruct(void *arg, int index)
{
    printf("trie data subtruct\n");
}

void trie_data_show_list(void *arg)
{
    printf("trie data show list\n");
}


