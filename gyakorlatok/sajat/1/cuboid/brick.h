#ifndef BRICK_H
#define BRICK_H

typedef struct Brick
{
    double x;
    double y;
    double z;
} Brick;

void set_brick_data(Brick* brick, double x, double y, double z);

double calc_brick_volume(const Brick* brick);

double calc_brick_surface(const Brick* brick);

//does the cuboid have any square faces? How many?
int count_square_faces(const Brick* brick);

#endif