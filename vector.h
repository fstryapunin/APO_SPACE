#include <math.h>

#ifndef VECTORS
#define VECTORS
typedef struct 
{
    double x;
    double y;
} Vector;


double get_magnitude_from_vector(Vector vector);
double get_magnitude_from_coordinates(double x, double y);
double get_distance(Vector a, Vector b);
double get_angle(Vector vector);

Vector rotate_vector(Vector vector, double theta_rad);
Vector add_vectors(Vector a, Vector b);
Vector subtract_vectors(Vector a, Vector b);

Vector multiply_vector_by_scalar(Vector vector, double scalar);
Vector divide_vector_by_scalar(Vector vector, double scalar);

Vector get_vector_from_angle(double angle);
Vector get_vector_from_angle_and_magnitude(double angle, double magnitude);

#endif