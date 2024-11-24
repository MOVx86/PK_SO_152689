#include "core/manager.h"

b8 run_manager_process(Managment *manager, pid_t pid_id) {
    printf("Starting manager process...\n");
    s32 status;

    printf("Managing warehouse process: %d\n", pid_id);
    sleep(30);

    printf("Closing warehouse process...\n");
    manager->warehouse.status = EXIT;

    waitpid(pid_id, &status, 0);
    sleep(3);

    printf("Closing manager process...\n");
    return TRUE;
}