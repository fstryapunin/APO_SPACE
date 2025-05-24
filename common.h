#ifndef COMON
#define COMON
#include <stdbool.h>
#include "vector.h"
#include "math.h"
typedef struct
{
    Vector position;
    double radius;
    bool is_visited;
    bool is_home;
} Planet;

#endif
