#ifndef IO_UTILS
#define IO_UTILS
#include <stdlib.h>
#include "mzapo_phys.h"
#include "mzapo_regs.h"


void *init_memory(){
    void *base =  map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (base == NULL) exit(1);
    return base;
}

#endif