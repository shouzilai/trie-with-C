#include "binary_tree.h"

static uint8_t s_level = -1;
static sets_p s_sets[TIRE_MAX_NODE] = {NULL};

int trie_child_sets_show()
{  
    uint8_t len = TRIE_CHILD_SET_SIZE, temp = 0;
    sets_p t_sets = NULL;

    for (int j = 0; j < TIRE_MAX_NODE; j++) {
        if (s_sets[j] == NULL) {
            continue;
        }
        t_sets = s_sets[j];
        printf("<<trie show start>> level is %d | ", t_sets->level);
        for (int i = len * sizeof(trie_p) - 1; i > -1 ; i--) {
            temp = *((uint8_t*)t_sets->child_sets + i);
            printf("%x ", temp);
            if (i % 8 == 0) {
                printf("| ");
            }
        }
        printf("<<trie show end>>\n");
    }

    printf("s_level is %d\n", s_level);
    return SUCCESS;
}

static sets_p trie_sets_init(uint8_t level)
{
    if (level < 0) {
        return NULL;
    }
    sets_p temp = NULL;

    // 1.分配 子集 空间
    temp = (sets_p)malloc(sizeof(sets_t));
    if (temp == NULL) {
        printf("trie sets init failure\n");
        return NULL;
    }
    memset(temp, 0x0, sizeof(sets_t));

    // 2.初始化 子集成员
    temp->level = level;
    temp->sets_count = 0;
    temp->child_sets = (trie_p*)malloc(TRIE_CHILD_SET_SIZE * sizeof(trie_p*));
    if (temp->child_sets == NULL) {
        printf("child sets malloc failure\n");
        return NULL;
    }
    memset(temp->child_sets, 0x0, TRIE_CHILD_SET_SIZE * sizeof(trie_p*));

    return temp;
}

static int trie_sets_deinit(sets_p b_sets_p)
{
    if (b_sets_p == NULL) {
        return FAILURE;
    }

    // 层次引用数到达 0 ，可释放子集空间
    if (b_sets_p->sets_count == 0) {
        if (b_sets_p->child_sets != NULL) {
            free(b_sets_p->child_sets);
            b_sets_p->child_sets = NULL;
        }
    } 
    // free(b_sets_p);
    // else {
    //     b_sets_p->sets_count = b_sets_p->sets_count - 1;
    // }


    return SUCCESS;
}

static int trie_sets_add(sets_p b_sets_p, uint8_t level)
{
    if (b_sets_p == NULL) {
        return FAILURE;
    }


}

static int trie_sets_delete(sets_p b_sets_p)
{
    if (b_sets_p == NULL) {
        return FAILURE;
    }


}



trie_p trie_init(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return NULL;
    }

    // 1、初始化 功能成员 
    b_trie_p->val = 0;
    b_trie_p->is_exist = NON_EXIST;
    b_trie_p->letter = 0;

    ///2、初始化 结构成员 
    b_trie_p->sets = trie_sets_init(0);
    s_sets[0] = b_trie_p->sets;
    s_level += 1;

    printf("trie init success, s_sets[0]->childs is 0x%p, level is %d\n", s_sets[0]->child_sets[0], s_sets[0]->level);
    return b_trie_p;
}

static int trie_single_delete(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }

    trie_p tries_ptr = b_trie_p;

    if (tries_ptr->sets != NULL) {
        trie_sets_deinit(tries_ptr->sets);
    }

    return SUCCESS;
}

int trie_deinit(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    uint8_t len = 0, count = 0;
    sets_p t_sets = NULL;
    trie_p* child_set = NULL;
    trie_p tries_ptr = b_trie_p;

    // 1.递归遍历整个前缀树层级的 子集.
    printf("delete node level, letter: | ");
    for (int i = 0; i < TIRE_MAX_NODE; i++) {
        if (s_sets[i] == NULL) {
            continue;
        }
        child_set = s_sets[i]->child_sets;
        count = s_sets[i]->sets_count;
        for (int j = 0; j < count; j++) {
            // 2.释放单个结点的资源
            printf("%d, %c %p | ", s_sets[i]->level, (*child_set)->letter, *child_set);
            trie_single_delete(*child_set);
            free(*child_set);
            *child_set = NULL;
            // printf("***********\n");
            child_set += 1;
        }
        // trie_deinit(*t_sets);
        // t_sets += 1;
    }
    printf("\n");

    printf("trie deinit success!!!\n");
    return SUCCESS;
}

static int trie_single_init(trie_p b_trie_p, uint8_t level, char c)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    trie_p tries_ptr = b_trie_p;

    b_trie_p->val = 0;
    b_trie_p->letter = c;
    b_trie_p->is_exist = NON_EXIST;
    //  b_trie_p->sets = NULL;

    // 出现新层级的 子集, 填充子集数组
    if (level > s_level) {
        b_trie_p->sets = trie_sets_init(level);
        s_level = (s_level + 1) % TIRE_MAX_NODE;
        s_sets[s_level] = b_trie_p->sets;    // 这两语句，一直顺序反了，注意一下；；导致添加结点一直失败
    } else {
        b_trie_p->sets = s_sets[level];
    }

    return SUCCESS;    
}

static int trie_singel_add(trie_p b_trie_p, uint8_t level, char c)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    uint8_t count = 0;
    sets_p t_sets = NULL;
    trie_p* child_set = NULL;
    trie_p tries_ptr = b_trie_p;

    t_sets = s_sets[level - 1];
    count = t_sets->sets_count;
    child_set = t_sets->child_sets;
    // child_set = tries_ptr->sets->child_sets;
    // count = tries_ptr->sets->sets_count;     // 注释部分是错误的，需要依据层次级别来确定分布位置
    
    if (count < TRIE_CHILD_SET_SIZE) {
        child_set += count;
        *child_set = (trie_p)malloc(sizeof(trie_t));
        printf("count is %d, *child_set 0x%p\n", count, *child_set);
        if (*child_set == NULL) {
            printf("trie single init failure\n");
            return FAILURE;
        }
        trie_single_init(*child_set, level, c);
        t_sets->sets_count = count + 1;
    } else {
        printf("trie single table is full\n");
        return FAILURE;
    }
    return SUCCESS;
}

// int trie_add1(trie_p b_trie_p, char* string_, uint8_t str_len)
// {
//     if (b_trie_p == NULL || str_len < 0) {
//         return FAILURE;
//     }
//     EXIST_STATE state = NON_EXIST;
//     char c_temp = 0;
//     uint8_t len = 0, pos = 0;
//     trie_p* temp = NULL;
//     trie_p cur_p = b_trie_p;
//     pos = cur_p->sets_count;
//     len = cur_p->sets_len;
//     temp = cur_p->child_sets;
//     // 扫描字符串 如果出现某层结点即将出现重复结点，移至下一层建立结点
//     for (int i = 0, j; i < str_len; i++) {
//         c_temp = string_[i];
//         printf("%c  ", c_temp);
//         for (j = 0; j < pos; j++) {
//             if ((*(temp + j))->letter == c_temp) { // 当前层次存在相同结点，扫描下一结点
//                 printf("Hello World\n");
//                 state = EXIST;
//                 cur_p = *(temp + j);
//                 break;
//             }
//         }
//         if (state == NON_EXIST) { // 当前层次不存在相同结点，直接建点
//             trie_singel_add(cur_p, c_temp);
//             cur_p = *(temp + j);
//         } else {
//             state = NON_EXIST;
//         }
//         // 临时变量接替，下一主结点的成员们
//         pos = cur_p->sets_count;
//         len = cur_p->sets_len;
//         temp = cur_p->child_sets;
//         // printf("\ncur_p is 0x%x\n", cur_p);
//     }
//     cur_p->is_exist = EXIST;
//     return SUCCESS;
// }

int trie_index(trie_p b_trie_p, char c)
{
    // if (b_trie_p == NULL || str_len < 0) {
    //     return FAILURE;
    // }
    // char c_temp = c;
    // uint8_t len = 0, pos = 0;
    // trie_p* temp = NULL;
    // trie_p cur_p = b_trie_p;

    // pos = cur_p->sets_count;
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
    uint8_t count = 0;
    sets_p t_sets = NULL;
    trie_p* child_set = NULL;
    trie_p cur_p = b_trie_p;

    // 扫描字符串 如果出现某层结点即将出现重复结点，移至下一层建立结点
    for (int i = 0; i < str_len; i++) {
        if (s_sets[i] == NULL) {
            continue;
        }
        c_temp = string_[i];
        t_sets = s_sets[i];
        child_set = t_sets->child_sets;
        count = t_sets->sets_count;
        printf("%d %c s_level is %d ", i, c_temp, s_level);

        for (int j = 0; j < count; j++) {
            if ((*(child_set + j))->letter == c_temp) { // 当前层次存在相同字符结点，扫描下一子集
                printf("Hello World\n");
                state = EXIST;
                cur_p = *(child_set + j);
                break;
            }
        }
        if (state == NON_EXIST) {
            trie_singel_add(cur_p, i + 1, c_temp);
        } else {
            state = NON_EXIST;
        }

        // for (j = 0; j < count; j++) {
        //     if ((*(t_sets + j))->letter == c_temp) { // 当前层次存在相同结点，扫描下一结点
        //         printf("Hello World\n");
        //         state = EXIST;
        //         cur_p = *(t_sets + j);
        //         break;
        //     }
        // }
        // if (state == NON_EXIST) { // 当前层次不存在相同结点，直接建点
        //     trie_singel_add(cur_p, i + 1, c_temp);
        //     cur_p = *(t_sets + j);
        // } else {
        //     state = NON_EXIST;
        // }

        // 临时变量接替，下一主结点的成员们
        // count = cur_p->sets->sets_count;
        // t_sets = cur_p->sets->child_sets;
        // printf("\ncur_p is 0x%x\n", cur_p);
    }
    cur_p->is_exist = EXIST;

    return SUCCESS;
}