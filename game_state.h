#include "common.h"

#define MAX_PLANETS 100
#define ACCELERATION_CONSTANT_M_UPDATE 0.01d
#define ANGULAR_ACCELLRATION_CONSTANT_RAD (2 * M_PI) / 10
#define FUEL_AMOUNT 500
#define FUEL_BURN_L_S 1
#define SCORE_REWARD 100
#define UPDATE_DELAY_MS 100

typedef enum PlayerState 
{
    CRASHED = 0,
    LANDED = 1,
    FLYING = 2
} PlayerState;

// speed is defined in meters per second
typedef struct 
{
    double rotation_radians;
    Vector speed;
    Vector position;
    PlayerState player_state;
    int current_planet_index;
    int remaining_fuel;
    int score;
    Planet planets[MAX_PLANETS];
    unsigned short planet_count;
} GameState;

typedef struct {
    GameState *state;
    double *acceration;
    double *rotation;
    bool stop;
} GameStateArgs;

GameState init_gamestate();
void *loop_game_state(GameStateArgs *args);
