#include "queue.h"

void enqueue_input_event(Queue *queue, InputEvent event){
    Input *input = malloc(sizeof(Input));
    input->event = event;

    Input *last = queue->start;

    if(last == NULL){
        queue->start = input;
        return;
    }

    while (last->next != NULL)
    {
        last = last->next;
    }    

    last->next = input;
};

InputEvent dequeu_input_event(Queue *queue){
    Input *next = queue->start;
    InputEvent event = next->event;

    queue->start = next->next;
    free(next);

    return event;
};

Queue init_queue(){
    return (Queue){ NULL, 0, false };
}

void free_queue(Queue *queue){
    Input *input = queue->start;

    while (input != NULL)
    {
        Input *temp = input->next;
        free(input);
        input = temp;   
    }
    
    queue->count = 0;
};

bool acquire_queue_lock(Queue *queue){
    if(queue->lock) return false;

    queue->lock = true;

    return true;
};

void release_queue_lock(Queue *queue){
    queue->lock = false;
};

void wait_for_queue_lock(Queue *queue){
    while(!acquire_queue_lock(queue)){
        usleep(100);
    };
}