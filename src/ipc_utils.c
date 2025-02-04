// DOC-MISSING
// UNFINISHED

#include "ipc_utils.h"

// create shared memory key
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

    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl");
        exit(1);
    }

    return sem_id;
}

// locking and unlocking semaphors
void sem_lock(s32 sem_id) {
    struct sembuf sb = {0, -1, 0};
    semop(sem_id, &sb, 1);
}

void sem_unlock(s32 sem_id) {
    struct sembuf sb = {0, 1, 0};
    semop(sem_id, &sb, 1);    
}