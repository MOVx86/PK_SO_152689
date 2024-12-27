#pragma once

#include "defines.h"
#include "ipc_utils.h"
#include "structures.h"
#include "pid_array.h"

void close_wh(s32 signal);
void close_ma(s32 signal);
void close_keep_wh_ma(s32 signal);
void close_wh_ma(s32 signal);

void close_main(s32 signal);

b8 run_manager_process(pid_list *processes, s32 shm_id, s32 sem_id);