#include "defines.h"
#include "structures.h"
#include "ipc_utils.h"
#include "pid_array.h"
#include "core/manager.h"
#include "assert.h"

struct mq_attr attributes = {
    .mq_flags   = 0,
    .mq_maxmsg  = 10,
    .mq_msgsize = sizeof(message_t),
    .mq_curmsgs = 0
};

void simulate_pid_list_file() {
    FILE *pidList = fopen("../bin/pidList", "w");
    assert(pidList != NULL);

    for (int i = 0; i < 3; ++i) {
        fprintf(pidList, "%d\n", i+1);
        fflush(pidList);
    }

    fclose(pidList);
    printf("[TEST 2] Simulated text file with pid list.\n");
}

void read_pid_list(pid_list *processes) {
    FILE *pidList = fopen("../bin/pidList", "r");
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
    assert(lines != 0);

    s32 current_pid;
    for (size_t i = 0; i < lines; ++i) {
        fscanf(pidList, "%d", &current_pid);
        #if DEBUG == 1
            printf("From pid list: %d\n", current_pid);
        #endif
        push_pid(processes, current_pid);
    }

    fclose(pidList);
}

void test_pid_list();
void test_display();
void test_signal_handler();

/* ---------------- */
/* TEST ENTRY POINT */
/* ---------------- */

int main(void) {
    test_pid_list();
    test_display();
    test_signal_handler();

    return 0;
}

/* -------------------------- */
/* TEST FUNCTIONS DEFINITIONS */
/* -------------------------- */

void test_pid_list() {
    printf("[TEST 1] START!\n");
    pid_list forks;

    // test initialization
    init_pid_list(&forks);
    assert(forks.pids == NULL);
    assert(forks.size == 0);
    assert(forks.capacity == 0);

    // test pushing pid to list
    push_pid(&forks, 1);
    assert(forks.pids != NULL);
    assert(forks.pids[0] == 1);
    assert(forks.size == 1);
    assert(forks.capacity == 2);

    // test freeing pid list
    free_list(&forks);
    assert(forks.pids == NULL);
    assert(forks.size == 0);
    assert(forks.capacity == 0);

    printf("[TEST 1] PASSED!\n");
}

void test_display() {
    printf("[TEST 2] START!\n");
    // simulate text file with pid list
    simulate_pid_list_file();
    pid_list forks;
    read_pid_list(&forks);

    // initialize ui (ncurses)
    initscr();
    noecho();
    curs_set(FALSE);

    WINDOW *testWindow = newwin(5,10,1,1);
    assert(testWindow != NULL);

    FILE *pidList = fopen("../bin/pidList", "w");
    assert(pidList != NULL);

    int printResult;
    for (size_t i = 0; i < forks.size; ++i) {
        printResult = mvwprintw(testWindow, 1, 1, "%10d", forks.pids[i]);
        assert(printResult >= 0);
    }

    int boxResult = box(testWindow, 0, 0);
    assert(boxResult == OK);

    delwin(testWindow);
    endwin();

    printf("[TEST 2] Initialized window successfully.\n");
    printf("[TEST 2] Properly printed to window.\n");
    printf("[TEST 2] Properly boxed window.\n");
    printf("[TEST 2] PASSED!\n");
}

void test_signal_handler() {
    printf("[TEST 3] START!\n");
    // set up test signal handler
    signal(SIGINT, signal_handler);

    // send test signal
    raise(SIGINT);

    // check if signal was handled
    assert(pendingMessage.commandID == 3);
    assert(strcmp(pendingMessage.message, "Close down All, keep records.") == 0);
    assert(pendingSignal == 1);

    printf("[TEST 3] PASSED!\n");
}