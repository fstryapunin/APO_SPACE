#include "input.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#define BLUE_KNOB_BITWISE_OFFSET 24
#define GREEN_KNOB_BITWISE_OFFSET 16
#define KNOB_MAX_VALUE 255

void *init_memory(){
    void *base =  map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (base == NULL) exit(1);
    return base;
}

Input init_input(){
    return (Input){ 0, 0, false, false};
}

double mapKnobToRads(unsigned short value) {
    return ((double)value / (double)KNOB_MAX_VALUE) * (2 * M_PI);
}

double mapKnobToAccelerationRange(unsigned short value) {
    return ((double)value / (double)KNOB_MAX_VALUE) * 10;
}

void get_input(Input *input, unsigned char *mem_base){
    uint32_t rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    uint8_t rotation_val = (rgb_knobs_value << BLUE_KNOB_BITWISE_OFFSET) >> BLUE_KNOB_BITWISE_OFFSET;
    uint8_t acceleration_val = (rgb_knobs_value << GREEN_KNOB_BITWISE_OFFSET) >> BLUE_KNOB_BITWISE_OFFSET;
    bool pause_val = (rgb_knobs_value << 7) >> 31;
    bool map_val = (rgb_knobs_value << 6) >> 31;

    input->rotation_input = mapKnobToRads(rotation_val);
    input->acceleration_input = mapKnobToAccelerationRange(acceleration_val);
    input->pause_input = pause_val != 0;
    input->map_input = map_val != 0;
};

void *loop_input_collection(InputArgs *args){
    unsigned char *mem_base = init_memory();
    while (!args->stop)
    {
        get_input(args->input, mem_base);
        printf("Input acc: %lf rot: %lf pause: %d, map: %d\n", 
            args->input->acceleration_input, 
            args->input->rotation_input,
            args->input->pause_input,
            args->input->map_input
        );
        
        usleep(INPUT_DELAY_MS * 1000);
    }
}