#include "core/warehouse.h"
#define MIN_TIME 10
#define MAX_TIME 30

b8 is_running = 1;
message_t message_buffer;
u32 priority;

b8 run_warehouse_process(Warehouse *warehouse, s32 shm_id, s32 sem_id) { 
    // starting warehouse process
    #if DEBUG == 1
        printf("\nWAREHOUSE SUB-PROCESS: %d\n", getpid());
    #endif

    mqd_t qWarehouse = mq_open("/qW", O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attributes);
    if (qWarehouse == (mqd_t)-1) {
        perror("Failed to open queue: \"/qW\"!");
        return FALSE;
    }

    // initializing warehouse values
    // TODO implement txt record reading
    warehouse->capacity = 100;
    warehouse->partX = 0;
    warehouse->partY = 0;
    warehouse->partZ = 0;

    // message buffer cleanup before loop start
    memset(&message_buffer, 0, sizeof(message_t));

    // warehouse process running loop
    while (is_running) {
        sleep((rand() % (MAX_TIME - MIN_TIME + 1) + MIN_TIME));
        printf("Warehouse is running\n");

        // TODO show current warehouse stock

        if(mq_receive(qWarehouse, (char *)&message_buffer, sizeof(message_buffer), &priority) == -1) {
            printf("Failed to recieve message!\n");
        } else {
            printf("Message recieved! Command no. %d | Message: %s\n", message_buffer.commandID, message_buffer.message);
            switch (message_buffer.commandID) {
                case 1:
                    printf("Signal recieved. Closing warehouse!\n");
                    is_running = 0;
                    break;
                case 3:
                    printf("Signal recieved. Closing warehouse, saving records!\n");
                    // TODO implement record saving in txt file
                    is_running = 0;
                    break;
                case 4:
                    printf("Signal recieved. Closing warehouse, not saving records!\n");
                    is_running = 0;
                    break;
                default:
                    printf("Command unrecognised!\n");
                    break;
            }
        }
    }

    return TRUE;
}