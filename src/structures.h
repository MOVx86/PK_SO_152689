// DOC-MISSING
// FINISHED

#pragma once

#include "defines.h"

// extern declaration of message queue attributes - used in different parts of code
extern struct mq_attr attributes;

// structure sent by message queues
typedef struct message_t {
    s32 commandID;
    u8  message[256];
} message_t;

typedef enum PART_TYPE {
    TYPE_X = 1,
    TYPE_Y = 2,
    TYPE_Z = 3
} PART_TYPE;

typedef struct Warehouse {
    volatile u32 partX;
    volatile u32 partY;
    volatile u32 partZ;
    volatile u32 capacity;
    b8 isOpen;
} Warehouse;

