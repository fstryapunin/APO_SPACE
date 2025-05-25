#ifndef COMON
#define COMON
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "vector.h"

typedef struct
{
    Vector position;
    double radius;
    bool is_visited;
    bool is_home;
} Planet;

#endif
