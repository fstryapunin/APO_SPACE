#include "common.h"
#define PIXELS_PER_METER_IN_GAME 10
#define PIXELS_PER_METER_IN_MAP 0.1f

void render_state(int fuel, int rotation, int player_x, int player_y, Planet *planets);
void render_map(int player_x, int player_y, Planet *planets);