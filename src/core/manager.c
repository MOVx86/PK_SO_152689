#include "core/manager.h"

#define MMQ_SEND(queue, queueName, messagePtr, messageSize, priority)                   \
    do {                                                                                \
        if(mq_send(queue, (const char *)(messagePtr), messageSize, priority) == -1) {   \
            printf("Failed to send message!");                                          \
            perror("Failed to send message!");                                          \
            mq_close(queue);                                                            \
            mq_unlink(queueName);                                                       \
        }                                                                               \
        else { printf("Message sent!\n");}                                              \
    } while (0)                                                                         \

u32 should_exit = 0;
message_t pending_message;
b8 pending_signal = 0;

void signal_handler(s32 signal) {
    printf("Signal caught!\n");
    switch (signal) {
        case SIGUSR1:
            memset(&pending_message, 0, sizeof(message_t));
            pending_message.commandID = 1;
            strcpy(pending_message.message, "Close down Warehouse.");
            pending_message.message[sizeof(pending_message.message) - 1] = '\0';
            pending_signal = 1;
            break;
        case SIGUSR2:
            memset(&pending_message, 0, sizeof(message_t));
            pending_message.commandID = 2;
            strcpy(pending_message.message, "Close down Manufacturers.");
            pending_message.message[sizeof(pending_message.message) - 1] = '\0';
            pending_signal = 1;
            break;
        case SIGINT:
            memset(&pending_message, 0, sizeof(message_t));
            pending_message.commandID = 3;
            strcpy(pending_message.message, "Close down All, keep records.");
            pending_message.message[sizeof(pending_message.message) - 1] = '\0';
            pending_signal = 1;
            break;
        case SIGTSTP:
            memset(&pending_message, 0, sizeof(message_t));
            pending_message.commandID = 4;
            strcpy(pending_message.message, "Close down All, do not keep records.");
            pending_message.message[sizeof(pending_message.message) - 1] = '\0';
            pending_signal = 1;
            break;
        case SIGTERM:
            memset(&pending_message, 0, sizeof(message_t));
            printf("Closing down manager and all sub-processes.");
            pending_signal = 1;
            should_exit = 1;
            break;
        default:
            printf("Signal unrecognized!\n");
            break;
    }
}

b8 run_manager_process(pid_list *processes, s32 shm_id, s32 sem_id) {
    // starting manager process (and adding it to hash map)
    printf("MANAGER PROCESS: %d\n", getpid());
    s32 status;

    mqd_t qWarehouse = mq_open("/qW", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &attributes);
    mqd_t qManufacturerA = mq_open("/qA", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &attributes);
    mqd_t qManufacturerB = mq_open("/qB", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &attributes);

    // signal handlers
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    signal(SIGINT,  signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGTERM, signal_handler);

    // parent process running loop (listening for signals)
    while (!should_exit) {
        if (pending_signal) {
            printf("Current message:\n%s\ncommand ID: %d\n", pending_message.message, pending_message.commandID);
            switch (pending_message.commandID) {
                case 1:
                    MMQ_SEND(qWarehouse, "/qW", &pending_message, sizeof(message_t), 1);
                    pending_signal = 0;
                    break;
                case 2:
                    MMQ_SEND(qManufacturerA, "/qA", &pending_message, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerB, "/qB", &pending_message, sizeof(message_t), 1);
                    pending_signal = 0;
                    break;
                case 3:
                    MMQ_SEND(qWarehouse, "/qW", &pending_message, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerA, "/qA", &pending_message, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerB, "/qB", &pending_message, sizeof(message_t), 1);
                    pending_signal = 0;
                    break;
                case 4:
                    MMQ_SEND(qWarehouse, "/qW", &pending_message, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerA, "/qA", &pending_message, sizeof(message_t), 1);
                    MMQ_SEND(qManufacturerB, "/qB", &pending_message, sizeof(message_t), 1);
                    pending_signal = 0;
                    break;
                default:
                    pending_signal = 0;
                    break;
            }
        }
        sleep(10);
        printf("Manager is running\n");
    }

    // check if warehouse closed (or force close)
    for (size_t i = 0; i < processes->size; ++i) {
        printf("Closing process %d\n", processes->pids[i]);
        if(waitpid(processes->pids[i], &status, 0) == -1) {
            perror("Failed to close process!");
        };
    }

    return TRUE;
}