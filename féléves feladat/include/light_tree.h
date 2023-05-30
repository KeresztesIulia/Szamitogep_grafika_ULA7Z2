#ifndef LIGHT_TREE_H
#define LIGHT_TREE_H

#include <stdbool.h>
#include <obj/model.h>

#include "texture.h"
#include "utils.h"

typedef struct Light_tree
{
    Model model;
    GLuint texture_id;

    int charges;
    float duration;
    float remaining;
    bool active;
    bool predator_touching;
    float activation_distance;

    float weak_light_fog_density;
    float strong_light_fog_density;
} Light_tree;

void init_light_tree(Light_tree* light_tree);

void update_light_tree(Light_tree* light_tree, float elapsed_time, vec3* predator_position);

void strengthen_light(Light_tree* light_tree);
void light_back_to_normal(Light_tree* light_tree);

void render_light_tree(const Light_tree* light_tree);

#endif