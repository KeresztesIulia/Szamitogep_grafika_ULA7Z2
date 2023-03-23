#include "color.h"

void set_color_data(Color* color, int R, int G, int B, float A)
{
    if (A < 0) A = 0;
    if (A > 1) A = 1;

    color->R = R;
    color->G = G;
    color->B = B;
    color->A = A;
}
    