// DOC-MISSING
// FINISHED

#include "core/warehouse.h"
#define MIN_TIME 10
#define MAX_TIME 30

b8 isRunning = TRUE;
message_t messageBuffer;
u32 priority;

void load_warehouse_record(Warehouse *warehouse) {
    // open warehouse record file, if it's empty/doesn't exist - load default values
    // otherwise read values from text file
    FILE *warehouseLog = fopen("./warehouseLog", "r");
    if (warehouseLog == NULL) {
        perror("load_record");

        warehouse->capacity = DEFAULT_CAP;
        warehouse->partX = DEFAULT_X;
        warehouse->partY = DEFAULT_Y;
        warehouse->partZ = DEFAULT_Z;
        warehouse->isOpen = TRUE;

        return;
    }

    fseek(warehouseLog, 0, SEEK_END);
    u64 fileSize = ftell(warehouseLog);

    if (fileSize == 0) {
        warehouse->capacity = DEFAULT_CAP;
        warehouse->partX = DEFAULT_X;
        warehouse->partY = DEFAULT_Y;
        warehouse->partZ = DEFAULT_Z;
        warehouse->isOpen = TRUE;       
    }
    else {
        fseek(warehouseLog, 0, SEEK_SET);
        fscanf(warehouseLog, "%u %u %u %u", &warehouse->capacity, &warehouse->partX, &warehouse->partY, &warehouse->partZ);
        warehouse->isOpen = TRUE;
    }

    fclose(warehouseLog);
}

void write_warehouse_record(Warehouse * warehouse) {
    FILE *warehouseLog = fopen("./warehouseLog", "w");
    if (warehouseLog == NULL) {
        perror("write_record");
        return;
    }

    fprintf(warehouseLog, "%u\n%u\n%u\n%u\n", warehouse->capacity, warehouse->partX, warehouse->partY, warehouse->partZ);

    fclose(warehouseLog);
}

b8 run_warehouse_process(Warehouse *warehouse, s32 shm_id, s32 sem_id) { 
    // starting warehouse process
    #if DEBUG == 1
        printf("\nWAREHOUSE SUB-PROCESS: %d\n", getpid());
    #endif

    // opening the message queue used bo the warehouse (manager -> warehouse)
    mqd_t qWarehouse = mq_open("/qW", O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attributes);
    if (qWarehouse == (mqd_t)-1) {
        perror("Failed to open queue");
        return FALSE;
    }

    // initializing warehouse values
    // values are loaded from a text file (warehouse records)
    // if no file is found, load default values
    sem_lock(sem_id);
    load_warehouse_record(warehouse);
    sem_unlock(sem_id);

    // message buffer cleanup before loop start
    memset(&messageBuffer, 0, sizeof(message_t));

    // warehouse process running loop
    while (isRunning) {
        #if USE_SLEEP == 1
            sleep((rand() % (MAX_TIME - MIN_TIME + 1) + MIN_TIME));
        #endif

        // locking semaphor -> listening for commands from message queue, displaying current warehouse state
        sem_lock(sem_id);

        if (mq_receive(qWarehouse, (char *)&messageBuffer, sizeof(messageBuffer), &priority) == -1) {}
        else {
            #if DEBUG == 1
                printf("[%d]Message recieved! Command no. %d | Message: %s\n", getpid(), messageBuffer.commandID, messageBuffer.message);
            #endif
            switch (messageBuffer.commandID) {
                case 1:
                    isRunning = FALSE;
                    warehouse->isOpen = FALSE;
                    break;
                case 3:
                    write_warehouse_record(warehouse);
                    isRunning = FALSE;
                    warehouse->isOpen = FALSE;
                    break;
                case 4:
                    isRunning = FALSE;
                    warehouse->isOpen = FALSE;
                    break;
                default:
                    break;
            }
        }

        #if DEBUG == 1
            printf("Test display: C = %u, X = %u, Y = %u, Z = %u\n", warehouse->capacity, warehouse->partX, warehouse->partY, warehouse->partZ);
        #endif

        sem_unlock(sem_id);
    }

    return TRUE;
}