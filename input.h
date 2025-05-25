#include "common.h"

#define INPUT_DELAY_MS 50

typedef struct
{
    double acceleration_input;
    double rotation_input;
    bool pause_input;
    bool map_input;
} Input;

typedef struct
{
    Input *input;
    bool stop;
} InputArgs;


Input init_input();
void *loop_input_collection(InputArgs *args);

