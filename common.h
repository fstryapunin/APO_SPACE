#include <stdbool.h>
typedef struct
{
    int x;
    int y;
    int radius;
    bool is_visited;
    bool is_home;
} Planet;

typedef struct 
{
    int x;
    int y;
} Vector;