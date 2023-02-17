#include "binary_tree.h"


static int trie_is_exist(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return NON_EXIST;
    }
    
    return b_trie_p->is_exist;
}

static char trie_letter(trie_p b_trie_p)
{
    if (b_trie_p == NULL) {
        return '-';
    }
    
    return b_trie_p->letter;
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
        printf("%14p <%c> <%d>| ", trie_ptr, trie_letter(trie_ptr), trie_is_exist(trie_ptr));
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
        printf("delete: %c  ", (*trie_ptr)->letter);
        trie_deinit(*trie_ptr);
        trie_ptr += 1;
    }

    // 2、释放单个结点的资源
    trie_single_delete(tries_ptr);
    if (tries_ptr != NULL && tries_ptr->is_exist != DEFINITELY) {
        free(tries_ptr);
        b_trie_p = NULL;
    }

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

int trie_str_is_exist(trie_p b_trie_p, const char* string_, uint8_t str_len)
{
    if (b_trie_p == NULL || string_ == NULL || str_len < 0) {
        return FAILURE;
    }
    EXIST_STATE state = NON_EXIST;
    char c_temp = 0;
    uint8_t level = 0, count = 0;
    trie_p* temp = NULL;
    trie_p cur_p = b_trie_p, trie_ptr = b_trie_p;

    count = cur_p->sets_count;
    level = cur_p->sets_level;
    temp = cur_p->child_sets;

    for (int i = 0; i < str_len; i++) {
        c_temp = *(string_ + i);
        // 扫描字符串中每个字符，一旦出现本地为存储的返回 FAILURE
        for (int j = 0; j < TIRE_MAX_NODE; j++) {
            if (*(temp + j) == NULL) {
                continue;
            }
            trie_ptr = *(temp + j);
            cur_p = trie_ptr;
            if (trie_letter(trie_ptr) == c_temp) {
                break;
            } else if (j == count - 1) {
                return NON_EXIST;
            } else {
                continue;
            }
        }

        count = cur_p->sets_count;
        level = cur_p->sets_level;
        temp = cur_p->child_sets;
    }

    return trie_is_exist(cur_p);
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
        printf("%c ", c_temp);

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
    }
    cur_p->is_exist = EXIST;

    return SUCCESS;
}