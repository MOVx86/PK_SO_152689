#include "ipc_utils.h"

s32 create_shared_memory(key_t key, size_t size) {
    s32 shm_id = shmget(key, size, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    return shm_id;
}

s32 create_semaphore(key_t key) {
    int sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("shmget");
        exit(1);
    }

    return sem_id;
}

// TODO understand how semaphor locking works
void sem_lock(s32 sem_id) {
    struct sembuf sb = {0, -1, 0};
    semop(sem_id, &sb, 1);
}

void sem_unlock(s32 sem_id) {
    struct sembuf sb = {0, 1, 0};
    semop(sem_id, &sb, 1);    
}