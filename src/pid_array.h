#pragma once

#include "defines.h"

typedef struct pid_list {
    pid_t *pids;
    size_t size;
    size_t capacity;
} pid_list;

void init_pid_list(pid_list *list);
void push_pid(pid_list *list, pid_t new_pid);
void free_list(pid_list *list);