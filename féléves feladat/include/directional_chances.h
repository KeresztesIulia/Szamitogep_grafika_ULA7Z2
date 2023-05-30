#ifndef DIRECTIONAL_CHANCES_H
#define DIRECTIONAL_CHANCES_H

typedef struct Directional_chances
{
    float N;
    float NW;
    float W;
    float SW;
    float S;
    float SE;
    float E;
    float NE;
} Directional_chances;

void init_chances(Directional_chances* chances);
void calc_base_chances(Directional_chances* chances, float dist_min_x, float dist_max_x, float dist_min_y, float dist_max_y);
void shift_chances(Directional_chances* chances, float dist_min_x, float dist_max_x, float dist_min_y, float dist_max_y);
int direction_from_chances(Directional_chances* chances);

#endif