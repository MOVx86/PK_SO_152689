// DOC-MISSING
// UNFINISHED

#include "ui/ui_terminal.h"

b8 uiIsRunning = 1;
message_t messageBuffer;
u32 priority;

void read_pid_list(pid_list *processes) {
    FILE *pidList = fopen("./pidList", "r");
    if (pidList == NULL) {
        perror("read_pid_list");
        return;
    }

    s32 lines = 0;
    s32 ch;
    while ((ch = fgetc(pidList)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }
    rewind(pidList);

    s32 current_pid;
    for (size_t i = 0; i < lines; ++i) {
        fscanf(pidList, "%d", &current_pid);
        #if DEBUG == 1
            printf("%d\n", current_pid);
        #endif
        push_pid(processes, current_pid);
    }

    fclose(pidList);
}

// definition of message queue attributes
struct mq_attr attributes = {
    .mq_flags   = 0,
    .mq_maxmsg  = 10,
    .mq_msgsize = sizeof(message_t),
    .mq_curmsgs = 0
};

/* ----------------------- */
/* SEPARATE UI ENTRY POINT */
/* ----------------------- */

int main(s32 argc, s8 *argv[]) {
    // check for correct argument count
    if (argc < 3) {
        fprintf(stderr, "Incorrect arguents! Correct: %s <shm_id> <sem_id>\n", argv[0]);
        exit(1);
    }

    // open manager message queue
    mqd_t qInterface = mq_open("/qI", O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attributes);

    // initialize pid list inside UI process, write to it from pid list text file
    pid_list forks;
    init_pid_list(&forks);
    read_pid_list(&forks);

    #if DEBUG == 1
        for (size_t i = 0; i < forks.size; ++i) {
            printf("%d\n", forks.pids[i]);
        }
        sleep(5);
    #endif

    // get shared memory ID and semaphor ID
    s32 shm_id = atoi(argv[1]);
    s32 sem_id = atoi(argv[2]);

    // attached shared memory to UI process (semaphor is already created and handled by main process)
    Warehouse *warehouse = (Warehouse *) shmat(shm_id, NULL, 0);
    if (warehouse == (void*) -1) {
        perror("shmat failed");
        return 1;
    }

    // initialize UI
    initscr();
    noecho();
    curs_set(FALSE);

    WINDOW *warehouseWindow = newwin(8, 42, 1, 1);
    WINDOW *processWindow = newwin(11, 42, 10, 1);

    /* -------------------- */
    /* STARTING THE UI LOOP */
    /* -------------------- */

    // wait for warehouse to open before displaying UI
    while (warehouse->isOpen != TRUE);
    clear();

    // main UI loop
    while (uiIsRunning) {
        // lock semaphor

        // update display data
        // window 1 (warehouse data)

        char whStatusString[40];
        if (warehouse->isOpen) {
            snprintf(whStatusString, sizeof(whStatusString), " Warehouse is OPEN                     ");
        }
        else {
            snprintf(whStatusString, sizeof(whStatusString), " Warehouse is CLOSED                   ");
        }
    
        box(warehouseWindow, 0, 0);
        mvwprintw(warehouseWindow, 1, 1, " WAREHOUSE SUMMARY                      ");
        mvwprintw(warehouseWindow, 2, 1, " Capcity: %28d ", warehouse->capacity);
        mvwprintw(warehouseWindow, 3, 1, " Part X count: %23d ", warehouse->partX);
        mvwprintw(warehouseWindow, 4, 1, " Part Y count: %23d ", warehouse->partY);
        mvwprintw(warehouseWindow, 5, 1, " Part Z count: %23d ", warehouse->partZ);
        mvwprintw(warehouseWindow, 6, 1, "%s", whStatusString);

        // window 2 (process info)
        box(processWindow, 0, 0);
        mvwprintw(processWindow, 1, 1, " PROCESS LIST                           ");
        mvwprintw(processWindow, 2, 1, " Manager (parent) process: %12d ", forks.pids[0]);
        mvwprintw(processWindow, 3, 1, " UI process: %26d ", forks.pids[1]);
        mvwprintw(processWindow, 4, 1, " Warehouse process: %19d ", forks.pids[2]);
        mvwprintw(processWindow, 5, 1, " Supplier X process: %18d ", forks.pids[3]);
        mvwprintw(processWindow, 6, 1, " Supplier Y process: %18d ", forks.pids[4]);
        mvwprintw(processWindow, 7, 1, " Supplier Z process: %18d ", forks.pids[5]);
        mvwprintw(processWindow, 8, 1, " Manufacturer A process: %14d ", forks.pids[6]);
        mvwprintw(processWindow, 9, 1, " Manufacturer B process: %14d ", forks.pids[7]);

        // refresh window display
        wrefresh(warehouseWindow);
        wrefresh(processWindow);

        // check message queue for commands
        if (mq_receive(qInterface, (char *)&messageBuffer, sizeof(messageBuffer), &priority) == -1) {}
        else {
            if (messageBuffer.commandID == 5) {
                uiIsRunning = 0;
            }
        }

        // 100 ms sleep to avoid flickering
        usleep(1000000);
    }

    endwin();
    return 0;
}