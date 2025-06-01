#include <stdio.h>
#include "game_state.h"
#include "math.h"
#include "float.h"

#define NUMBER_OF_COLLISION_POINTS 10
#define LANDING_COLLISION_POINT_START_INDEX 9
#define DISTANCE_TRESHOLD_FOR_LANDING 1
#define SPEED_TRESHOLD_FOR_LANDING 5

GameState init_gamestate(){
    GameState state;

    state.planets[0] = (Planet){{0, 0}, 100, true, true};



    state.rotation_set_point = M_PI / 2;
    state.motor_power = 0;
    state.speed = (Vector){ 0, 0};
    state.position = (Vector) {0, 104};
    state.player_state = LANDED;
    state.planet_count = 1;
    state.remaining_fuel = (double)FUEL_AMOUNT;
    state.current_planet = -1;

    return state;
};

Vector get_gravity_acceleration(Vector player, Planet planet) {
    double mass = pow(planet.radius, 3) * 100000;
    double distance = get_distance(player, planet.position);

    if(distance - planet.radius <= 0.001) return (Vector) {0, 0};

    Vector direction = subtract_vectors(planet.position, player);
    return multiply_vector_by_scalar(direction, (GRAVITY_CONSTANT * mass) / (distance * distance));
}

PlayerState detect_colision(GameState *state, int *collision_planet_index){
    Vector next_position = add_vectors(state->position, state->speed);
    bool collision = false;
    *collision_planet_index = -1;

    for(int planet_index = 0; planet_index < state->planet_count; planet_index++){
        double distance = 
            get_distance(state->planets[planet_index].position, next_position) 
            - state->planets[planet_index].radius;

        if(distance <= 0.001){
            collision = true;
            *collision_planet_index = planet_index;
            break;
        }
    }

    if(collision){
        return LANDING_SPEED_TRESHOLD >= get_magnitude_from_vector(state->speed) ? LANDED : CRASHED;
    }

    return FLYING;
}

Vector get_acceleration_vector(GameState *state){
    Vector player_acceleration = state->motor_power > 0 ? rotate_vector((Vector){ACCELERATION_CONSTANT * state->motor_power, 0}, state->rotation_set_point) : (Vector) {0, 0};
    Vector gravity_acceleration_cumulative = (Vector){0, 0};
    // printf("P direction %lf %lf,\n", player_acceleration.x, player_acceleration.y);

    for(int i = 0; i < state->planet_count; i++){
        Vector acceleration = get_gravity_acceleration(state->position, state->planets[i]);
        Vector normalized = divide_vector_by_scalar(acceleration, UPDATE_FREQUENCY);
        gravity_acceleration_cumulative = add_vectors(gravity_acceleration_cumulative, normalized);
    }

    return add_vectors(player_acceleration, gravity_acceleration_cumulative);
};

int get_motor_power(int current, InputEvent event){
    if(event == ROTATE_RIGHT_BLUE && current < MAX_ACCELERATION_INPUT){
        return current + 1;
    }

    if(event == ROTATE_LEFT_BLUE && current > MIN_ACCELERATION_INPUT){
        return current - 1;
    }

    return current;
}

double get_rotation_radians(double current, InputEvent event){
    double next = current;
    if(event == ROTATE_LEFT_GREEN){
        next = next - RADIANS_PER_KNOB_INPUT;
    }

    if(event == ROTATE_RIGHT_GREEN){
        next = next + RADIANS_PER_KNOB_INPUT;
    }

    return fmod(next, 2 * M_PI);
}

void update_gamestate(GameState *state, Queue *queue){
    wait_for_queue_lock(queue);

    if(state->player_state == LANDED && queue->count == 0){
        release_queue_lock(queue);
        return;
    }

    InputEvent input = queue->count > 0 ? dequeu_input_event(queue) : NONE;
    release_queue_lock(queue);

    if(state == LANDED && (input != ROTATE_LEFT_BLUE && input != ROTATE_RIGHT_BLUE)){
        return;
    }
 
    state->motor_power = get_motor_power(state->motor_power, input);
    state->rotation_set_point = get_rotation_radians(state->rotation_set_point, input);
    state->speed = add_vectors(state->speed, get_acceleration_vector(state));

    state->player_state = detect_colision(state, &state->current_planet);

    if(state->player_state == FLYING){
        state->position = add_vectors(state->position, state->speed);
        return;
    }

    if(state->player_state == LANDED){
        state->speed = (Vector){0, 0};
        state->motor_power = 0;
        state->rotation_set_point = 
            get_angle(subtract_vectors(state->position, state->planets[state->current_planet].position));
        return;
    }

    if(state->player_state == CRASHED){
        state->speed = (Vector){0, 0};
        state->motor_power = 0;
        return;
    }
};

void *loop_game_state(GameStateArgs *args){
    printf("Udpating at %lf, acc per update : %lf\n", UPDATE_DELAY_MS * 1000, ACCELERATION_CONSTANT);

    while (!*(args->stop))
    {
        update_gamestate(args->state, args->input_queue);
        usleep((int)(UPDATE_DELAY_MS * 1000));
    }

    return NULL;
}