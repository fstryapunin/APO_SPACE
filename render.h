#include "common.h"
#define LED_COUNT 32
#define PIXELS_PER_METER_IN_GAME 10
#define PIXELS_PER_METER_IN_MAP 0.1f
#define RENDER_DELAY_MS 20

enum MenuState 
{
    START,
    PAUSE
};

typedef struct
{
    int *fuel;
    double *rotation;
    Vector *position;
    int *player_state; 
    Planet *planets;
    unsigned short *planet_count;
    unsigned char *memory_base;
    bool stop;
} RenderArgs;

void *loop_render(RenderArgs *args);