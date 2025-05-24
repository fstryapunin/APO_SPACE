#include "vector.h"
#include "math.h"

double get_distance(Vector *a, Vector *b)
{
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
};

void rotate(Vector *vector, Vector *rotated_vector, int theta_rad)
{
    double theta_cos = cos(theta_rad);
    double theta_sin = sin(theta_rad);

    rotated_vector->x = vector->x * theta_cos - vector->y * theta_sin;
    rotated_vector->y = vector->x * theta_sin + vector->y * theta_cos;
};