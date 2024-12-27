#include "core/manager.h"

u32 should_exit = 0;

void close_wh(s32 signal) {
    // TODO send proper queue message
    printf("Caught signal SIGUSR1!\n");
}

void close_ma(s32 signal) {
    // TODO send proper queue message
    printf("Caught signal SIGUSR2!\n");
}

void close_keep_wh_ma(s32 signal) {
    // TODO send proper queue message
    printf("Caught signal SIGINT!\n");
}

void close_wh_ma(s32 signal) {
    // TODO send proper queue message
    printf("Caught signal SIGTSTP!\n");
}

void close_main(s32 signal) {
    printf("Caught signal SIGTERM!\n");
    should_exit = 1;
}

b8 run_manager_process(pid_list *processes, s32 shm_id, s32 sem_id) {
    printf("MANAGER PROCESS: %d\n", getpid());
    s32 status;

    // signal handlers
    signal(SIGUSR1, close_wh);
    signal(SIGUSR2, close_ma);
    signal(SIGINT, close_keep_wh_ma);
    signal(SIGTSTP, close_wh_ma);
    signal(SIGTERM, close_main);

    // parent process running loop (listening for signals)
    while (!should_exit) {
        pause();
    }

    for (int i = 0; i < processes->size; i++) {
        waitpid(processes->pids[i], &status, 0);
        printf("SUB-PROCESS: %d CLOSED SUCCESSFULLY\n", processes->pids[i]);
    }

    return TRUE;
}