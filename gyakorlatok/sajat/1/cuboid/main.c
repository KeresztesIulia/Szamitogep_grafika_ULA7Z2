#include "brick.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
    Brick brick;
    double volume, surface;
    int n_square_faces;

    set_brick_data(&brick, 10, 10, 10);
    volume = calc_brick_volume(&brick);  
    surface = calc_brick_surface(&brick);
    n_square_faces = count_square_faces(&brick);

    printf("Brick volume: %lf\nBrick surface area: %lf\nThe brick has %d square faces.\n", volume, surface, n_square_faces);

    return 0;
}