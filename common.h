#ifndef COMON
#define COMON
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "vector.h"

#define PLAYER_WIDTH 2
#define PLAYER_HEIGHT 8
#define FUEL_AMOUNT 3200
#define FUEL_PER_LED (FUEL_AMOUNT / 32)

typedef enum PlayerState 
{
    CRASHED = 0,
    LANDED = 1,
    FLYING = 2
} PlayerState;

typedef struct
{
    Vector position;
    double radius;
    bool is_visited;
    bool is_home;
} Planet;

#endif
