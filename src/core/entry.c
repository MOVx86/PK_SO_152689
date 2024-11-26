#define PROCESS_COUNT 5

#include "ipc_utils.h"
#include "pid_array.h"
#include "core/manager.h"


int main(void) {
    // prepare dynamic list of pids
    pid_list forks;
    init_pid_list(&forks);

    // generate ipc keys and shared mem keys
    size_t shm_size = sizeof(Managment);
    key_t mng_key = ftok("../bin/fabryka", 1);

    s32 shm_id = create_shared_memory(mng_key, shm_size);
    s32 sem_id = create_semaphore(mng_key);

    Managment *manager = (struct Managment *) shmat(shm_id, NULL, 0);
    if (manager == (void *)-1) {
        perror("shmat");
        return 1;
    }

    // creating all forked processes
    pid_t warehouse_id = fork();
    if (warehouse_id == 0) {
        // warehouse process
        run_warehouse_process(&manager->warehouse, shm_id, sem_id);
        exit(0);
    }
    push_pid(&forks, warehouse_id);

    // manager process, controls all child-processes:
    // - warehouse
    // - manofacturers
    // - suppliers
    run_manager_process(manager, &forks);

    if (shmdt(manager) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}