#ifndef COMON
#define COMON
#include <stdbool.h>
#include "vector.h"
#include "math.h"
typedef struct
{
    Vector position;
    int radius;
    bool is_visited;
    bool is_home;
} Planet;

#endif
