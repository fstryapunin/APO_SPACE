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
    state.position = (Vector) {0, 102};
    state.player_state = LANDED;
    state.planet_count = 1;

    state.remaining_fuel = FUEL_AMOUNT;
    state.score = 0;
    state.current_planet = -1;

    return state;
};

int get_nearest_planet_index(GameState *state){
    int index = -1;
    double min_distance = DBL_MAX;

    for(int planet_index = 0; planet_index < state->planet_count; planet_index++){
        double distance = get_distance(state->position, state->planets[planet_index].position);
        if(distance < min_distance){
            min_distance = distance;
            index = planet_index;
        }
    }
    
    return index;
}

Vector get_gravity_acceleration(Vector player, Planet planet) {
    double mass = pow(planet.radius, 3) * 100000;
    double distance = get_distance(player, planet.position);

    if(distance - planet.radius <= 0.001) return (Vector) {0, 0};

    Vector direction = subtract_vectors(planet.position, player);
    return multiply_vector_by_scalar(direction, (GRAVITY_CONSTANT * mass) / (distance * distance));
}

Vector get_speed(GameState *state, Vector acceleration){
    Planet nearest = state->planets[state->nearest_planet];

    Vector current = add_vectors(state->speed, acceleration);

    double distance = get_distance(add_vectors(state->position, current), nearest.position) - nearest.radius;

    if(distance <= 0) return (Vector){ 0, 0};

    return current;
};

Vector get_acceleration_vector(GameState *state){
    Vector player_acceleration = state->motor_power > 0 ? rotate_vector((Vector){ACCELERATION_CONSTANT * state->motor_power, 0}, state->rotation_set_point) : (Vector) {0, 0};
    Vector gravity_acceleration_cumulative = (Vector){0, 0};
    // printf("P direction %lf %lf,\n", player_acceleration.x, player_acceleration.y);

    for(int i = 0; i < state->planet_count; i++){
        Vector acceleration = get_gravity_acceleration(state->position, state->planets[i]);
        Vector normalized = divide_vector_by_scalar(acceleration, UPDATE_FREQUENCY);
        // printf("G direction %lf %lf,\n", normalized.x, normalized.y);

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

    // state->rotation_radians = fmod((M_PI / 2 + steering_set_point_radians), (2 * M_PI));
    // state->nearest_planet = get_nearest_planet_index(state);
    // Vector acceleration = get_acceleration_vector(state, acceleration_input);
    // state->speed = get_speed(state, acceleration);
    // state->position = add_vectors(state->position, state->speed);
    
    // printf("acceleration: %lf %lf %lf speed : %lf %lf position: %lf %lf\n",
    //     acceleration_input,
    //     acceleration.x, 
    //     acceleration.y,
    //     state->speed.x,
    //     state->speed.y,
    //     state->position.x,
    //     state->position.y
    // );


    int collision_planet_index = -1;
    PlayerState nextPlayerState = get_player_state(state, &collision_planet_index);

    // if(nextPlayerState == CRASHED){
    //     state->speed = (Vector){0, 0};
    //     state->player_state = CRASHED;
    //     state->current_planet_index = collision_planet_index;
    //     return;
    // }

    // if(nextPlayerState == LANDED && state->player_state != LANDED){
    //     Planet collision_planet = state->planets[collision_planet_index];
    //     Vector planet_center = subtract_vectors(state->position, collision_planet.position);
    //     double angle_to_planet = get_angle(planet_center);
    //     double distance = get_magnitude_from_vector(planet_center);

    //     Vector position_relative_to_planet = add_vectors(
    //         rotate_vector((Vector){ 0, 2 }, angle_to_planet), 
    //         get_vector_from_angle_and_magnitude(angle_to_planet, collision_planet.radius));

    //     state->position = add_vectors(position_relative_to_planet, collision_planet.position);
    //     state->speed = (Vector){ 0, 0 };
    //     state->rotation_radians = angle_to_planet;
    //     state->current_planet_index = collision_planet_index;

    //     if(state->planets[collision_planet_index].is_visited == false){
    //         state->score = state->score + SCORE_REWARD;
    //         state->planets[collision_planet_index].is_visited = true;
    //     }

    //     return;
    // }

    state->player_state = nextPlayerState;
};

void *loop_game_state(GameStateArgs *args){
    // printf("Stared \n");
    printf("Udpating at %lf, acc per update : %lf\n", UPDATE_DELAY_MS * 1000, ACCELERATION_CONSTANT);

    while (!args->stop)
    {
        update_gamestate(args->state, *(args->rotation), *(args->acceration));

        printf("\rState: %lf %lf %lf %lf\n", 
            args->state->position.x, 
            args->state->position.y, 
            args->state->speed.x, 
            args->state->speed.y
        );

        usleep((int)(UPDATE_DELAY_MS * 1000));
    }

    return NULL;
}