#pragma once

#include "defines.h"
#include "ipc_utils.h"

typedef enum wh_status {
    EXIT    = 0,
    RUNNING = 1
} wh_status;

typedef enum wh_document {
    NGEN_DOC_ON_TERMINATION = 0,
    GEN_DOC_ON_TERMINATION  = 1
} wh_document;

typedef enum wh_capacity {
    EMPTY       = 0,
    OPERATIONAL = 1,
    FULL        = 2 
} wh_capacity;

typedef struct Warehouse {
    volatile u32 partX;
    volatile u32 partY;
    volatile u32 partZ;
    volatile u32 capacity;
    wh_status status;
    wh_document document;
    wh_capacity capacityStatus;
} Warehouse;

b8 run_warehouse_process(Warehouse *warehouse, s32 shm_id, s32 sem_id);