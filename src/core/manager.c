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

#if RUN_UI == 1
    s32 terminal_fd = STDIN_FILENO; // monitoring STDIN for terminal input
#endif

void signal_handler(s32 signal) {
    #if DEBUG == 1
        printf("Signal caught!\n");
    #endif
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
    #if RUN_UI == 1
        initscr();
        raw();
        noecho();
        curs_set(FALSE);

        int uiWidth = 30;
        int warehouseUiHeight = 9;
        int processesUiHeight = 8;

        WINDOW *window = newwin(warehouseUiHeight + processesUiHeight + 1, uiWidth, 0, 0);

        // input processing
        fd_set readfds;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100;
    #endif

    // parent process running loop
    // listening for signals & sending messages to sub-processes
    // TODO terminal UI

    // input handling
    #if RUN_UI == 1
        s8 inputBuffer[256];
        s32 bufferIndex = 0;

        memset(inputBuffer, 0, sizeof(inputBuffer));
    #endif

    while (!shouldExit) {
        // updating terminal UI
        #if RUN_UI == 1

            werase(window);

            char warehouseState[30];
            if (warehouse->isOpen) {
                strncpy(warehouseState, "|Warehouse is open.          |", sizeof(warehouseState));
            } else {
                strncpy(warehouseState, "|Warehouse is closed.        |", sizeof(warehouseState));
            }

            // updating contents of warehouse segment
            mvwprintw(window, 0, 0, "|----------------------------|");
            mvwprintw(window, 1, 0, "|Current state of warehouse: |");
            mvwprintw(window, 2, 0, "|Capacity: %-17d |", warehouse->capacity);
            mvwprintw(window, 3, 0, "|Part X: %-19d |", warehouse->partX);
            mvwprintw(window, 4, 0, "|Part Y: %-19d |", warehouse->partY);
            mvwprintw(window, 5, 0, "|Part Z: %-19d |", warehouse->partZ);
            mvwprintw(window, 6, 0, "%s", warehouseState);
            mvwprintw(window, 7, 0, "|----------------------------|");

            // updating contents of processes segment
            mvwprintw(window, 8, 0, "|Current running processes:  |");
            mvwprintw(window, 9, 0, "|Main process: %-13u |", getpid());
            for (size_t i = 0; i < processes->size; ++i) {
                mvwprintw(window, i+10, 0, "|[PID]: %-20u |", processes->pids[i]);
            }
            mvwprintw(window, 16, 0, "|----------------------------|");
            mvwprintw(window, 17, 0, "%30s", inputBuffer);
            wmove(window, 17, 0);

            wrefresh(window);

            // waiting for input
            FD_ZERO(&readfds);
            FD_SET(terminal_fd, &readfds);

            int ready = select(terminal_fd + 1, &readfds, NULL, NULL, &timeout);
            if (ready > 0 && FD_ISSET(terminal_fd, &readfds)) {
                char input = getchar();
                if (input == 10) {
                    inputBuffer[bufferIndex] = '\0';

                    mvwprintw(window, 17, 0, "                              ");
                    wrefresh(window);

                    int status = system(inputBuffer);

                    bufferIndex = 0;
                    memset(inputBuffer, 0, sizeof(inputBuffer));
                }
                else if (input == 127) {
                    if (bufferIndex > 0) {
                        bufferIndex--;
                        int cursorPos = getcury(window);
                        wmove(window, cursorPos - 1, 0);
                        wdelch(window);
                        wrefresh(window);
                    }
                }
                else {
                    if (bufferIndex < sizeof(inputBuffer) - 1) {
                        inputBuffer[bufferIndex++] = input;
                    }

                    waddch(window, input);
                    wrefresh(window);
                }
            }
        #elif RUN_UI == 0
            system("clear");
            printf("Current warehouse state\nCAPACITY: %u\nPART X: %u\nPART Y: %u\nPART Z: %u\n", warehouse->capacity, warehouse->partX, warehouse->partY, warehouse->partZ);
            printf("Main process: %d\n", getpid());
            #if USE_SLEEP == 1
                sleep(15);
            #endif
            usleep(1000000);
        #endif

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

        #if USE_SLEEP == 1
            usleep(100000);
        #endif
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