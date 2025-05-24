#include "common.h"

#define MAX_PLANETS 100
#define ACCELERATION_CONSTANT_M_S 20
#define FUEL_AMOUNT 500
#define FUEL_BURN_L_S 1

typedef struct 
{
    int rotation;
    int speed_x;
    int speed_y;
    int position_x;
    int position_y;
    int is_stopped;
    int is_crashed;
    int fuel_state;
    int score;
    Planet *planets;
} GameState;

void init_gamestate(GameState *state);
void update_gamestate(GameState *state, int steering_input, int acceleration_input);
