#include "core/manufacturer.h"

#define MIN_TIME 5
#define MAX_TIME 10

b8 manufacturerIsRunning = TRUE;
message_t manMessageBuffer;
u32 manPriority;

b8 run_manufactuter_process(const char *queue, Warehouse *warehouse, s32 shm_id, s32 sem_id) {
    // starting manufacturer process
    #if DEBUG == 1
        printf("MANUFACTURER SUB-PROCESS: %d\n", getpid());
    #endif

    mqd_t qManufacturer = mq_open(queue, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attributes);
    if (qManufacturer == (mqd_t)-1) {
        perror("Failed to open queue");
        return FALSE;
    }

    // message buffer cleanup before loop start
    memset(&manMessageBuffer, 0, sizeof(message_t));

    // wait lock to not go into while loop without isOpen == TRUE
    while(!warehouse->isOpen) {}

    // main manufacturer process running loop
    while(manufacturerIsRunning) {
        #if USE_SLEEP == 1
            sleep((rand() % (MAX_TIME - MIN_TIME + 1) + MIN_TIME));
        #endif

        // listening for command on message queue
        if (mq_receive(qManufacturer, (char *)&manMessageBuffer, sizeof(manMessageBuffer), &manPriority) == -1) {}
        else {
            #if DEBUG == 1
                printf("[%d]Message recieved! Command no. %d | Message: %s\n", getpid(), manMessageBuffer.commandID, manMessageBuffer.message);
            #endif 
            switch (manMessageBuffer.commandID) {
                case 2:
                    manufacturerIsRunning = FALSE;
                    break;
                case 3:
                    manufacturerIsRunning = FALSE;
                    break;
                case 4:
                    manufacturerIsRunning = FALSE;
                    break;
                default:
                    break;
            }
            if (manufacturerIsRunning == FALSE) { continue; }      
        }

        // "manufacturing a part" and updating warehouse data
        sem_lock(sem_id);
        if (warehouse->isOpen) {
            if (warehouse->capacity < (DEFAULT_CAP - 6) && warehouse->partX > 0 && warehouse->partY > 0 && warehouse->partZ > 0) {
                warehouse->partX -= 1; warehouse->partY -= 1; warehouse->partZ -= 1;
                warehouse->capacity += 6;
            }
            else {
                printf("Warehouse is empty!\n");
            }
        }
        sem_unlock(sem_id);
    }

    return TRUE;
}