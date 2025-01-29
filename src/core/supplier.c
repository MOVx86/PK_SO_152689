#include "core/supplier.h"

#define MIN_TIME 1
#define MAX_TIME 5

b8 run_supplier_process(Warehouse *warehouse, PART_TYPE type, s32 shm_id, s32 sem_id) {
    // starting supplier process
    #if DEBUG == 1
        printf("SUPPLIER SUB-PROCESS: %d\n", getpid());
    #endif

    // wait lock to not go into while loop without isOpen == TRUE
    while(!warehouse->isOpen) {}

    while(warehouse->isOpen) {
        #if USE_SLEEP == 1
            sleep((rand() % (MAX_TIME - MIN_TIME + 1) + MIN_TIME) * (type + 1));
        #endif

        sem_lock(sem_id);
        if (warehouse->isOpen) {
            switch (type) {
                case TYPE_X:
                    if (warehouse->capacity >= 1) {
                        warehouse->partX += 1;
                        warehouse->capacity -= 1;
                    }
                    else {
                        printf("Not enough space in warehouse!\n");
                    }
                    break;
                case TYPE_Y:
                    if (warehouse->capacity >= 2) {
                        warehouse->partY += 1;
                        warehouse->capacity -= 2;
                    }
                    else {
                        printf("Not enough space in warehouse!\n");
                    }
                    break;
                case TYPE_Z:
                    if (warehouse->capacity >= 3) {
                        warehouse->partZ += 1;
                        warehouse->capacity -= 3;
                    }
                    else {
                        printf("Not enough space in warehouse!\n");
                    }
                    break;
                default:
                    break;
            }
        }
        sem_unlock(sem_id);    
    }

    return TRUE;
}