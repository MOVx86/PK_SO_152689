#include "core/manager.h"

b8 run_manager_process(Managment *manager, pid_list *list) {
    printf("Starting manager process...\n");
    s32 status;

    printf("Managing warehouse process: %d\n", list->pids[0]);
    sleep(30);

    printf("Closing warehouse process...\n");
    manager->warehouse.status = EXIT;

    waitpid(list->pids[0], &status, 0);
    sleep(3);

    printf("Closing manager process...\n");
    return TRUE;
}