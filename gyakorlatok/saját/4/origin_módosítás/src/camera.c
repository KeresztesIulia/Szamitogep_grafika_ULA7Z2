#include "camera.h"

#include <GL/gl.h>

#include <math.h>

void init_camera(Camera* camera)
{
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->position.z = 1.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
}

void update_camera(Camera* camera, double time)
{
    double angle_x, angle_z;
    double side_angle_x, side_angle_z;

    angle_x = degree_to_radian(camera->rotation.x);
    angle_z = degree_to_radian(camera->rotation.z);

    side_angle_x = degree_to_radian(camera->rotation.x + 90.0);
    side_angle_z = degree_to_radian(camera->rotation.z + 90.0);

    camera->position.x += cos(side_angle_z) * cos(angle_x) * camera->speed.x * time;
    camera->position.y += sin(angle_z) * cos(side_angle_x) * camera->speed.y * time;
    camera->position.z += sin(angle_z) * camera->speed.z * time;

    // camera->position.x += cos(angle) * camera->speed.y * time;
    // camera->position.y += sin(angle) * camera->speed.y * time;
    // camera->position.x += cos(side_angle) * camera->speed.x * time;
    // camera->position.y += sin(side_angle) * camera->speed.x * time;

}

void set_view(const Camera* camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera* camera, double horizontal, double vertical)
{
    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0) {
        camera->rotation.z -= 360.0;
    }

    if (camera->rotation.x < 0) {
        camera->rotation.x += 360.0;
    }

    if (camera->rotation.x > 360.0) {
        camera->rotation.x -= 360.0;
    }
}

void set_camera_speed(Camera* camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera* camera, double speed)
{
    camera->speed.x = speed;
}

void set_camera_vertical_speed(Camera* camera, double speed)
{
    camera->speed.z = speed;
}
