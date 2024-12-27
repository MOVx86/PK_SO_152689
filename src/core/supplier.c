#include "core/supplier.h"

b8 run_supplier_process(PART_TYPE type, s32 shm_id, s32 sem_id) {
    printf("SUPPLIER SUB-PROCESS: %d\n", getpid());

    sleep(10);

    return TRUE;
}