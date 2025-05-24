#include "common.h"
#define LED_COUNT 32
#define PIXELS_PER_METER_IN_GAME 10
#define PIXELS_PER_METER_IN_MAP 0.1f

enum MenuState 
{
    START,
    PAUSE
};

void render_state(int fuel, int rotation, int player_x, int player_y, bool is_stopped, bool is_crashed, Planet *planets);
void render_map(int player_x, int player_y, Planet *planets);
void render_menu(MenuState state);