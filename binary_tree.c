#include "binary_tree.h"

// int trie_child_sets_show(void)
// {  
//     uint8_t level = TRIE_CHILD_SET_SIZE, temp = 0;
//     sets_p t_sets = NULL;

//     for (int j = 0; j < TIRE_MAX_NODE; j++) {
//         if (s_sets[j] == NULL) {
//             printf(" %d NULL ", j);
//             continue;
//         }
//         t_sets = s_sets[j];
//         printf("<<trie show start>> level, count, set_p, child_set %d %d %p %p \n| ", t_sets->level, s_sets[j]->sets_count, s_sets[j], s_sets[j]->child_sets);
//         for (int i = level * sizeof(trie_p) - 1; i > -1 ; i--) {
//             temp = *((uint8_t*)t_sets->child_sets + i);
//             printf("%x ", temp);
//             if (i % 8 == 0) {
//                 printf("| ");
//             }
//         }
//         printf("<<trie show end>>\n");
//     }
//     printf("s_level is %d\n", s_level);
//     return SUCCESS;
// }

static char trie_letter(trie_p *b_trie_p)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }

    return (*b_trie_p)->letter;
}

int trie_child_sets_show(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    
    uint8_t level = b_trie_p->sets_level, temp = 0, count = 0;
    trie_p* tries_ptr = b_trie_p->child_sets, trie_ptr = b_trie_p;
    count = b_trie_p->sets_count;

    printf("<<<trie show start>>> level<%d> count<%d> | ", level, count);
    for (int i = 0; i < TIRE_MAX_NODE; i++) {
        trie_ptr = *(tries_ptr + i);
        printf("%18p | ", trie_ptr);
    }
    printf("<<<trie show end>>>\n");

    return SUCCESS;
}

trie_p trie_init(trie_p b_trie_p, uint8_t level)
{
    if (b_trie_p == NULL) {
        return NULL;
    }

    // 1、初始化 功能成员 
    b_trie_p->val = 0;
    b_trie_p->is_exist = DEFINITELY;
    b_trie_p->letter = 0;

    ///2、初始化 结构成员 
    b_trie_p->sets_count = 0;
    b_trie_p->sets_level = level;
    b_trie_p->child_sets = (trie_p*)malloc(TIRE_MAX_NODE * sizeof(trie_p));
    if (b_trie_p->child_sets == NULL) {
        printf("trie init failure\n");
        return NULL;
    }
    memset(b_trie_p->child_sets, 0x0, TIRE_MAX_NODE * sizeof(trie_p));

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
    uint8_t level = 0, pos = 0;
    trie_p* trie_ptr = NULL;
    trie_p tries_ptr = b_trie_p;

    // 1、递归遍历整个前缀树
    pos = tries_ptr->sets_count;
    level = tries_ptr->sets_level;
    trie_ptr = tries_ptr->child_sets;
    for (int i = 0; i < pos; i++) {
        printf("delete letter: %c  ", (*trie_ptr)->letter);
        trie_deinit(*trie_ptr);
        trie_ptr += 1;
    }
    printf("\n");

    // 2、释放单个结点的资源
    trie_single_delete(tries_ptr);
    if (tries_ptr != NULL && tries_ptr->is_exist != DEFINITELY) {
        free(tries_ptr);
        b_trie_p = NULL;
    }

    // printf("trie deinit success!!!\n");
    return SUCCESS;
}

static int trie_single_init(trie_p b_trie_p, char c, int level)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    trie_p tries_ptr = b_trie_p;

    b_trie_p->val = 0;
    b_trie_p->letter = c;
    b_trie_p->is_exist = NON_EXIST;

    b_trie_p->sets_count = 0;
    b_trie_p->sets_level = level;
    b_trie_p->child_sets = (trie_p*)malloc(TIRE_MAX_NODE * sizeof(trie_p));
    if (b_trie_p->child_sets == NULL) {
        printf("trie single init failure\n");
        return FAILURE;
    }

    memset(b_trie_p->child_sets, 0x0, TIRE_MAX_NODE * sizeof(trie_p));

    return SUCCESS;    
}

static int trie_singel_add(trie_p b_trie_p, char c, int level)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    uint8_t count = 0;
    trie_p* temp = NULL;
    trie_p tries_ptr = b_trie_p;

    count = tries_ptr->sets_count;
    // level = tries_ptr->sets_level;
    temp = tries_ptr->child_sets;
    
    if (count < TIRE_MAX_NODE) {
        temp += count;
        *temp = (trie_p)malloc(sizeof(trie_t));
        printf("count is %d, *temp 0x%p\n", count, *temp);
        if (*temp == NULL) {
            printf("trie single init failure\n");
            return FAILURE;
        }
        trie_single_init(*temp, c, level);
        // tries_ptr->sets_pos = (pos + 1) == level ? pos : pos + 1;
        tries_ptr->sets_count = count + 1;
    } else {
        printf("trie single table is full\n");
        return FAILURE;
    }
    return SUCCESS;
}

int trie_ergodic(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }

    uint8_t level = b_trie_p->sets_level, temp = 0, count = 0;
    trie_p* tries_ptr = NULL, trie_ptr = b_trie_p;

    count = trie_ptr->sets_count;
    level = trie_ptr->sets_level;
    tries_ptr = trie_ptr->child_sets;
    for (int i = 0; i < count; i++) {
        // printf("ergodic: %c  ", (*tries_ptr)->letter);
        trie_ergodic(*tries_ptr);
        tries_ptr += 1;
    }
    
    trie_child_sets_show(trie_ptr);

    return SUCCESS;
}

int trie_index(trie_p b_trie_p, char c)
{
    // if (b_trie_p == NULL || str_level < 0) {
    //     return FAILURE;
    // }
    // char c_temp = c;
    // uint8_t level = 0, pos = 0;
    // trie_p* temp = NULL;
    // trie_p cur_p = b_trie_p;

    // pos = cur_p->sets_pos;
    // level = cur_p->sets_level;
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
    if (str_len > 10) {
        str_len = 8;
    }
    EXIST_STATE state = NON_EXIST;
    char c_temp = 0;
    uint8_t level = 0, count = 0;
    trie_p* temp = NULL;
    trie_p cur_p = b_trie_p;

    count = cur_p->sets_count;
    level = cur_p->sets_level;
    temp = cur_p->child_sets;

    // 扫描字符串 如果出现某层结点即将出现重复结点，移至下一层建立结点
    for (int i = 0, j; i < str_len; i++) {
        c_temp = string_[i];
        printf("%c  ", c_temp);


        for (j = 0; j < count; j++) {
            if ((*(temp + j))->letter == c_temp) { // 当前层次存在相同结点，扫描下一结点
                printf("Hello World\n");
                state = EXIST;
                cur_p = *(temp + j);
                break;
            }
        }
        if (state == NON_EXIST) { // 当前层次不存在相同结点，直接建点
            trie_singel_add(cur_p, c_temp, i + 1);
            cur_p = *(temp + j);
        } else {
            state = NON_EXIST;
        }

        // 临时变量接替，下一主结点的成员们
        count = cur_p->sets_count;
        level = cur_p->sets_level;
        temp = cur_p->child_sets;
        // printf("\ncur_p is 0x%x\n", cur_p);
    }
    cur_p->is_exist = EXIST;

    return SUCCESS;
}