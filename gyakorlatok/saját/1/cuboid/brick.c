#include "brick.h"

#include <math.h>

void set_brick_data(Brick* brick, double x, double y, double z)
{
    if (x > 0.0)
    {
        brick->x = x;
    }
    else
    {
        brick->x = NAN;
    }
    if (y > 0.0)
    {
       brick->y=y;
    }
    else
    {
        brick->y = NAN;
    }
    if (z > 0.0)
    {
        brick->z=z;
    }
    else
    {
        brick->z = NAN;
    }
}

double calc_brick_volume(const Brick* brick)
{
    double volume = brick->x * brick->y * brick->z;
    return volume;
}

double calc_brick_surface(const Brick* brick)
{
    double surface = 2 * (brick->x * brick->y) + 2 * (brick->x * brick->z) + 2 * (brick->y * brick->z);
    return surface;
}

int count_square_faces(const Brick* brick)
{
    int n_square_faces = 0;
    if(brick->x == brick->y)
    {
        n_square_faces += 2;
    }
    if(brick->x == brick->z)
    {
        n_square_faces += 2;
    }
    if(brick->y == brick->z)
    {
        n_square_faces += 2;
    }
    return n_square_faces;
}