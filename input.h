#include "common.h"
#include "queue.h"

#define INPUT_DELAY_MS 10

typedef struct
{
    Queue *input_queue;
    bool *stop;
} InputArgs;

void *loop_input_collection(InputArgs *args);
