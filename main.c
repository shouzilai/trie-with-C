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

    trie_child_sets_show(my_trie_ptr);
    trie_add(my_trie_ptr, "abc", 3);
    trie_add(my_trie_ptr, "aaz", 3);
    trie_add(my_trie_ptr, "aba", 3);
    trie_add(my_trie_ptr, "abcd", 4);

    trie_ergodic(my_trie_ptr);
    // trie_child_sets_show(my_trie_ptr);
    // trie_child_sets_show(*my_trie_ptr->child_sets);
    // trie_child_sets_show(*(*my_trie_ptr->child_sets)->child_sets);
    // trie_child_sets_show(*(*(*my_trie_ptr->child_sets)->child_sets)->child_sets);

    trie_deinit(my_trie_ptr);
    printf("Hello World\n");

    return 0;
}

// gcc -o trial main.c binary_tree.c

            // else {
            //     uint8_t _pos = cur_p->sets_pos;
            //     trie_p* temp = cur_p->child_sets;
            //     for (int k = 0; k < _pos; k++) {
            //         if ((*(temp + k))->letter == c_temp) { // 当前的下一层次存在相同结点，扫描下一结点
            //             printf("Hello World\n");
            //             state = EXIST;
            //             cur_p = *(temp + k);
            //             break;
            //         }
            //     }                
            // }