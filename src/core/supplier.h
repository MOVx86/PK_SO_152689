// DOC-MISSING
// FINISHED

#pragma once

#include "defines.h"
#include "ipc_utils.h"
#include "structures.h"

b8 run_supplier_process(Warehouse *warehouse, PART_TYPE type, s32 shm_id, s32 sem_id);