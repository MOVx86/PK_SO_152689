#pragma once

#include "defines.h"
#include "ipc_utils.h"
#include "core/warehouse.h"
#include "core/manofacturers.h"
#include "core/suppliers.h"

typedef struct Managment {
    Warehouse warehouse;
    // TODO manufactures;
    // TODO suppliers;
    
} Managment;

void command_1();
void command_2();
void command_3();
void command_4();

b8 run_manager_process(Managment *manager, pid_t pid_id);
