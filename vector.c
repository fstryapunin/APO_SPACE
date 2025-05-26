#include "vector.h"

double get_magnitude_from_vector(Vector vector){
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

double get_magnitude_from_coordinates(double x, double y){
    return sqrt(pow(x, 2) + pow(y, 2));
}

double get_distance(Vector a, Vector b){
    return get_magnitude_from_coordinates(a.x - b.x, a.y - b.y);
};

double get_angle(Vector vector){
    return atan2(vector.y, vector.x);
}

Vector rotate_vector(Vector vector, double theta_rad){
    double theta_cos = cos(theta_rad);
    double theta_sin = sin(theta_rad);

    return (Vector){ 
        vector.x * theta_cos - vector.y * theta_sin, 
        vector.x * theta_sin + vector.y * theta_cos
    };
};

Vector add_vectors(Vector a, Vector b){
    return (Vector){ a.x + b.x, a.y + b.y };
}

Vector subtract_vectors(Vector a, Vector b){
    return (Vector){ a.x - b.x, a.y - b.y };
}

Vector get_vector_from_angle(double angle){
    return (Vector){ cos(angle), sin(angle) };
}

Vector multiply_vector_by_scalar(Vector vector, double scalar){
    return (Vector){ scalar * vector.x, scalar * vector.y };
}

Vector divide_vector_by_scalar(Vector vector, double scalar){
    return (Vector){ vector.x / scalar, vector.y / scalar };
}

Vector get_vector_from_angle_and_magnitude(double angle, double magnitude){
    return multiply_vector_by_scalar(get_vector_from_angle(angle), magnitude);
}