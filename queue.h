#include "common.h"

#ifndef INPUT_QUEUE
#define INPUT_QUEUE

typedef enum InputEvent {
    ROTATE_LEFT_GREEN,
    ROTATE_RIGHT_GREEN,
    ROTATE_LEFT_BLUE,
    ROTATE_RIGHT_BLUE,
    RED_KEY,
    BLUE_KEY,
    GREEN_KEY,
    NONE
} InputEvent;

typedef struct Input {
    InputEvent event;
    struct Input *next;
} Input;

typedef struct {
    Input *start;
    int count;
    bool lock;
} Queue;

void enqueue_input_event(Queue *queue, InputEvent event);
InputEvent dequeu_input_event(Queue *queue);

bool acquire_queue_lock(Queue *queue);
void release_queue_lock(Queue *queue);

void wait_for_queue_lock(Queue *queue);

Queue init_queue();
void free_queue();

#endif