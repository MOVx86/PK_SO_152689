// DOC-MISSING
// UNFINISHED

#pragma once

#include "defines.h"

s32 create_shared_memory(key_t key, size_t size);
s32 create_semaphore(key_t key);

void sem_lock(s32 sem_id);
void sem_unlock(s32 sem_id);