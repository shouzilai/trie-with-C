#include "binary_tree.h"

static int s_level = -1;
static sets_p s_sets[TIRE_MAX_NODE] = {NULL};

int trie_child_sets_show(void)
{  
    uint8_t len = TRIE_CHILD_SET_SIZE, temp = 0;
    sets_p t_sets = NULL;

    for (int j = 0; j < TIRE_MAX_NODE; j++) {
        if (s_sets[j] == NULL) {
            printf(" %d NULL ", j);
            continue;
        }
        t_sets = s_sets[j];
        printf("<<trie show start>> level, count, set_p, child_set %d %d %p %p \n| ", t_sets->level, s_sets[j]->sets_count, s_sets[j], s_sets[j]->child_sets);
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

static sets_p trie_sets_alloc(uint8_t level)
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

static int trie_sets_init(sets_p t_sets, uint8_t level)
{
    if (level < 0) {
        return FAILURE;
    }
    uint8_t count = 0;
    trie_p* tries_ptr = NULL;
    trie_p trie_ptr = NULL;
    sets_p temp = NULL;

    if (s_level == -1) {
        temp = trie_sets_alloc(level);
        s_sets[0] = temp;
        s_level = 0;
        return FAILURE;
    }

    count = t_sets->sets_count;
    tries_ptr = t_sets->child_sets;
    trie_ptr = *tries_ptr;
    if (level > s_level) {
        // 出现新层级的 子集, 填充子集数组
        temp = trie_sets_alloc(level);
        trie_ptr->sets = temp;
        s_level = (s_level + 1) % TIRE_MAX_NODE;
        s_sets[s_level] = trie_ptr->sets;
    } else {
        trie_ptr->sets = s_sets[level];
    }

    return SUCCESS;
}

static int trie_sets_deinit(sets_p* b_sets_p)
{
    if (b_sets_p == NULL) {
        return FAILURE;
    }
    sets_p cur_set_p = *b_sets_p;

    // 层次引用数到达 0 ，可释放子集空间
    if (cur_set_p->sets_count == 0) {
        printf("%p on ", cur_set_p->child_sets);
        if (cur_set_p->child_sets != NULL) {
            printf("%p down ", cur_set_p->child_sets);  
            free(cur_set_p->child_sets);
            cur_set_p->child_sets = NULL;
        }
        free(cur_set_p);
        *b_sets_p = NULL;
    } 

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
    trie_sets_init(NULL, 0);
    b_trie_p->sets = s_sets[0];

    // printf("trie init success, s_sets[0]->childs is 0x%p, level is %d\n", s_sets[0]->child_sets[0], s_sets[0]->level);
    printf("trie init success\n");
    return b_trie_p;
}

static int trie_single_delete(trie_p* b_trie_p)
{
    if (b_trie_p == NULL || *b_trie_p == NULL) {
        return FAILURE;
    }

    trie_p trie_ptr = *b_trie_p;

    if (trie_ptr != NULL) {
        free(trie_ptr);
        *b_trie_p = NULL;
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
    trie_p* tries_ptr = NULL;
    trie_p trie_ptr = b_trie_p;

    // 1.遍历整个前缀树层级的 子集.
    printf("delete node level, letter, count: | ");
    for (int i = 0; i < TIRE_MAX_NODE; i++) {
        if (s_sets[i] == NULL) {
            continue;
        }
        tries_ptr = s_sets[i]->child_sets;
        count = s_sets[i]->sets_count;
        for (int j = 0; j < count; j++) {
            // 2.回收 单个结点 的资源
            s_sets[i]->sets_count -= 1;
            printf("%d, %c, %d, %p | ", s_sets[i]->level, (*tries_ptr)->letter, s_sets[i]->sets_count, s_sets[i]);
            trie_single_delete(tries_ptr);
            tries_ptr += 1;
        }
        // 3.回收 层级 的资源
        trie_sets_deinit(s_sets + i);
        
    }
    printf("\n");

    printf("trie deinit success!!!\n");
    return SUCCESS;
}

static int trie_single_init(sets_p t_sets, char c)
{
    if (t_sets == NULL) {
        return FAILURE;
    }
    trie_p trie_ptr = NULL;
    uint8_t count = 0;
    trie_p* tries_ptr = NULL;

    count = t_sets->sets_count;
    tries_ptr = t_sets->child_sets;
    if (count >= TRIE_CHILD_SET_SIZE || tries_ptr == NULL) {
        return FAILURE;
    }

    *(tries_ptr + count) = (trie_p)malloc(sizeof(trie_t));
    if (*(tries_ptr + count) == NULL) {
        printf("trie single init failure\n");
        return FAILURE;
    }
    trie_ptr = *(tries_ptr + count);

    trie_ptr->val = 0;
    trie_ptr->letter = c;
    trie_ptr->is_exist = EXIST;
    trie_ptr->sets = NULL;

    t_sets->sets_count = count + 1;

    return SUCCESS;
}

static int trie_singel_add(uint8_t level, char c)
{
    if (level < 0) {
        return FAILURE;
    }
    uint8_t count = 0;
    sets_p t_sets = NULL;
    trie_p* tries_ptr = NULL;
    trie_p trie_ptr = NULL;

    t_sets = s_sets[level - 1];
    count = t_sets->sets_count;
    tries_ptr = t_sets->child_sets;

    if (trie_single_init(t_sets, c) != SUCCESS) {
        printf("trie single table is full\n");
        return FAILURE;
    }

    if (trie_sets_init(t_sets, level) != SUCCESS) {
        printf("trie sets init failure\n");
        return FAILURE;
    }

    return SUCCESS;
}

int trie_ergodic(void)
{
    uint8_t len = TRIE_CHILD_SET_SIZE;
    trie_p trie_ptr = NULL;
    sets_p t_sets = NULL;

    for (int j = 0; j < TIRE_MAX_NODE; j++) {
        if (s_sets[j] == NULL) {
            continue;
        }
        t_sets = s_sets[j];
        printf("<<trie ergodic start>> level is %d %p | ", t_sets->level, s_sets[j]);
        for (int i = 0; i < t_sets->sets_count; i++) {
            trie_ptr = *(t_sets->child_sets + i);
            printf("%c, %d, %d | ", trie_ptr->letter, trie_ptr->val, trie_ptr->is_exist);
        }
        printf("<<trie ergodic end>>\n");
    }

    return SUCCESS;
}

int trie_add(trie_p b_trie_p, char* string_, uint8_t str_len)
{
    if (string_ == NULL || str_len < 0) {
        return FAILURE;
    }
    EXIST_STATE state = NON_EXIST;
    char c_temp = 0;
    uint8_t count = 0;
    sets_p t_sets = NULL;
    trie_p* tries_ptr = NULL;
    trie_p trie_ptr = b_trie_p;

    // 扫描字符串 如果出现某层结点即将出现重复结点，移至下一层建立结点
    for (int i = 0; i < str_len; i++) {
        if (s_sets[i] == NULL) {
            continue;
        }
        c_temp = string_[i];
        t_sets = s_sets[i];
        tries_ptr = t_sets->child_sets;
        count = t_sets->sets_count;
        // printf("%d %c s_level is %d ", i, c_temp, s_level);

        for (int j = 0; j < count; j++) {
            if ((*(tries_ptr + j))->letter == c_temp) { // 当前层次存在相同字符结点，扫描下一子集
                printf("Hello World\n");
                state = EXIST;
                break;
            }
        }

        if (state == NON_EXIST) {
            trie_singel_add(i + 1, c_temp);
        } else {
            state = NON_EXIST;
        }
    }
    

    return SUCCESS;
}

// int trie_index()