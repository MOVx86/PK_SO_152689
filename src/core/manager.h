// DOC-MISSING
// UNFINISHED

#pragma once

#include "defines.h"
#include "ipc_utils.h"
#include "structures.h"
#include "pid_array.h"

extern b8 pendingSignal;
extern message_t pendingMessage;

void signal_handler(s32 signal);

b8 run_manager_process(pid_list *processes, Warehouse *warehouse, s32 shm_id, s32 sem_id);