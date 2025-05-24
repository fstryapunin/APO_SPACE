#include "game_state.h"

#define NUMBER_OF_COLLISION_POINTS 10
#define LANDING_COLLISION_POINT_START_INDEX 9
#define DISTANCE_TRESHOLD_FOR_LANDING 1
#define SPEED_TRESHOLD_FOR_LANDING 5

static const Vector collision_points[NUMBER_OF_COLLISION_POINTS] = 
    {{0, 2}, {1, 1}, {1, 0}, {1, -1}, {-1, 1}, {1, 0}, {-1, -1}, {1, -2}, {-1, -2}, {0, -2}};

PlayerState get_player_state(GameState *state, int *collision_planet_index){
    for(int planet_index = 0; planet_index < state->planet_count; planet_index++){
        for(int colision_point_index = 0; colision_point_index  <  NUMBER_OF_COLLISION_POINTS; colision_point_index++)
        {
            Vector collision_point_relative_vector = rotate_vector(collision_points[colision_point_index], state->rotation_radians);
            Vector collision_point_coordinates = add_vectors(state->position, collision_point_relative_vector);
            double distance = get_distance(state->planets[planet_index].position, collision_point_coordinates);
            double speed_magnitude = get_magnitude_from_vector(state->speed);

            if(colision_point_index == LANDING_COLLISION_POINT_START_INDEX && distance < DISTANCE_TRESHOLD_FOR_LANDING && speed_magnitude < SPEED_TRESHOLD_FOR_LANDING)
            {
                *collision_planet_index = planet_index;
                return LANDED;
            }
            if(distance < state->planets[planet_index].radius){
                *collision_planet_index = planet_index;
                return CRASHED;
            }
        }
    }
    
    return FLYING;
};

double get_rotation(double rotation, double set_point){
    if(abs(set_point - rotation) < ANGULAR_ACCELLRATION_CONSTANT_RAD) return set_point;
    
    return set_point > rotation ? rotation + ANGULAR_ACCELLRATION_CONSTANT_RAD : rotation - ANGULAR_ACCELLRATION_CONSTANT_RAD;
};

Vector get_acceleration_vector(double rotation, int acceleration_input){
    return rotate_vector((Vector){acceleration_input * acceleration_input, 0}, rotation);
};

void init_gamestate(GameState *state);

void update_gamestate(GameState *state, double steering_set_point_radians, int acceleration_input){
    if(state->player_state == LANDED && acceleration_input == 0){
        return;
    }

    state->rotation_radians = get_rotation(state->rotation_radians, steering_set_point_radians);
    state->speed = add_vectors(state->speed, get_acceleration_vector(state->rotation_radians, acceleration_input));
    state->position = add_vectors(state->position, state->speed);

    
    int collision_planet_index = -1;
    PlayerState nextPlayerState = get_player_state(state, &collision_planet_index);


    if(nextPlayerState == CRASHED){
        state->speed = (Vector){0, 0};
        state->player_state = CRASHED;
        state->current_planet_index = collision_planet_index;
        return;
    }

    if(nextPlayerState == LANDED && state->player_state != LANDED){
        Planet collision_planet = state->planets[collision_planet_index];
        Vector planet_center = subtract_vectors(state->position, collision_planet.position);
        double angle_to_planet = get_angle(planet_center);
        double distance = get_magnitude_from_vector(planet_center);

        Vector position_relative_to_planet = add_vectors(
            rotate_vector((Vector){ 0, 2 }, angle_to_planet), 
            get_vector_from_angle_and_magnitude(angle_to_planet, collision_planet.radius));

        state->position = add_vectors(position_relative_to_planet, collision_planet.position);
        state->speed = (Vector){ 0, 0 };
        state->rotation_radians = angle_to_planet;
        state->current_planet_index = collision_planet_index;

        if(state->planets[collision_planet_index].is_visited == false){
            state->score = state->score + SCORE_REWARD;
            state->planets[collision_planet_index].is_visited = true;
        }

        return;
    }

    state->player_state = nextPlayerState;
};