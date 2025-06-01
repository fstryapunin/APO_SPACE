#include "input.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#define BLUE_KNOB_BITWISE_OFFSET 24
#define GREEN_KNOB_BITWISE_OFFSET 16

#define BLUE_BUTTON_BITWISE_OFFSET 7
#define GREEN_BUTTON_BITWISE_OFFSET 6
#define RED_BUTTON_BITWISE_OFFSET 5

#define KNOB_MAX_VALUE 255

void *init_memory()
{
    void *base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (base == NULL)
        exit(1);
    return base;
}

uint32_t get_knobs_input(unsigned char *mem_base)
{
    return *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
};

uint32_t parse_knob_input(uint32_t input_value, int offset)
{
    return (input_value << offset) >> BLUE_KNOB_BITWISE_OFFSET;
};

uint32_t parse_button_value(uint32_t input_value, int offset)
{
    return (input_value << offset) >> 31;
}

bool detect_button_raising_edge(uint32_t previous_raw_knobs_value, uint32_t raw_knobs_value, int offset)
{
    return parse_button_value(raw_knobs_value, offset) > parse_button_value(previous_raw_knobs_value, offset);
}

bool detect_overflow(uint32_t prev, uint32_t new)
{
    return abs(prev - new) > KNOB_MAX_VALUE / 2;
}

int get_knob_values_difference(uint32_t previous_raw_knobs_value, uint32_t raw_knobs_value, int offset)
{
    int knob_previus_value = (int)parse_knob_input(previous_raw_knobs_value, offset);
    int knob_value = (int)parse_knob_input(raw_knobs_value, offset);

    int multiplier = detect_overflow(knob_previus_value, knob_value) ? -1 : 1;

    if (knob_value > knob_previus_value)
    {
        return multiplier * 1;
    }

    if (knob_value < knob_previus_value)
    {
        return multiplier * -1;
    }

    return 0;
}

static uint32_t previous_value;

void *loop_input_collection(InputArgs *args)
{
    unsigned char *mem_base = (unsigned char *)init_memory();

    previous_value = get_knobs_input(mem_base);

    while (!*(args->stop))
    {
        uint32_t knobs_raw_value = get_knobs_input(mem_base);

        wait_for_queue_lock(args->input_queue);

        if (detect_button_raising_edge(previous_value, knobs_raw_value, BLUE_BUTTON_BITWISE_OFFSET))
        {
            enqueue_input_event(args->input_queue, BLUE_KEY);
        }

        if (detect_button_raising_edge(previous_value, knobs_raw_value, GREEN_BUTTON_BITWISE_OFFSET))
        {
            enqueue_input_event(args->input_queue, GREEN_KEY);
        }

        if (detect_button_raising_edge(previous_value, knobs_raw_value, RED_BUTTON_BITWISE_OFFSET))
        {
            enqueue_input_event(args->input_queue, RED_KEY);
        }

        int blue_knob_diff = get_knob_values_difference(previous_value, knobs_raw_value, BLUE_KNOB_BITWISE_OFFSET);
        int green_knob_diff = get_knob_values_difference(previous_value, knobs_raw_value, GREEN_KNOB_BITWISE_OFFSET);

        if (blue_knob_diff > 0)
        {
            // printf("Enq rotate right blue\n");
            enqueue_input_event(args->input_queue, ROTATE_RIGHT_BLUE);
        }

        if (blue_knob_diff < 0)
        {
            // printf("Enq rotate left blue\n");
            enqueue_input_event(args->input_queue, ROTATE_LEFT_BLUE);
        }

        if (green_knob_diff > 0)
        {
            // printf("Enq rotate right green\n");
            enqueue_input_event(args->input_queue, ROTATE_RIGHT_GREEN);
        }

        if (green_knob_diff < 0)
        {
            // printf("Enq rotate left green\n");
            enqueue_input_event(args->input_queue, ROTATE_LEFT_GREEN);
        }

        previous_value = knobs_raw_value;

        release_queue_lock(args->input_queue);

        usleep(INPUT_DELAY_MS * 1000);
    }
}