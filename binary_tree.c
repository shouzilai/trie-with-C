#include "binary_tree.h"


int trie_child_sets_show(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    
    uint8_t len = b_trie_p->sets_len, temp = 0;
    trie_p* tries_ptr = b_trie_p->child_sets;

    printf("<<<trie show start>>> | ");
    for (int i = len * sizeof(trie_p) - 1; i > -1 ; i--) {
        temp = *((uint8_t*)tries_ptr + i);
        printf("%x ", temp);
        if (i % 4 == 0) {
            printf("| ");
        }
    }
    printf("<<<trie show end>>>\n");

    return SUCCESS;
}

trie_p trie_init(trie_p b_trie_p, uint8_t sets_len)
{
    if (b_trie_p == NULL) {
        return NULL;
    }

    // 1、初始化 功能成员 
    b_trie_p->val = 0;
    b_trie_p->is_exist = NON_EXIST;
    b_trie_p->letter = 0;

    ///2、初始化 结构成员 
    b_trie_p->sets_pos = 0;
    b_trie_p->sets_len = sets_len;
    b_trie_p->child_sets = (trie_p*)malloc(sets_len * sizeof(trie_p));
    if (b_trie_p->child_sets == NULL) {
        printf("trie init failure\n");
        return NULL;
    }
    memset(b_trie_p->child_sets, 0x0, sets_len * sizeof(trie_p));

    printf("trie init success\n");
    return b_trie_p;
}

static int trie_single_delete(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }

    trie_p tries_ptr = b_trie_p;
    if (tries_ptr->child_sets != NULL) {
        free(tries_ptr->child_sets);
        tries_ptr->child_sets = NULL;
    }

    return SUCCESS;
}

int trie_deinit(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    uint8_t len = 0, pos = 0;
    trie_p* temp = NULL;
    trie_p tries_ptr = b_trie_p;

    // 1、递归遍历整个前缀树
    pos = tries_ptr->sets_pos;
    len = tries_ptr->sets_len;
    temp = tries_ptr->child_sets;
    for (int i = 0; i < pos; i++) {
        printf("delete letter: %c  ", (*temp)->letter);
        trie_deinit(*temp);
        temp += 1;
    }
    printf("\n");

    // 2、释放单个结点的资源
    trie_single_delete(tries_ptr);

    printf("trie deinit success!!!\n");
    return SUCCESS;
}

static int trie_single_init(trie_p b_trie_p, char c)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    trie_p tries_ptr = b_trie_p;

    b_trie_p->val = 0;
    b_trie_p->letter = c;
    b_trie_p->is_exist = NON_EXIST;

    b_trie_p->sets_pos = 0;
    b_trie_p->sets_len = TIRE_MAX_NODE;
    b_trie_p->child_sets = (trie_p*)malloc(TIRE_MAX_NODE * sizeof(trie_p));
    if (b_trie_p->child_sets == NULL) {
        printf("trie single init failure\n");
        return FAILURE;
    }

    memset(b_trie_p->child_sets, 0x0, TIRE_MAX_NODE * sizeof(trie_p));

    return SUCCESS;    
}

static int trie_singel_add(trie_p b_trie_p, char c)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    uint8_t len = 0, pos = 0;
    trie_p* temp = NULL;
    trie_p tries_ptr = b_trie_p;

    pos = tries_ptr->sets_pos;
    len = tries_ptr->sets_len;
    temp = tries_ptr->child_sets;
    
    if (pos < len) {
        temp += pos;
        *temp = (trie_p)malloc(sizeof(trie_t));
        printf("pos is %d, *temp 0x%x\n", pos, *temp);
        if (*temp == NULL) {
            printf("trie single init failure\n");
            return FAILURE;
        }
        trie_single_init(*temp, c);
        // tries_ptr->sets_pos = (pos + 1) == len ? pos : pos + 1;
        tries_ptr->sets_pos = pos + 1;
    } else {
        printf("trie single table is full\n");
        return FAILURE;
    }
    return SUCCESS;
}

int trie_add1(trie_p b_trie_p, char* string_, uint8_t str_len)
{
    if (b_trie_p == NULL || str_len < 0) {
        return FAILURE;
    }
    EXIST_STATE state = NON_EXIST;
    char c_temp = 0;
    uint8_t len = 0, pos = 0;
    trie_p* temp = NULL;
    trie_p cur_p = b_trie_p;

    pos = cur_p->sets_pos;
    len = cur_p->sets_len;
    temp = cur_p->child_sets;

    // 扫描字符串 如果出现某层结点即将出现重复结点，移至下一层建立结点
    for (int i = 0, j; i < str_len; i++) {
        c_temp = string_[i];
        printf("%c  ", c_temp);


        for (j = 0; j < pos; j++) {
            if ((*(temp + j))->letter == c_temp) { // 当前层次存在相同结点，扫描下一结点
                printf("Hello World\n");
                state = EXIST;
                cur_p = *(temp + j);
                break;
            }
        }
        if (state == NON_EXIST) { // 当前层次不存在相同结点，直接建点
            trie_singel_add(cur_p, c_temp);
            cur_p = *(temp + j);
        } else {
            state = NON_EXIST;
        }

        // 临时变量接替，下一主结点的成员们
        pos = cur_p->sets_pos;
        len = cur_p->sets_len;
        temp = cur_p->child_sets;
        // printf("\ncur_p is 0x%x\n", cur_p);
    }
    cur_p->is_exist = EXIST;

    return SUCCESS;
}

int trie_index(trie_p b_trie_p, char c)
{
    // if (b_trie_p == NULL || str_len < 0) {
    //     return FAILURE;
    // }
    // char c_temp = c;
    // uint8_t len = 0, pos = 0;
    // trie_p* temp = NULL;
    // trie_p cur_p = b_trie_p;

    // pos = cur_p->sets_pos;
    // len = cur_p->sets_len;
    // temp = cur_p->child_sets;

    // for (int i = 0; i < pos; i++) {
    //     if ((*(temp + i))->letter == c_temp) {
    //         return 
    //     }
    // }
    // trie_index

    return SUCCESS;
}

int trie_add(trie_p b_trie_p, char* string_, uint8_t str_len)
{
    if (b_trie_p == NULL || str_len < 0) {
        return FAILURE;
    }
    EXIST_STATE state = NON_EXIST;
    char c_temp = 0;
    uint8_t len = 0, pos = 0;
    trie_p* temp = NULL;
    trie_p cur_p = b_trie_p;

    pos = cur_p->sets_pos;
    len = cur_p->sets_len;
    temp = cur_p->child_sets;

    // 扫描字符串 如果出现某层结点即将出现重复结点，移至下一层建立结点
    for (int i = 0, j; i < str_len; i++) {
        c_temp = string_[i];
        printf("%c  ", c_temp);


        for (j = 0; j < pos; j++) {
            if ((*(temp + j))->letter == c_temp) { // 当前层次存在相同结点，扫描下一结点
                printf("Hello World\n");
                state = EXIST;
                cur_p = *(temp + j);
                break;
            }
        }
        if (state == NON_EXIST) { // 当前层次不存在相同结点，直接建点
            trie_singel_add(cur_p, c_temp);
            cur_p = *(temp + j);
        } else {
            state = NON_EXIST;
        }

        // 临时变量接替，下一主结点的成员们
        pos = cur_p->sets_pos;
        len = cur_p->sets_len;
        temp = cur_p->child_sets;
        // printf("\ncur_p is 0x%x\n", cur_p);
    }
    cur_p->is_exist = EXIST;

    return SUCCESS;
}