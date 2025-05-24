#include "game_state.h"
#include "math.h"

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
            double distance = get_distance(state->planets[planet_index].position, collision_point_coordinates) - state->planets[planet_index].radius;
            double speed_magnitude = get_magnitude_from_vector(state->speed);

            if(colision_point_index == LANDING_COLLISION_POINT_START_INDEX && distance < (DISTANCE_TRESHOLD_FOR_LANDING) && speed_magnitude < SPEED_TRESHOLD_FOR_LANDING)
            {
                *collision_planet_index = planet_index;
                return LANDED;
            }
            if(distance < 0){
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

void init_gamestate(GameState *state){
    state->speed = (Vector){ 0, 0};
    state->position = (Vector) {40000, -10000 + 2};
    state->rotation_radians = M_PI;
    state->player_state = LANDED;
    state->planet_count = MAX_PLANETS;
    state->remaining_fuel = FUEL_AMOUNT;
    state->score = 0;
    state->current_planet_index = -1;

    state->planets[0] = (Planet){{-16719, -63097}, 7648, false, false};
    state->planets[1] = (Planet){{40000, -10000}, 7976, true, true};
    state->planets[2] = (Planet){{88100, -10353}, 4345, false, false};
    state->planets[3] = (Planet){{53529, -8223}, 6785, false, false};
    state->planets[4] = (Planet){{23202, 4065}, 4553, false, false};
    state->planets[5] = (Planet){{95178, 83109}, 9589, false, false};
    state->planets[6] = (Planet){{42626, -10783}, 5251, false, false};
    state->planets[7] = (Planet){{20515, -4892}, 5911, false, false};
    state->planets[8] = (Planet){{-34721, -15668}, 1805, false, false};
    state->planets[9] = (Planet){{-14723, -63413}, 6098, false, false};
    state->planets[10] = (Planet){{93361, 5058}, 8351, false, false};
    state->planets[11] = (Planet){{-32615, 51893}, 3335, false, false};
    state->planets[12] = (Planet){{96391, -33691}, 4962, false, false};
    state->planets[13] = (Planet){{11005, -34367}, 9597, false, false};
    state->planets[14] = (Planet){{-25403, 28686}, 7055, false, false};
    state->planets[15] = (Planet){{-27209, -98071}, 6656, false, false};
    state->planets[16] = (Planet){{72670, 28797}, 9930, false, false};
    state->planets[17] = (Planet){{-69866, -72826}, 5658, false, false};
    state->planets[18] = (Planet){{-48330, 99605}, 9090, false, false};
    state->planets[19] = (Planet){{-61721, 96213}, 9231, false, false};
    state->planets[20] = (Planet){{76783, -22171}, 8431, false, false};
    state->planets[21] = (Planet){{-1302, 65227}, 4118, false, false};
    state->planets[22] = (Planet){{12395, 40672}, 2137, false, false};
    state->planets[23] = (Planet){{3931, 14606}, 3628, false, false};
    state->planets[24] = (Planet){{61391, -34138}, 9184, false, false};
    state->planets[25] = (Planet){{16672, -45031}, 2823, false, false};
    state->planets[26] = (Planet){{58144, 18593}, 1013, false, false};
    state->planets[27] = (Planet){{28996, -95074}, 1513, false, false};
    state->planets[28] = (Planet){{82487, -68183}, 4630, false, false};
    state->planets[29] = (Planet){{-60705, 37941}, 2072, false, false};
    state->planets[30] = (Planet){{-75635, 40057}, 5760, false, false};
    state->planets[31] = (Planet){{31369, 92442}, 8585, false, false};
    state->planets[32] = (Planet){{-81366, -16715}, 3280, false, false};
    state->planets[33] = (Planet){{27422, 23328}, 3090, false, false};
    state->planets[34] = (Planet){{45319, 58852}, 4527, false, false};
    state->planets[35] = (Planet){{90555, 47501}, 5558, false, false};
    state->planets[36] = (Planet){{23672, 5919}, 2408, false, false};
    state->planets[37] = (Planet){{-64426, -10668}, 3548, false, false};
    state->planets[38] = (Planet){{-39765, -97769}, 2132, false, false};
    state->planets[39] = (Planet){{98726, -76847}, 5706, false, false};
    state->planets[40] = (Planet){{50744, 382}, 8289, false, false};
    state->planets[41] = (Planet){{-93039, 39927}, 4683, false, false};
    state->planets[42] = (Planet){{92238, -59618}, 1414, false, false};
    state->planets[43] = (Planet){{97402, 78159}, 2466, false, false};
    state->planets[44] = (Planet){{67135, -64044}, 4453, false, false};
    state->planets[45] = (Planet){{-22690, -41275}, 6578, false, false};
    state->planets[46] = (Planet){{72096, -74853}, 1574, false, false};
    state->planets[47] = (Planet){{-2918, -50405}, 3352, false, false};
    state->planets[48] = (Planet){{-9751, -68513}, 8370, false, false};
    state->planets[49] = (Planet){{-56942, -71194}, 5887, false, false};
    state->planets[50] = (Planet){{-4680, -10528}, 6985, false, false};
    state->planets[51] = (Planet){{-11011, 26575}, 3917, false, false};
    state->planets[52] = (Planet){{-18631, 1193}, 9211, false, false};
    state->planets[53] = (Planet){{28662, 19629}, 2579, false, false};
    state->planets[54] = (Planet){{50431, 18091}, 6792, false, false};
    state->planets[55] = (Planet){{69961, 70087}, 1687, false, false};
    state->planets[56] = (Planet){{39962, 64257}, 8186, false, false};
    state->planets[57] = (Planet){{-11603, 12494}, 5500, false, false};
    state->planets[58] = (Planet){{-67980, -52884}, 6194, false, false};
    state->planets[59] = (Planet){{-27389, 50450}, 6306, false, false};
    state->planets[60] = (Planet){{92329, 82276}, 1338, false, false};
    state->planets[61] = (Planet){{61177, -93245}, 3138, false, false};
    state->planets[62] = (Planet){{-91226, -22761}, 8632, false, false};
    state->planets[63] = (Planet){{-91219, 80777}, 1115, false, false};
    state->planets[64] = (Planet){{95603, 28697}, 5570, false, false};
    state->planets[65] = (Planet){{73768, 81833}, 6379, false, false};
    state->planets[66] = (Planet){{23616, 57154}, 4407, false, false};
    state->planets[67] = (Planet){{86546, -52532}, 8210, false, false};
    state->planets[68] = (Planet){{-16461, -79102}, 9985, false, false};
    state->planets[69] = (Planet){{-5902, -55892}, 3354, false, false};
    state->planets[70] = (Planet){{33700, 59646}, 1716, false, false};
    state->planets[71] = (Planet){{7658, -37884}, 2948, false, false};
    state->planets[72] = (Planet){{-74303, -25590}, 7342, false, false};
    state->planets[73] = (Planet){{59453, -72054}, 4349, false, false};
    state->planets[74] = (Planet){{70907, -68385}, 8208, false, false};
    state->planets[75] = (Planet){{90519, 1984}, 4982, false, false};
    state->planets[76] = (Planet){{82632, -76465}, 6850, false, false};
    state->planets[77] = (Planet){{-8877, 67179}, 9069, false, false};
    state->planets[78] = (Planet){{29973, -52778}, 8710, false, false};
    state->planets[79] = (Planet){{54211, -16503}, 9544, false, false};
    state->planets[80] = (Planet){{-59050, 52496}, 6340, false, false};
    state->planets[81] = (Planet){{59660, -23308}, 7051, false, false};
    state->planets[82] = (Planet){{58281, 59759}, 6103, false, false};
    state->planets[83] = (Planet){{-71103, -83534}, 6764, false, false};
    state->planets[84] = (Planet){{-68179, 50488}, 7603, false, false};
    state->planets[85] = (Planet){{-19666, 98144}, 6718, false, false};
    state->planets[86] = (Planet){{74808, 20978}, 2664, false, false};
    state->planets[87] = (Planet){{42633, 80652}, 1866, false, false};
    state->planets[88] = (Planet){{-33581, 3638}, 6849, false, false};
    state->planets[89] = (Planet){{-29713, 68452}, 3940, false, false};
    state->planets[90] = (Planet){{94369, -44275}, 8379, false, false};
    state->planets[91] = (Planet){{82785, -68067}, 2409, false, false};
    state->planets[92] = (Planet){{-42945, -88676}, 5576, false, false};
    state->planets[93] = (Planet){{78195, -54203}, 8624, false, false};
    state->planets[94] = (Planet){{-94616, 79868}, 1530, false, false};
    state->planets[95] = (Planet){{-95352, -22588}, 6339, false, false};
    state->planets[96] = (Planet){{28443, -16788}, 5806, false, false};
    state->planets[97] = (Planet){{94169, 46692}, 6225, false, false};
    state->planets[98] = (Planet){{-51030, 65863}, 4176, false, false};
    state->planets[99] = (Planet){{33766, 94380}, 4533, false, false};
};

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