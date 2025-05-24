#include "common.h"

#define MAX_PLANETS 100
#define ACCELERATION_CONSTANT_M_S 10
#define FUEL_AMOUNT 500
#define FUEL_BURN_L_S 1

// speed is defined in meters per second
typedef struct 
{
    int rotation_radians;
    int speed_x;
    int speed_y;
    int position_x;
    int position_y;
    bool is_stopped;
    bool is_crashed;
    int remaining_fuel;
    int score;
    Planet *planets;
} GameState;

void init_gamestate(GameState *state);
void update_gamestate(GameState *state, int steering_input, int acceleration_input);
