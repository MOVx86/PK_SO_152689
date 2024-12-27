#include "core/warehouse.h"

b8 run_warehouse_process(Warehouse *warehouse, s32 shm_id, s32 sem_id) { 
    printf("WAREHOUSE SUB-PROCESS: %d\n", getpid());

    sleep(10);

    return TRUE;
}