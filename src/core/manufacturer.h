// DOC-MISSING
// UNFINISHED

#pragma once

#include "defines.h"
#include "ipc_utils.h"
#include "structures.h"

b8 run_manufactuter_process(const char *queue, Warehouse *warehouse, s32 shm_id, s32 sem_id);