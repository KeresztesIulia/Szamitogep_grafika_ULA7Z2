#include "light_tree.h"
#include "utils.h"

#include <obj/load.h>
#include <limits.h>
#include <math.h>

void init_light_tree(Light_tree* light_tree)
{
    load_model(&(light_tree->model), "assets/models/light_tree.obj");
    light_tree->texture_id = load_texture("assets/textures/light_tree.jpg");

    light_tree->charges = INT_MAX;
    light_tree->duration = 30;
    light_tree->remaining = 0;
    light_tree->active = false;
    light_tree->predator_touching = false;
    light_tree->activation_distance = 0.35;

    light_tree->weak_light_fog_density = 0.3;
    light_tree->strong_light_fog_density = 0.2;
    
}

void update_light_tree(Light_tree* light_tree, float elapsed_time, vec3* predator_position)
{
    float distance = sqrt(pow(predator_position->x, 2) + pow(predator_position->z, 2));

    if (distance <= light_tree->activation_distance)
    {
        if (!light_tree->predator_touching)
        {
            strengthen_light(light_tree);    
        }
        light_tree->predator_touching = true;
    }
    else
    {
        light_tree->predator_touching = false;
    }
    if (light_tree->active)
    {
        light_tree->remaining -= elapsed_time;
        if (light_tree->remaining < 0)
        {
            light_back_to_normal(light_tree);
        }
    }
    
}

void strengthen_light(Light_tree* light_tree)
{
    if (light_tree->charges > 0)
    {
        light_tree->charges--;

        light_tree->remaining = light_tree->duration;
        glFogf(GL_FOG_DENSITY, light_tree->strong_light_fog_density);

        light_tree->active = true;
        printf("ACTIVATED! charges left: %d\n", light_tree->charges);
    }
}

void light_back_to_normal(Light_tree* light_tree)
{
    glFogf(GL_FOG_DENSITY, light_tree->weak_light_fog_density);
    light_tree->active = false;
}

void render_light_tree(const Light_tree* light_tree)
{
    glDisable(GL_FOG);
    glBindTexture(GL_TEXTURE_2D, light_tree->texture_id);
    draw_model(&(light_tree->model));
    glEnable(GL_FOG);
}