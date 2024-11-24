#include "core/warehouse.h"

b8 run_warehouse_process(Warehouse *warehouse, s32 shm_id, s32 sem_id) {
    printf("Starting warehouse child-process...\n");
    
    // set up initial values
    // TODO check for document with values from last running warehouse process
    warehouse->partX          = 0;
    warehouse->partY          = 0;
    warehouse->partZ          = 0;
    warehouse->capacity       = 100;
    warehouse->status         = RUNNING;
    warehouse->document       = NGEN_DOC_ON_TERMINATION;
    warehouse->capacityStatus = EMPTY;

    // running loop
    while (1) {
        // TODO check for doc update signal (off by default)
        // TODO check for terminate signal

        printf (
            "Stan magazynowy:\nX = %d, Y = %d, Z = %d, Pozostałe miejsce = %d, Status: %d\n",
            warehouse->partX, warehouse->partY, warehouse->partZ, warehouse->capacity, warehouse->status
            );
        
        sleep(3); //temporary
        // TODO add connection to delivery and manufacture

        if (warehouse->status == EXIT) {
            if (warehouse->document == GEN_DOC_ON_TERMINATION) {
                // TODO generate/update text document
            }
            break;
        };
    }

    printf("Closing warehouse child-process...\n");
    return TRUE;
}