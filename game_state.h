#include "common.h"
#include "queue.h"

#define PLANET_COUNT 3
#define ACCELERATION_CONSTANT_M_S (double)0.01
#define ANGULAR_ACCELLRATION_CONSTANT_RAD (2 * M_PI) / 10
#define GRAVITY_CONSTANT 6.67430e-11

#define SCORE_REWARD 100

#define ACCELERATION_CONSTANT (ACCELERATION_CONSTANT_M_S / UPDATE_FREQUENCY)
#define LANDING_SPEED_TRESHOLD ((double)5 / UPDATE_FREQUENCY)
#define FUEL_BURN_PER_SECOND 10

#define MAX_ACCELERATION_INPUT 10
#define MIN_ACCELERATION_INPUT 0
#define RADIANS_PER_KNOB_INPUT (2 * M_PI) / (double)20

#define UPDATE_FREQUENCY (double)100
#define UPDATE_DELAY_MS ((double)1 / UPDATE_FREQUENCY) * 1000


typedef enum PlayerState 
{
    CRASHED = 0,
    LANDED = 1,
    FLYING = 2
} PlayerState;

// speed is defined in meters per second
typedef struct 
{
    int motor_power;
    double rotation_set_point;
    Vector speed;
    Vector position;
    PlayerState player_state;
    double remaining_fuel;
    int current_planet;
    Planet planets[PLANET_COUNT];
    unsigned short planet_count;
} GameState;

typedef struct {
    GameState *state;
    Queue *input_queue;
    bool *stop;
} GameStateArgs;

GameState init_gamestate();
void *loop_game_state(GameStateArgs *args);
