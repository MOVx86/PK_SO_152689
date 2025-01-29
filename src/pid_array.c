// DOC-MISSING
// FINISHED

#include "pid_array.h"

void init_pid_list(pid_list *list) {
    list->pids = NULL;
    list->size = 0;
    list->capacity = 0;
}

void push_pid(pid_list *list, pid_t new_pid) {
    if (list->size == list->capacity) {
        size_t new_capacity = (list->capacity == 0) ? 2 : list->capacity * 2;
        pid_t *new_pids = realloc(list->pids, new_capacity * sizeof(pid_t));
        if (!new_pids) {
            perror("failed to allocate memory!");
            exit(1);
        }
        list->pids = new_pids;
        list->capacity = new_capacity;
    }
    list->pids[list->size++] = new_pid;
}

void free_list(pid_list *list) {
    free(list->pids);
    list->pids = NULL;
    list->size = 0;
    list->capacity = 0;
}