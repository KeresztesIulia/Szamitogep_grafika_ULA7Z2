#ifndef VECTOR.H
#define VECTOR.H

typedef struct Vector
{
    float x;
    float y;
    float hom;
} Vector;

void inicialize_vector(Vector* vector, float x, float y, float z);

void transform_point(Vector* vector, float** matrix);

#endif