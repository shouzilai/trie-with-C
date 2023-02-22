#include "binary_tree.h"

static void *s_data_task_p = NULL;

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

static int trie_task_register(trie_task_p *task_p, void* extrnal_data_p)
{
    if (task_p == NULL || extrnal_data_p == NULL) {
        return FAILURE;
    }

    trie_task_p cur_task_p = NULL, target_task_p = (trie_task_p)extrnal_data_p;

    cur_task_p = (trie_task_p)malloc(sizeof(trie_task_t));

    cur_task_p->init      = target_task_p->init;
    cur_task_p->deinit    = target_task_p->deinit;
    cur_task_p->add       = target_task_p->add;
    cur_task_p->subtruct = target_task_p->subtruct;
    cur_task_p->show_list = target_task_p->show_list; 

    *task_p = cur_task_p;

    return SUCCESS;
}

trie_p trie_init(trie_p b_trie_p, uint8_t level, void* extrnal_data_p)
{
    if (b_trie_p == NULL) {
        return NULL;
    }
    s_data_task_p = extrnal_data_p;

    // 1、初始化 功能成员 
    trie_task_register(&b_trie_p->task, s_data_task_p);
    b_trie_p->argument = b_trie_p->task->init("first", 0x1);
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


    // b_trie_p->task->deinit(b_trie_p->argument);
    b_trie_p->task->add(NULL, "aaa", 3);
    b_trie_p->task->subtruct(NULL, 0);
    b_trie_p->task->show_list(NULL);


    printf("trie init success\n");
    return b_trie_p;
}

static int trie_task_unregister(trie_task_p *task_p)
{
    if (task_p == NULL) {
        return FAILURE;
    }

    trie_task_p cur_task_p = NULL;

    if (*task_p != NULL) {
        free(*task_p);
        *task_p = NULL;
    }
 
    return SUCCESS;
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
    if (tries_ptr->is_exist == EXIST || tries_ptr->is_exist == DEFINITELY) {
        b_trie_p->task->deinit(b_trie_p->argument);
    }
    trie_task_unregister(&tries_ptr->task);

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
    for (int i = 0; i < TIRE_MAX_NODE; i++) {
        if (trie_ptr[i] != NULL) {
            printf("delete: %c  ", trie_ptr[i]->letter);
            trie_deinit(trie_ptr[i]);
        }
    }

    // 2、释放单个结点的资源
    trie_single_delete(tries_ptr);
    if (tries_ptr != NULL && tries_ptr->is_exist != DEFINITELY) {
        free(tries_ptr);
        b_trie_p = NULL;
    }

    return SUCCESS;
}

static int trie_single_init(trie_p b_trie_p, char c, int level, int is_end)
{
    if (b_trie_p == NULL) {
        return FAILURE;
    }
    trie_p tries_ptr = b_trie_p;

    if (is_end != 0) {
        trie_task_register(&b_trie_p->task, s_data_task_p);
        b_trie_p->argument = b_trie_p->task->init("init", 0x1);
        b_trie_p->is_exist = EXIST;
    } else {
        b_trie_p->is_exist = NON_EXIST;
        b_trie_p->argument = NULL;
        b_trie_p->task = NULL;
    }
    b_trie_p->letter = c;

    b_trie_p->sets_count = 0;
    b_trie_p->sets_level = level;
    b_trie_p->child_sets = (trie_p*)calloc(TIRE_MAX_NODE, sizeof(trie_p));
    if (b_trie_p->child_sets == NULL) {
        printf("trie single init failure\n");
        return FAILURE;
    }

    return SUCCESS;    
}

static int trie_singel_add(trie_p b_trie_p, char c, int level, int is_end)
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
        trie_single_init(*temp, c, level, is_end);
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
    trie_p* temp = NULL;
    trie_p cur_p = b_trie_p, trie_ptr = b_trie_p;

    temp = cur_p->child_sets;
    for (int i = 0; i < str_len; i++) {
        c_temp = string_[i];
        // 扫描字符串中每个字符，一旦出现本地为存储的返回 FAILURE
        for (int j = 0; j < TIRE_MAX_NODE; j++) {
            if (*(temp + j) != NULL) {
                trie_ptr = *(temp + j);
                cur_p = trie_ptr;
                if (trie_letter(trie_ptr) == c_temp) {
                    break;
                }
            } else if (j == TIRE_MAX_NODE - 1) {
                return NON_EXIST;
            } else {
                continue;
            }
        }
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
    uint8_t level = 0, count = 0, is_end = 0;
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
            is_end = (i == str_len - 1 ? 1 : 0);
            trie_singel_add(cur_p, c_temp, i + 1, is_end);
            cur_p = *(temp + j);
        } else {
            state = NON_EXIST;
        }

        // 临时变量接替，下一主结点的成员们
        count = cur_p->sets_count;
        level = cur_p->sets_level;
        temp = cur_p->child_sets;
    }
    // cur_p->is_exist = EXIST;

    return SUCCESS;
}

static int trie_single_deinit(trie_p *b_trie_ps, uint8_t c_count, uint8_t pos)
{
    if (b_trie_ps == NULL || c_count < 0 || pos < 0) {
        return FAILURE;
    }

    trie_p* tries_ptr = b_trie_ps, *sets_p = NULL;
    trie_p trie_ptr = NULL;

    sets_p = b_trie_ps[pos - 1]->child_sets;
    for (int j = 0; j < TIRE_MAX_NODE; j++) {
        if (sets_p[j] != NULL) {
            trie_ptr = sets_p[j];
            if (b_trie_ps[pos] == sets_p[j]) {
                printf("subtruct sets -%p- NULL \n", sets_p[j]);
                sets_p[j] = NULL;
            }
        }
    }

    return SUCCESS;
}

static int trie_single_subtruct(trie_p *b_trie_ps, uint8_t c_count)
{
    if (b_trie_ps == NULL ) {
        return FAILURE;
    }
    if (b_trie_ps[c_count - 1]->sets_count) {// 重点出现：如果树尾的子集上有挂载，只需要置 EXIST 即可结束
        b_trie_ps[c_count - 1]->task->deinit(b_trie_ps[c_count - 1]->argument);
        trie_task_unregister(&b_trie_ps[c_count - 1]->task);
        b_trie_ps[c_count - 1]->is_exist = NON_EXIST;
        return FAILURE;
    }
    char c_last = 0;
    uint8_t level = 0, count = 0;
    trie_p* tries_ptr = b_trie_ps, *sets_p = NULL;
    trie_p trie_ptr = NULL;

    for (int i = c_count - 1; i > -1; i--) {
        trie_ptr = b_trie_ps[i];
        c_last = trie_ptr->letter;
        if (i == c_count - 1) { // 单树的末尾结点直接删除
            printf("subtruct first %c\n", c_last);
            trie_single_delete(trie_ptr);
            free(trie_ptr);
        } else {
            // 上层结点的相应处理，如，计数减一
            trie_ptr->sets_count -= 1;
            if (trie_ptr->sets_count == 0) {
                printf("subtruct normal %c\n", c_last);
                trie_single_delete(trie_ptr);
                if (trie_ptr != NULL && trie_ptr->is_exist != DEFINITELY) {
                    free(trie_ptr);
                }
            }
        }
        if (i != 0) {// 父子集抹空
            trie_single_deinit(b_trie_ps, c_count, i);
        }
    }

    return SUCCESS;
}

int trie_subtruct(trie_p b_trie_p, char* string_, uint8_t str_len)
{
    if (b_trie_p == NULL || str_len < 0) {
        return FAILURE;
    }
    if (trie_str_is_exist(b_trie_p, string_, str_len) != EXIST) {
        return FAILURE;
    }

    EXIST_STATE state = NON_EXIST;
    char c_temp = 0;
    trie_p* temp = NULL;
    trie_p cur_p = b_trie_p, trie_ptr = b_trie_p;
    trie_p sub_ps[str_len + 1];

    sub_ps[0] = b_trie_p;
    temp = cur_p->child_sets;
    for (int i = 0; i < str_len; i++) {
        c_temp = string_[i];
        // 扫描字符串中每个字符，一旦出现本地为存储的返回 FAILURE
        for (int j = 0; j < TIRE_MAX_NODE; j++) {
            if (*(temp + j) != NULL) {
                trie_ptr = *(temp + j);
                cur_p = trie_ptr;
                if (trie_letter(trie_ptr) == c_temp) {
                    sub_ps[i + 1] = cur_p;
                    break;
                }
            } else if (j == TIRE_MAX_NODE - 1) {
                return NON_EXIST;
            } else {
                continue;
            }
        }
        temp = cur_p->child_sets;
    }
    trie_single_subtruct(sub_ps, str_len + 1);

    return SUCCESS;
}

trie_p trie_index(trie_p b_trie_p, char* string_, uint8_t str_len)
{
    if (b_trie_p == NULL || str_len < 0) {
        return NULL;
    }
    EXIST_STATE state = NON_EXIST;
    char c_temp = 0;
    trie_p* temp = NULL;
    trie_p cur_p = b_trie_p, trie_ptr = b_trie_p;

    temp = cur_p->child_sets;
    for (int i = 0; i < str_len; i++) {
        c_temp = string_[i];
        // 扫描字符串中每个字符，一旦出现本地为存储的返回 FAILURE
        for (int j = 0; j < TIRE_MAX_NODE; j++) {
            if (*(temp + j) != NULL) {
                trie_ptr = *(temp + j);
                cur_p = trie_ptr;
                if (trie_letter(trie_ptr) == c_temp) {
                    break;
                }
            } else if (j == TIRE_MAX_NODE - 1) {
                return NULL;
            } else {
                continue;
            }
        }
        temp = cur_p->child_sets;
    }

    return cur_p;
}

trie_p _trie_data_add(trie_p b_trie_p, char* string_, uint8_t str_len)
{
    if (b_trie_p == NULL || str_len < 0) {
        return NULL;
    }


}

trie_p _trie_data_subtruct(trie_p b_trie_p, char* string_, uint8_t str_len)
{
    if (b_trie_p == NULL || str_len < 0) {
        return NULL;
    }

    
}

