#include "directional_chances.h"

#include <stdlib.h>

void init_chances(Directional_chances* chances)
{
    chances->N = 0.125;
    chances->NW = 0.125;
    chances->W = 0.125;
    chances->SW = 0.125;
    chances->S = 0.125;
    chances->SE = 0.125;
    chances->E = 0.125;
    chances->NE = 0.125;
}

void calc_base_chances(Directional_chances* chances, float dist_min_x, float dist_max_x, float dist_min_y, float dist_max_y)
{
    float chance_change = 0.0125;

    if (dist_min_x < dist_max_x)
        {
            chances->NE += chance_change;
            chances->E += chance_change;
            chances->SE += chance_change;

            chances->NW -= chance_change;
            chances->W -= chance_change;
            chances->SW -= chance_change;
            // + NE, E, SE
            // - NW, W, SW
        }
        else if (dist_min_x > dist_max_x)
        {
            chances->NW += chance_change;
            chances->W += chance_change;
            chances->SW += chance_change;

            chances->NE -= chance_change;
            chances->E -= chance_change;
            chances->SE -= chance_change;
            // + NW, W, SW
            // - NE, E, SE
        }

        if (dist_min_y < dist_max_y)
        {
            chances->NW += chance_change;
            chances->N += chance_change;
            chances->NE += chance_change;

            chances->SW -= chance_change;
            chances->S -= chance_change;
            chances->SE -= chance_change;
            // + NW, N, NE
            // - SW, S, SE
        }
        else if (dist_min_y > dist_max_y)
        {
            chances->SW += chance_change;
            chances->S += chance_change;
            chances->SE += chance_change;

            chances->NW -= chance_change;
            chances->N -= chance_change;
            chances->NE -= chance_change;
            // + SW, S, SE
            // - NW, N, NE
        }
}

void shift_chances(Directional_chances* chances, float dist_min_x, float dist_max_x, float dist_min_y, float dist_max_y)
{
    if (dist_min_x < 1)
        {
            chances->E += chances->W;
            chances->N += chances->NW / 2;
            chances->NE += chances->NW / 2;
            chances->S += chances->SW / 2;
            chances->SE += chances->SW / 2;

            chances->W = 0;
            chances->NW = 0;
            chances->SW = 0;
        }
        if (dist_max_x < 1)
        {
            chances->W += chances->E;
            chances->N += chances->NE / 2;
            chances->NW += chances->NE / 2;
            chances->S += chances->SE / 2;
            chances->SW += chances->SE / 2;

            chances->E = 0;
            chances->NE = 0;
            chances->SE = 0;
        }
        if (dist_min_y < 1)
        {
            chances->N += chances->S;
            chances->W += chances->SW / 2;
            chances->NW += chances->SW / 2;
            chances->E += chances->SE / 2;
            chances->NE += chances->SE / 2;

            chances->S = 0;
            chances->SW = 0;
            chances->SE = 0;
        }
        if (dist_max_y < 1)
        {
            chances->S += chances->N;
            chances->W += chances->NW / 2;
            chances->SW += chances->NW / 2;
            chances->E += chances->NE / 2;
            chances->SE += chances->NE / 2;

            chances->N = 0;
            chances->NW = 0;
            chances->NE = 0;
        }
}

int direction_from_chances(Directional_chances* chances)
{
    float chance_limit = 1, random;
    int dir;

    random = rand() / (float)RAND_MAX;

    for (int i = 0; i < 8; i++)
    {
        if (random > chance_limit)
        {
            dir = i - 1;
            break;
        }

        switch(i)
        {
            case 0:
                chance_limit -= chances->E;
                break;
            case 1:
                chance_limit -= chances->NE;
                break;
            case 2:
                chance_limit -= chances->N;
                break;
            case 3:
                chance_limit -= chances->NW;
                break;
            case 4:
                chance_limit -= chances->W;
                break;
            case 5:
                chance_limit -= chances->SW;
                break;
            case 6:
                chance_limit -= chances->S;
                break;
            case 7:
                chance_limit -= chances->SE;
                break;
            default:
                break;
        }
    }
    
    return dir * 45;
}