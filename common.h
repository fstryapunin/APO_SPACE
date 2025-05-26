#ifndef COMON
#define COMON
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "vector.h"

#define PLAYER_WIDTH 2
#define PLAYER_HEIGHT 4

typedef struct
{
    Vector position;
    double radius;
    bool is_visited;
    bool is_home;
} Planet;

#endif
