#include "defines.h"
#include "structures.h"
#include "ipc_utils.h"
#include "pid_array.h"
#include "core/manager.h"
#include "core/warehouse.h"
#include "core/supplier.h"
#include "core/manufacturer.h"

// TODO detailed comments
// TODO messaging system
// TODO all internal subprocess functioning

/* ------------------- */
/* PROGRAM ENTRY POINT */
/* ------------------- */

struct mq_attr attributes = {
    .mq_flags   = 0,
    .mq_maxmsg  = 10,
    .mq_msgsize = sizeof(message_t),
    .mq_curmsgs = 0
};

int main(void) {
    /* --------------------- */
    /* INITIAL PROGRAM SETUP */
    /* --------------------- */

    // all forks stored in a dynamic list
    // used to send all sub-process PIDs to parent process loop
    pid_list forks;
    init_pid_list(&forks);

    // generate ipc and shared mem keys
    size_t shm_size = sizeof(Warehouse);
    key_t mng_key = ftok("../bin/fabryka", 1);

    s32 shm_id = create_shared_memory(mng_key, shm_size);
    s32 sem_id = create_semaphore(mng_key);

    Warehouse *warehouse = (struct Warehouse *) shmat(shm_id, NULL, 0);
    if (warehouse == (void*) -1) {
        perror("shmat failed");
        return 1;
    }

    /* ----------------------------------- */
    /* CREATING FORKS & ALL MESSAGE QUEUES */
    /* ----------------------------------- */

    // warehouse sub-process
    pid_t warehouse_id = fork();
    if (warehouse_id == 0) {
        if (run_warehouse_process(warehouse, shm_id, sem_id)) {
            return 0;;
        }
        else {
            perror("manager subprocess failed to close properly");
            return 1;
        }
    }
    push_pid(&forks, warehouse_id);


    // suppliers sub-processes
    pid_t supplierX_id = fork();
    if (supplierX_id == 0) {
        if (run_supplier_process(TYPE_X, shm_id,sem_id)) {
            return 0;;
        }
        else {
            perror("supplier X subprocess failed to close properly");
            return 1;
        }
    }
    push_pid(&forks, supplierX_id);

    pid_t supplierY_id = fork();
    if (supplierY_id == 0) {
        if (run_supplier_process(TYPE_Y, shm_id,sem_id)) {
            return 0;;
        }
        else {
            perror("supplier Y subprocess failed to close properly");
            return 1;
        }
    }
    push_pid(&forks, supplierY_id);

    pid_t supplierZ_id = fork();
    if (supplierZ_id == 0) {
        if (run_supplier_process(TYPE_Z, shm_id,sem_id)) {
            return 0;;
        }
        else {
            perror("supplier Z subprocess failed to close properly");
            return 1;
        }
    }
    push_pid(&forks, supplierZ_id);

    // manufacturers sub-processes
    pid_t manufacturerA_id = fork();
    if (manufacturerA_id == 0) {
        if (run_manufactuter_process(shm_id, sem_id)) {
            return 0;;
        }
        else {
            perror("manufacturer A subprocess failed to close properly");
            return 1;
        }
    }
    push_pid(&forks, manufacturerA_id);

    pid_t manufacturerB_id = fork();
    if (manufacturerB_id == 0) {
        if (run_manufactuter_process(shm_id, sem_id)) {
            return 0;;
        }
        else {
            perror("manufacturer A subprocess failed to close properly");
            return 1;
        }
    }
    push_pid(&forks, manufacturerA_id);

    /* ------------------------------------------- */
    /* MANAGER - PARENT PROCESS ACTUAL FUNCTIONING */
    /* ------------------------------------------- */

    // run manager parent process loop
    #if DEBUG == 1
        printf("Running manager\n");
    #endif
    if (run_manager_process(&forks, shm_id, sem_id));
    // TODO handling incorrect manager closing (wrapper macro?)

    // unlink all queues
    // TODO handling failed unlinking (wrapper macro?)
    mq_unlink("/qW");
    mq_unlink("/qA");
    mq_unlink("/qB");

    // clear shared memory
    if (shmdt(warehouse) == -1) {
        perror("shmdt failed");
        return 1;
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        return 1;
    }

    return 0;
}