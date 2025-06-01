#include "common.h"
#define LED_COUNT 32


#define PIXELS_PER_METER_IN_GAME 3
#define PIXELS_PER_METER_IN_MAP 0.1f

#define PLAYER_WIDTH_PX (PLAYER_WIDTH * PIXELS_PER_METER_IN_GAME)
#define PLAYER_HEIGHT_PX (PLAYER_HEIGHT * PIXELS_PER_METER_IN_GAME)

#define INDICATOR_TRESHOLD 30

#define RENDER_DELAY_MS 20

#define LCD_CENTER_X 240
#define LCD_CENTER_Y 160
#define LCD_WIDTH 480
#define LCD_HEIGHT 320


#define DARK_BLUE_COLOR 0x000F
#define WHITE_COLOR 0xFFFF
#define PLAYER_COLOR 0x07f2

enum MenuState 
{
    START,
    PAUSE
};

typedef struct
{
    double *rotation;
    Vector *position;
    Planet *planets;
    unsigned short *planet_count;
    int *nearest_planet_index;
    double *remaining_fuel;
    PlayerState *player_state;
    bool *stop;
} RenderArgs;

void *loop_render(RenderArgs *args);