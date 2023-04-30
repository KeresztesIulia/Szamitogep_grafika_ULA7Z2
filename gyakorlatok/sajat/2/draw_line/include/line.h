#include "vector.h"
#include "color.h"

#ifndef LINE.H
#define LINE.H

typedef struct Line
{
    Vector start_point;
    Vector end_point;
    Color color;
} Line;

void set_line_start_point(Line* line, Vector* start_point);

void set_line_end_point(Line* line, Vector* end_point);

void set_line_color(Line* line, Color* color);

void draw_line();


#endif