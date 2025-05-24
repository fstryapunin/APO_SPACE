typedef struct 
{
    double x;
    double y;
} Vector;

double get_distance(Vector a, Vector b);
Vector rotate(Vector vector, double theta_rad);