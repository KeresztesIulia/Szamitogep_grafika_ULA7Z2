#include "predator.h"
#include "utils.h"
#include "game_info.h"

#include <math.h>
#include <obj/model.h>
#include <obj/draw.h>

void init_predator(Predator* predator)
{    
    predator->real_position.x = 0.0;
    predator->real_position.y = 0.0;
    predator->real_position.z = 0.0;

    predator->relative_position.x = 0.0;
    predator->relative_position.y = -0.6; //height
    predator->relative_position.z = -1.0; //forward

    predator->rotation.x = 0;
    predator->rotation.y = 0;
    predator->rotation.z = 0;

    predator->points = 0;

    predator->direction = 0;

    load_model(&(predator->model), "assets/models/fox.obj");
    predator->texture_id = load_texture("assets/textures/fox.png");

    glBindTexture(GL_TEXTURE_2D, predator->texture_id);
}


void render_the_predator(const Predator* predator)
{
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, predator->texture_id);
    
    glRotatef(predator->rotation.x, 1, 0, 0);
    glTranslatef(predator->relative_position.x, predator->relative_position.y, predator->relative_position.z);
    glRotatef(predator->rotation.z, 0, 0, 1);
    glRotatef(predator->rotation.y, 0, 1, 0);
    glScalef(0.1, 0.1, 0.1);
    
    draw_model(&(predator->model));
    glPopMatrix();
}

void update_predator(Predator* predator, const vec3* camera_position, const vec3* camera_rotation)
{
    update_predator_rotation(predator, camera_rotation);
    update_predator_position(predator, camera_position);
}

void update_predator_rotation(Predator* predator, const vec3* camera_rotation)
{
    predator->rotation.x = -camera_rotation->x;
    predator->direction = camera_rotation->z+90;
}

void update_predator_position(Predator* predator, const vec3* camera_position)
{
    double rotation_angle = degree_to_radian(predator->direction);
    float cam_x = camera_position->x;
    float cam_y = camera_position->y;
    float cam_z = camera_position->z;

    predator->real_position.x = cam_x - sin(rotation_angle) * predator->relative_position.z;
    predator->real_position.z = cam_y + cos(rotation_angle) * predator->relative_position.z;
    predator->real_position.y = 0;
}


bool predator_out_of_range(vec3* position)
{
    int x_min, x_max, y_min, y_max;
    x_min = -X_RANGE / 2;
    x_max = X_RANGE / 2;
    y_min = -Y_RANGE / 2;
    y_max = Y_RANGE / 2;

    if (position->x < x_min || position->x > x_max || position->z < y_min || position->z > y_max)
    {
        return true;
    }
    return false;
}