// DOC-MISSING

#include "core/manager.h"

#define MMQ_SEND(queue, queueName, messagePtr, messageSize, priority)                   \
    do {                                                                                \
        if(mq_send(queue, (const char *)(messagePtr), messageSize, priority) == -1) {   \
            perror("Failed to send message");                                           \
            mq_close(queue);                                                            \
            mq_unlink(queueName);                                                       \
        }                                                                               \
    } while (0)                                                                         \

u32 shouldExit = 0;
b8 pendingSignal = 0;
message_t pendingMessage;

void signal_handler(s32 signal) {
    printf("Signal caught!\n");
    switch (signal) {
        case SIGUSR1:
            memset(&pendingMessage, 0, sizeof(message_t));
            pendingMessage.commandID = 1;
            strncpy(pendingMessage.message, "Close down Warehouse.", sizeof(pendingMessage.message));
            pendingMessage.message[sizeof(pendingMessage.message) - 1] = '\0';
            pendingSignal = 1;
            break;
        case SIGUSR2:
            memset(&pendingMessage, 0, sizeof(message_t));
            pendingMessage.commandID = 2;
            strncpy(pendingMessage.message, "Close down Manufacturers.", sizeof(pendingMessage.message));
            pendingMessage.message[sizeof(pendingMessage.message) - 1] = '\0';
            pendingSignal = 1;
            break;
        case SIGINT:
            memset(&pendingMessage, 0, sizeof(message_t));
            pendingMessage.commandID = 3;
            strncpy(pendingMessage.message, "Close down All, keep records.", sizeof(pendingMessage.message));
            pendingMessage.message[sizeof(pendingMessage.message) - 1] = '\0';
            pendingSignal = 1;
            break;
        case SIGTSTP:
            memset(&pendingMessage, 0, sizeof(message_t));
            pendingMessage.commandID = 4;
            strncpy(pendingMessage.message, "Close down All, do not keep records.", sizeof(pendingMessage.message));
            pendingMessage.message[sizeof(pendingMessage.message) - 1] = '\0';
            pendingSignal = 1;
            break;
        case SIGTERM:
            memset(&pendingMessage, 0, sizeof(message_t));
            pendingSignal = 1;
            shouldExit = 1;
            break;
        default:
            printf("Signal unrecognized!\n");
            break;
    }
}

b8 run_manager_process(pid_list *processes, Warehouse *warehouse, s32 shm_id, s32 sem_id) {
    // starting manager process
    #if DEBUG == 1
        printf("MANAGER PROCESS: %d\n", getpid());
    #endif

    s32 status;

    // opening all queues - O_NONBLOCK to keep the loop running
    mqd_t qWarehouse = mq_open("/qW", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &attributes);
    mqd_t qManufacturerA = mq_open("/qA", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &attributes);
    mqd_t qManufacturerB = mq_open("/qB", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &attributes);

    // signal handlers
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    signal(SIGINT,  signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGTERM, signal_handler);

    // initialize and set up UI (ncrses)
    initscr();
    noecho();
    curs_set(TRUE);

    int uiWidth = 30;
    int warehouseUiHeight = 9;
    int processesUiHeight = 8;

    WINDOW *warehouseWindow = newwin(warehouseUiHeight + processesUiHeight, uiWidth, 0, 0);
    WINDOW *processWindow = newwin(processesUiHeight, uiWidth, warehouseUiHeight, 0);

    // parent process running loop
    // listening for signals & sending messages to sub-processes
    // TODO terminal UI
    while (!shouldExit) {
        // updating terminal UI
        werase(warehouseWindow);
        werase(processWindow);

        char warehouseState[30];
        if (warehouse->isOpen) {
            strncpy(warehouseState, "|Warehouse is open.          |", sizeof(warehouseState));
        } else {
            strncpy(warehouseState, "|Warehouse is closed.        |", sizeof(warehouseState));
        }

        // updating contents of warehouse segment
        mvwprintw(warehouseWindow, 0, 0, "|----------------------------|");
        mvwprintw(warehouseWindow, 1, 0, "|Current state of warehouse: |");
        mvwprintw(warehouseWindow, 2, 0, "|Capacity: %-17d |", warehouse->capacity);
        mvwprintw(warehouseWindow, 3, 0, "|Part X: %-19d |", warehouse->partX);
        mvwprintw(warehouseWindow, 4, 0, "|Part Y: %-19d |", warehouse->partY);
        mvwprintw(warehouseWindow, 5, 0, "|Part Z: %-19d |", warehouse->partZ);
        mvwprintw(warehouseWindow, 6, 0, "%s", warehouseState);
        mvwprintw(warehouseWindow, 7, 0, "|----------------------------|");

        // updating contents of processes segment
        mvwprintw(processWindow, 0, 0, "|Current running processes:  |");
        mvwprintw(processWindow, 1, 0, "|Main process: %-13u |", getpid());
        for (size_t i = 0; i < processes->size; ++i) {
            mvwprintw(processWindow, i+2, 0, "|[PID]: %-20u |", processes->pids[i]);
        }
        mvwprintw(processWindow, 7, 0, "|----------------------------|");
        
        wrefresh(warehouseWindow);
        wrefresh(processWindow);

        // listening for signals
        if (pendingSignal) {
            #if DEBUG == 1
                printf("Current message:\n%s\ncommand ID: %d\n", pendingMessage.message, pendingMessage.commandID);
            #endif
            switch (pendingMessage.commandID) {
                case 1:
                    MMQ_SEND(qWarehouse, "/qW", &pendingMessage, sizeof(message_t), 1);
                    pendingSignal = 0;
                    break;
                case 2:
                    MMQ_SEND(qManufacturerA, "/qA", &pendingMessage, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerB, "/qB", &pendingMessage, sizeof(message_t), 1);
                    pendingSignal = 0;
                    break;
                case 3:
                    MMQ_SEND(qWarehouse, "/qW", &pendingMessage, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerA, "/qA", &pendingMessage, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerB, "/qB", &pendingMessage, sizeof(message_t), 1);
                    pendingSignal = 0;
                    break;
                case 4:
                    MMQ_SEND(qWarehouse, "/qW", &pendingMessage, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerA, "/qA", &pendingMessage, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerB, "/qB", &pendingMessage, sizeof(message_t), 1);
                    pendingSignal = 0;
                    break;
                default:
                    pendingSignal = 0;
                    break;
            }
        }
    }

    // check if all sub-processes closed correctly
    for (size_t i = 0; i < processes->size; ++i) {
        #if DEBUG == 1
            printf("Closing process %d\n", processes->pids[i]);
        #endif
        if(waitpid(processes->pids[i], &status, 0) == -1) {
            perror("Failed to close process!");
        };
    }

    return TRUE;
}