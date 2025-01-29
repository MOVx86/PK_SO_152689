// DOC-MISSING
// FINISHED

#include "defines.h"
#include "structures.h"
#include "ipc_utils.h"
#include "pid_array.h"
#include "core/manager.h"
#include "core/warehouse.h"
#include "core/supplier.h"
#include "core/manufacturer.h"

// wrapper macro for handling sub-process running loop functions
#define RUN_LOOP(pid, function)                                                         \
    do {                                                                                \
        if (function) {                                                                 \
            return(0);                                                                  \
        }                                                                               \
        else {                                                                          \
            char error_message[256];                                                    \
            snprintf(error_message, sizeof(error_message), "[%d] process failed", pid); \
            perror(error_message);                                                      \
            return(1);                                                                  \
        }                                                                               \
    } while (0)                                                                         \

// wrapper macro for handling unlinking of message queues
#define MQ_UNLINK(queue)                \
    do {                                \
        if (mq_unlink(queue) == -1) {   \
            perror(queue);              \
        }                               \
    }                                   \
    while (0)                           \

/* ------------------- */
/* PROGRAM ENTRY POINT */
/* ------------------- */

// definition of message queue attributes
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
    key_t shm_key = ftok("../bin/fabryka", 1);

    // create and allocate shared memory, create shared memory semaphore
    s32 shm_id = create_shared_memory(shm_key, shm_size);
    s32 sem_id = create_semaphore(shm_key);

    Warehouse *warehouse = (struct Warehouse *) shmat(shm_id, NULL, 0);
    if (warehouse == (void*) -1) {
        perror("shmat failed");
        return 1;
    }

    /* ----------------------------------- */
    /* CREATING FORKS & ALL MESSAGE QUEUES */
    /* ----------------------------------- */

    // all sub-processes run their "main" function where the proper running loop is located
    // sub-process loop functions are of bool type, if anything fails - FALSE is returned to indicate an error upon closing
    // sub-processes communicate with the manager through message queues and use shared memory

    // run warehouse sub-process
    // push warehouse PID to dynamic list
    pid_t warehouse_id = fork();
    if (warehouse_id == 0) {
        RUN_LOOP(warehouse_id, run_warehouse_process(warehouse, shm_id, sem_id));
    }
    push_pid(&forks, warehouse_id);

    // run supplier sub-processes
    // push suppliers PIDs to dynamic list
    pid_t supplierX_id = fork();
    if (supplierX_id == 0) {
        RUN_LOOP(supplierX_id, run_supplier_process(warehouse, TYPE_X, shm_id, sem_id));
    }
    push_pid(&forks, supplierX_id);

    pid_t supplierY_id = fork();
    if (supplierY_id == 0) {
        RUN_LOOP(supplierY_id, run_supplier_process(warehouse, TYPE_Y, shm_id, sem_id));
    }
    push_pid(&forks, supplierY_id);

    pid_t supplierZ_id = fork();
    if (supplierZ_id == 0) {
        RUN_LOOP(supplierZ_id, run_supplier_process(warehouse, TYPE_Z, shm_id, sem_id));
    }
    push_pid(&forks, supplierZ_id);

    // run manufacturer sub-processes
    // push manufacturers PIDs to dynamic list
    pid_t manufacturerA_id = fork();
    if (manufacturerA_id == 0) {
        RUN_LOOP(manufacturerA_id, run_manufactuter_process("/qA", warehouse, shm_id, sem_id));
    }
    push_pid(&forks, manufacturerA_id);

    pid_t manufacturerB_id = fork();
    if (manufacturerB_id == 0) {
        RUN_LOOP(manufacturerB_id ,run_manufactuter_process("/qB", warehouse, shm_id, sem_id));
    }
    push_pid(&forks, manufacturerB_id);

    /* ------------------------------------------- */
    /* MANAGER - PARENT PROCESS ACTUAL FUNCTIONING */
    /* ------------------------------------------- */

    // main process loop, used to manage all sub-processes by sending commands to proper message queues
    // main process loop also contains the terminal UI

    // run manager parent process loop
    if (!run_manager_process(&forks, warehouse, shm_id, sem_id)) {
        char error_message[256];
        snprintf(error_message, sizeof(error_message), "[%d] process failed", getpid());
        perror(error_message);
    }

    // unlink all queues
    MQ_UNLINK("/qW");
    MQ_UNLINK("/qA");
    MQ_UNLINK("/qB");

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