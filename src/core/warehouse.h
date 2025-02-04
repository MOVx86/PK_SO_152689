// DOC-MISSING
// UNFINISHED

#pragma once

#include "defines.h"
#include "ipc_utils.h"
#include "structures.h"

void load_warehouse_record(Warehouse *warehouse);
void write_warehouse_record(Warehouse * warehouse);
b8 run_warehouse_process(Warehouse *warehouse, s32 shm_id, s32 sem_id);