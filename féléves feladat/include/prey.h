#ifndef PREY_H
#define PREY_H

#include "utils.h"
#include "texture.h"

#include <obj/model.h>
#include <stdbool.h>

#define POINT_LOSS_PER_SECOND 1

typedef struct Prey
{
    Model model;
    GLuint texture_id;
    vec3 position;
    float z_rotation;
    float max_dist;
    float speed_multiplier;

    float scaling;

    float min_points;
    float points;

    bool alive;
} Prey;

void init_rabbit(Prey* rabbit, vec3* position);
void init_chicken(Prey* chicken, vec3* position);
void init_prey_common(Prey* prey, vec3* position);
void update_prey(Prey* prey, const vec3* predator_pos, double elapsed_time);
void prey_runaway(Prey* prey, vec3* direction, float distance, float elapsed_time);
bool move_prey(Prey* prey, vec3* direction, float angle, float dist_speed, float elapsed_time);

/**
 * The prey may move randomly when not threatened by the predator
*/
void prey_wander(Prey* prey, float elapsed_time);

void render_prey(const Prey* prey);
int prey_out_of_range(vec3* position, float angle);
void go_new_direction(Prey* prey, float dist_speed, double elapsed_time, double angle);
float calc_angle(vec3* direction);
void check_prey_collision(Prey* prey1, Prey* prey2, float elapsed_time);
void update_after_collision(Prey* prey1, Prey* prey2, float elapsed_time);
bool same_dir_prey(float rotation1, float rotation2);

void update_points(Prey* prey, double elapsed_time);

#endif