#include "vector.h"

void inicialize_vector(Vector* vector, float x, float y, float hom)
{
    vector->x = x;
    vector->y = y;
    vector->hom = hom;
}

Vector transform_point(Vector* vector, float* matrix)
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            
        }
    }
}