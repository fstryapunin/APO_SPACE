#include "common.h"
#define LED_COUNT 32
#define PIXELS_PER_METER_IN_GAME 3
#define PIXELS_PER_METER_IN_MAP 0.1f
#define RENDER_DELAY_MS 20
#define LCD_WIDTH 480
#define LCD_HEIGHT 320
#define DARK_BLUE_COLOR 0x000F
#define WHITE_COLOR 0xFFFF

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
    bool stop;
} RenderArgs;

void *loop_render(RenderArgs *args);