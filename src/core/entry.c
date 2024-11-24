#define PROCESS_COUNT 5

#include "ipc_utils.h"
#include "core/manager.h"

int main(void) {
    // generate ipc keys and shared mem keys
    printf("Gen keys...\n");
    size_t shm_size = sizeof(Managment);
    key_t mng_key = ftok("../bin/fabryka", 1);

    printf("Gen shared mem...\n");
    s32 shm_id = create_shared_memory(mng_key, shm_size);
    s32 sem_id = create_semaphore(mng_key);

    Managment *manager = (struct Managment *) shmat(shm_id, NULL, 0);
    if (manager == (void *)-1) {
        perror("shmat");
        return 1;
    } else {
        printf("Mem pointer created!\n");
    }

    pid_t warehouse_id = fork();
    if (warehouse_id == 0) {
        // warehouse process
        printf("Fork created!\n");
        run_warehouse_process(&manager->warehouse, shm_id, sem_id);
        exit(0);
    }

    run_manager_process(manager, warehouse_id);

    if (shmdt(manager) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    printf("All processes exited successfully! Shared memory released!\n");
    return 0;
}