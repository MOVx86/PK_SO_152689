#include "core/manufacturer.h"

b8 run_manufactuter_process(s32 shm_id, s32 sem_id) {
    printf("MANUFACTURER SUB-PROCESS: %d\n", getpid());

    sleep(10);

    return TRUE;
}