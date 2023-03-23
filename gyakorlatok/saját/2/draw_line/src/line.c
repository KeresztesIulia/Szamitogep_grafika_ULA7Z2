#include "line.h"

void set_line_start_point(Line* line, Vector* start_point)
{
    line->start_point = *start_point;
}

void set_line_end_point(Line* line, Vector* end_point)
{
    line->end_point = *end_point;
}

void set_line_color(Line* line, Color* color)
{
    line->color = *color;
}

void draw_line(Line* line); //from line->start_point to line->end_point, with line->color color
