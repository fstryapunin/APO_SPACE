#include "common.h"

#define INPUT_DELAY_MS 50

typedef struct
{
    double acceleration_input;
    double rotation_input;
    bool pause_input;
    bool map_input;
    bool stop;
} Input;

typedef struct
{
    Input *input;
    bool stop;
    unsigned char *mem_base;
} InputArgs;


Input init_input(unsigned char *mem_base);
void *loop_input_collection(InputArgs *args);

