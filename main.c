#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>

#include "common.h"
#include "binary_tree.h"
#include "trial.h"

const trie_task_t trial_t = {
    trie_data_init, trie_data_deinit, trie_data_add, trie_data_subtruct, trie_data_show_list
};

int main()
{
    uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    trie_t my_trie, *my_trie_ptr;
    my_trie_ptr = trie_init(&my_trie, 0, (void*)&trial_t);

    if (my_trie_ptr == NULL) {
        printf("NULL\n");
    }

    // exit(0);

    // trie_child_sets_show(my_trie_ptr);
    trie_add(my_trie_ptr, "great", 5);
    trie_add(my_trie_ptr, "tri", 3);
    trie_add(my_trie_ptr, "trial", 5);
    // trie_add(my_trie_ptr, "my", 2);
    // trie_add(my_trie_ptr, "health", 6);
    // trie_add(my_trie_ptr, "none", 4);

    trie_ergodic(my_trie_ptr);
    // _trie_data_add(my_trie_ptr, "great", "123456", 0x1);
    // _trie_data_add(my_trie_ptr, "great", "dfsdfafasdfs", 0x1);
    // _trie_data_show_list(my_trie_ptr, "great");
    // _trie_data_subtruct(my_trie_ptr, "great", 1);
    // _trie_data_show_list(my_trie_ptr, "great");

    // printf("\nexist is %d\n", trie_str_is_exist(my_trie_ptr, "trial", 3));
    // printf("exist is %d\n", trie_str_is_exist(my_trie_ptr, "abc", 3));

    trie_subtruct(my_trie_ptr, "tri", 3);
    trie_subtruct(my_trie_ptr, "great", 5);
    
    // printf("%p", trie_index(my_trie_ptr, "great", 5));
    // trie_child_sets_show(trie_index(my_trie_ptr, "tri", 3));
    
    // trie_child_sets_show(my_trie_ptr);
    // trie_child_sets_show(*my_trie_ptr->child_sets);
    // trie_child_sets_show(*(*my_trie_ptr->child_sets)->child_sets);
    // trie_child_sets_show(*(*(*my_trie_ptr->child_sets)->child_sets)->child_sets);

    trie_deinit(my_trie_ptr);
    printf("Hello World\n");

    return 0;
}

// gcc -o trial main.c binary_tree.c trial.c
// valgrind --leak-check=yes ./trial



