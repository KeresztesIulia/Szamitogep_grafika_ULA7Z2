#ifndef COLOR.H
#define COLOR.H

typedef struct Color
{
    int R;
    int G;
    int B;
    float A;
} Color;

void set_color_data(Color* color, int R, int G, int B, float A=1);
#endif