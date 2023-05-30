#ifndef PREDATOR_H
#define PREDATOR_H

#include "utils.h"
#include "texture.h"
#include <obj/model.h>
#include <stdbool.h>

typedef struct Predator
{
    Model model;
    GLuint texture_id;

    vec3 relative_position;
    vec3 real_position;
    vec3 rotation;

    float points;

    float direction;
    bool in_range;
} Predator;

void init_predator(Predator* predator);
void render_the_predator(const Predator* predator);
void update_predator(Predator* predator, const vec3* camera_position, const vec3* camera_rotation);
void update_predator_position(Predator* predator, const vec3* camera_position);
void update_predator_rotation(Predator* predator, const vec3* camera_rotation);
bool predator_out_of_range(vec3* position);

#endif