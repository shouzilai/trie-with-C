#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>

#include "common.h"
#include "binary_tree.h"

int main()
{
    uint8_t data[10] = {0};
    trie_t my_trie, *my_trie_ptr;
    my_trie_ptr = trie_init(&my_trie, 0);

    if (my_trie_ptr == NULL) {
        printf("NULL\n");
    }

    // trie_child_sets_show(my_trie_ptr);
    trie_add(my_trie_ptr, "great", 5);
    trie_add(my_trie_ptr, "tri", 3);
    trie_add(my_trie_ptr, "trial", 5);
    trie_add(my_trie_ptr, "my", 2);

    trie_ergodic(my_trie_ptr);
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

// gcc -o trial main.c binary_tree.c
// valgrind --leak-check=yes ./trial



