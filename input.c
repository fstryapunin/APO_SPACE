#include "input.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#define BLUE_KNOB_BITWISE_OFFSET 24
#define GREEN_KNOB_BITWISE_OFFSET 16
#define KNOB_MAX_VALUE 255

static int acc_offset = 0;
static int rotation_offset = 0;

Input init_input(unsigned char *mem_base){
    Input input;
    uint32_t rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    rotation_offset = (int)((rgb_knobs_value << BLUE_KNOB_BITWISE_OFFSET) >> BLUE_KNOB_BITWISE_OFFSET);
    acc_offset = (int)((rgb_knobs_value << GREEN_KNOB_BITWISE_OFFSET) >> BLUE_KNOB_BITWISE_OFFSET);

    printf("offsets %u %u\n", rotation_offset, acc_offset);

    return (Input){0, 0, false, false};
}

double map_to_rads(int input) {
    if(input < 0) return (double)0;
    if(input > 40) return 2 * M_PI;
 
    return ((double)input / 40.0) * (2 * M_PI);
}


double map_to_accel(int input){
    if(input < 0) return (double)0;
    if(input > 40) return 10;
    
    return (double)input / 4;
}

void get_input(Input *input, unsigned char *mem_base){
    uint32_t rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    uint32_t rotation_val = (rgb_knobs_value << BLUE_KNOB_BITWISE_OFFSET) >> BLUE_KNOB_BITWISE_OFFSET;
    uint32_t acceleration_val = (rgb_knobs_value << GREEN_KNOB_BITWISE_OFFSET) >> BLUE_KNOB_BITWISE_OFFSET;

    bool pause_val = (rgb_knobs_value << 7) >> 31;
    bool map_val = (rgb_knobs_value << 6) >> 31;

    // printf("\r%lf %lf", map_to_rads((int)rotation_val - rotation_offset), map_to_accel((int)acceleration_val - acc_offset));

    input->rotation_input = map_to_rads((int)rotation_val - rotation_offset);
    input->acceleration_input = map_to_accel((int)acceleration_val - acc_offset);
    input->pause_input = pause_val != 0;
    input->map_input = map_val != 0;
};

void *loop_input_collection(InputArgs *args){
    while (!args->stop)
    {
        get_input(args->input, args->mem_base);
        // printf("\rInput acc: %lf rot: %lf pause: %d, map: %d", 
        //     args->input->acceleration_input, 
        //     args->input->rotation_input,
        //     args->input->pause_input,
        //     args->input->map_input
        // );
        fflush(stdout);
        
        usleep(INPUT_DELAY_MS * 1000);
    }
}