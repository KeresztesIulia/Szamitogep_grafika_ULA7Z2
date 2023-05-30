#include "prey.h"
#include "utils.h"
#include "game_info.h"
#include "directional_chances.h"

#include <stdlib.h>
#include <math.h>
#include <obj/model.h>
#include <obj/draw.h>
#include <stdbool.h>

void init_rabbit(Prey* rabbit, vec3* position)
{
    init_prey_common(rabbit, position);

    rabbit->points = 500;
    rabbit->min_points = 100;

    load_model(&(rabbit->model), "assets/models/rabbit.obj");
    rabbit->texture_id = load_texture("assets/textures/rabbit.png");

    rabbit->scaling = 0.25;

    glBindTexture(GL_TEXTURE_2D, rabbit->texture_id);
}

void init_chicken(Prey* chicken, vec3* position)
{
    init_prey_common(chicken, position);

    chicken->points = 300;
    chicken->min_points = 50;

    load_model(&(chicken->model), "assets/models/chicken.obj");
    chicken->texture_id = load_texture("assets/textures/chicken.png");

    chicken->scaling = 1.5f;

    glBindTexture(GL_TEXTURE_2D, chicken->texture_id);
}

void init_prey_common(Prey* prey, vec3* position)
{
    prey->position.x = position->x;
    prey->position.y = position->y;
    prey->position.z = position->z;
    
    prey->z_rotation = 0.0;

    prey->max_dist = 5;
    prey->speed_multiplier = 0.5;
    prey->alive = true;
}

void update_prey(Prey* prey, const vec3* predator_pos, double elapsed_time)
{
    float distance;
    vec3 direction;

    direction.x = prey->position.x - predator_pos->x;
    direction.y = prey->position.y - predator_pos->z;
    direction.z = prey->position.z - predator_pos->y;

    distance = sqrt(pow(direction.x, 2) + pow(direction.y, 2) + pow(direction.z, 2));

    if (distance <= prey->max_dist)
    {
        prey_runaway(prey, &direction, distance, elapsed_time);
    }
    else
    {
        prey_wander(prey, elapsed_time);
    }

    if(prey->z_rotation < 0)
    {
        prey->z_rotation += 360;
    }
    if(prey->z_rotation > 360)
    {
        prey->z_rotation -= 360;
    }

    update_points(prey, elapsed_time);

}

void prey_runaway(Prey* prey, vec3* direction, float distance, float elapsed_time)
{
    float dist_speed, angle;
    
    if (distance == 0)
    {
        distance = 0.00001;
    }

    //normalize direction vector
    direction->x /= distance;
    direction->y /= distance;
    direction->z /= distance;

    angle = calc_angle(direction);

    dist_speed = pow(fmax(1 - distance/prey->max_dist, 0), 0.25);

    if (move_prey(prey, direction, angle, dist_speed, elapsed_time) && distance < prey->max_dist)
    {
        prey->z_rotation = angle-90;
    }
}

bool move_prey(Prey* prey, vec3* direction, float angle, float dist_speed, float elapsed_time)
{
    int new_dir;
    vec3 new_position;

    new_position.x = prey->position.x + direction->x * dist_speed * elapsed_time * prey->speed_multiplier;
    new_position.y = prey->position.y + direction->y * dist_speed * elapsed_time * prey->speed_multiplier;
    new_position.z = prey->position.z + direction->z * dist_speed * elapsed_time * prey->speed_multiplier;

    new_dir = prey_out_of_range(&new_position, angle);

    if (new_dir != -1)
    {
        go_new_direction(prey, dist_speed, elapsed_time, new_dir);
        return false;
    }
    else
    {
        prey->position = new_position;
        return true;
    }
}

void prey_wander(Prey* prey, float elapsed_time)
{
    int will_move;
    Directional_chances chances;
    int x_min, x_max, y_min, y_max;
    float dist_min_x, dist_max_x, dist_min_y, dist_max_y;
    int angle;
    
    will_move = rand() % 400;

    if (will_move == 50) // any number, doesn't matter
    {
        init_chances(&chances);

        x_min = -X_RANGE / 2;
        x_max = X_RANGE / 2;
        y_min = -Y_RANGE / 2;
        y_max = Y_RANGE / 2;

        dist_min_x = fabs(x_min - prey->position.x);
        dist_max_x = fabs(x_max - prey->position.x);
        dist_min_y = fabs(y_min - prey->position.y);
        dist_max_y = fabs(y_max - prey->position.y);

        calc_base_chances(&chances, dist_min_x, dist_max_x, dist_min_y, dist_max_y);
        shift_chances(&chances, dist_min_x, dist_max_x, dist_min_y, dist_max_y);
        angle = direction_from_chances(&chances);

        go_new_direction(prey, 100, elapsed_time, angle);
    }
}

void render_prey(const Prey* prey)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, prey->texture_id);
    
    glTranslatef(prey->position.x, prey->position.y, prey->position.z);
    glRotatef(prey->z_rotation, 0, 0, 1);
    glScalef(prey->scaling, prey->scaling, prey->scaling);

    draw_model(&(prey->model));
    glPopMatrix();
}

int prey_out_of_range(vec3* position, float angle)
{
    float angle1, angle2;
    int x_min, x_max, y_min, y_max;
    x_min = -X_RANGE / 2;
    x_max = X_RANGE / 2;
    y_min = -Y_RANGE / 2;
    y_max = Y_RANGE / 2;

    if (position->x < x_min || position->x > x_max || position->y < y_min || position->y > y_max)
    {
        if (position->x < x_min && position->y < y_min)
        {
            angle1 = 90;
            angle2 = 0;
        }
        else if (position->x < x_min && position->y > y_max)
        {
            angle1 = 0;
            angle2 = 270;
        }
        else if (position->x > x_max && position->y < y_min)
        {
            angle1 = 90;
            angle2 = 180;
        }
        else if (position->x > x_max && position->y > y_max)
        {
            angle1 = 180;
            angle2 = 270;
        }
        else if (position->x < x_min || position->x > x_max)
        {
            angle1 = 90;
            angle2 = 270;
        }
        else if (position->y < y_min || position->y > y_max)
        {
            angle1 = 0;
            angle2 = 180;
        }
        return closer_angle(angle, angle1, angle2);
    }
    return -1;
}

void go_new_direction(Prey* prey, float dist_speed, double elapsed_time, double angle)
{
    float rotation_amount;
    vec3 direction;

    prey->z_rotation = angle-90;
    rotation_amount = degree_to_radian(prey->z_rotation);

    direction.x = -sin(rotation_amount);
    direction.y = cos(rotation_amount);

    prey->position.x += direction.x * dist_speed * elapsed_time * prey->speed_multiplier;
    prey->position.y += direction.y * dist_speed * elapsed_time * prey->speed_multiplier;
}

float calc_angle(vec3* direction)
{
    if (direction->y > 0)
    {
        if (direction->x == 0)
        {
            return 90;
        }
        else if (direction->x > 0)
        {
            return atan(direction->y/direction->x) * 180 / M_PI;
        }
        else
        {
            return 180 + atan(direction->y/direction->x) * 180 / M_PI;
        }
    }
    else if (direction->y == 0)
    {
        if (direction->x == 0)
        {
            return 0;
        }
        else if (direction->x < 0)
        {
            return 180;
        }
        else
        {
            return 0;
        }
    }
    else // y < 0
    {
        if (direction->x == 0)
        {
            return 270;
        }
        else if (direction->x > 0)
        {
            return atan(direction->y/direction->x) * 180 / M_PI;
        }
        else
        {
            return 180 + atan(direction->y/direction->x) * 180 / M_PI;
        }
    }
}

void check_prey_collision(Prey* prey1, Prey* prey2, float elapsed_time)
{
    float dist_x, dist_y, dist;

    dist_x = fabs(prey1->position.x - prey2->position.x);
    dist_y = fabs(prey1->position.y - prey2->position.y);

    dist = sqrt(pow(dist_x,2) + pow(dist_y,2));

    if (dist < COLLISION_DISTANCE)
    {
        if (same_dir_prey(prey1->z_rotation, prey2->z_rotation))
        {
            prey1->z_rotation += 60 * elapsed_time;
            prey2->z_rotation -= 60 * elapsed_time;
        }
        else
        {
            prey1->z_rotation += 60 * elapsed_time;
            prey2->z_rotation += 60 * elapsed_time;
        }

        update_after_collision(prey1, prey2, elapsed_time);
    }
}

void update_after_collision(Prey* prey1, Prey* prey2, float elapsed_time)
{
    float rotation_radian1, rotation_radian2, angle;
    vec3 direction;

    rotation_radian1 = degree_to_radian(prey1->z_rotation);
    rotation_radian2 = degree_to_radian(prey2->z_rotation);

    direction.x = -sin(rotation_radian1);
    direction.y = cos(rotation_radian1);
    direction.z = 0;
    angle = calc_angle(&direction);
    move_prey(prey1, &direction, angle, 1, elapsed_time);
    
    direction.x = -sin(rotation_radian2);
    direction.y = cos(rotation_radian2);
    direction.z = 0;
    angle = calc_angle(&direction);
    move_prey(prey2, &direction, angle, 1, elapsed_time);
}

bool same_dir_prey(float rotation1, float rotation2)
{
    float rotation_difference = fabs(rotation1 - rotation2);
    
    if (rotation_difference < 90 || rotation_difference > 270)
    {
        return true;
    }
    return false;
}

void update_points(Prey* prey, double elapsed_time)
{
    if (prey->points != prey->min_points)
    {
        prey->points -= POINT_LOSS_PER_SECOND * elapsed_time;
        if (prey->points < prey->min_points)
        {
            prey->points = prey->min_points;
        }
    }
}