#include <GL/GL.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "scene.h"
#include "utils.h"
#include "game_info.h"
#include "light_tree.h"
#include "texture.h"

#include <obj/load.h>
#include <obj/draw.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>


void init_scene(Scene* scene)
{
    scene->general_material.ambient.red = 1;
    scene->general_material.ambient.green = 1;
    scene->general_material.ambient.blue = 1;

    scene->general_material.diffuse.red = 1.0;
    scene->general_material.diffuse.green = 1.0;
    scene->general_material.diffuse.blue = 1.0;

    scene->general_material.specular.red = 1.0;
    scene->general_material.specular.green = 1.0;
    scene->general_material.specular.blue = 1.0;

    scene->general_material.shininess = 255.0;

    scene->prey_count = MAX_ANIMALS;

    init_limits_textures(scene);

    init_light_tree(&(scene->light_tree));
    init_predator(&(scene->predator));

    init_prey(scene);
    init_fog(scene);
    srand(time(0));

}

void set_lighting()
{
    float ambient_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    float diffuse_light[] = { 0.5f, 0.0f, 0.0f, 1.0f };
    float specular_light[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    float position[] = { 20.0f, 0.0f, 2.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    float diffuse_light2[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    float position2[] = { 0.0f, 20.0f, 2.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT1, GL_POSITION, position2);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene, const vec3* camera_position, const vec3* camera_rotation, double elapsed_time)
{
    int i;
    update_light_tree(&(scene->light_tree), elapsed_time, &(scene->predator.real_position));
    update_predator(&(scene->predator), camera_position, camera_rotation);
    predator_caught_prey(scene);
    for (i = 0; i < MAX_ANIMALS; i++)
    {
        if (scene->prey[i].alive)
        {
            update_prey(&(scene->prey[i]), &(scene->predator.real_position), elapsed_time);
        }
    }
    prey_collision(scene, elapsed_time);
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->general_material));
    set_lighting();
    draw_origin();
    render_light_tree(&(scene->light_tree));

    draw_limits(scene);

}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}

void init_prey(Scene* scene)
{
    vec3 prey_pos;
    int x_min, y_min;
    int coords[MAX_ANIMALS];

    x_min = -X_RANGE / 2;
    y_min = -Y_RANGE / 2;

    randperm(coords, 0, X_RANGE * Y_RANGE - 1, MAX_ANIMALS);

    prey_pos.z = 0;

    for (int i=0; i<MAX_ANIMALS; i++)
    {  
        prey_pos.x = coords[i] / Y_RANGE + x_min;
        prey_pos.y = coords[i] % Y_RANGE + y_min;
        if (i < PREY_1)
        {
            init_rabbit(&(scene->prey[i]), &prey_pos);
        }
        else
        {
            init_chicken(&(scene->prey[i]), &prey_pos);
        }
    }
}

void init_fog(Scene* scene)
{
    scene->fog = 0;
    float fog_color[] = {0.2, 0.5, 0.2, 1.0};
    glFogfv(GL_FOG_COLOR, fog_color);
    glFogf(GL_FOG_DENSITY, 0.3);
    glFogf(GL_FOG_MODE, GL_EXP);
}

void draw_limits(const Scene* scene) //!!!!!
{
    float i, j, size;
    float x_min, x_max, y_min, y_max;

    x_min = -X_RANGE / 2 - 0.2;
    x_max = X_RANGE / 2 + 0.2;
    y_min = -Y_RANGE / 2 - 0.2;
    y_max = Y_RANGE / 2 + 0.2;

    size = 0.5;

    glBegin(GL_QUADS);
    draw_floor_ceiling(scene, x_min, x_max, y_min, y_max, size);
    draw_walls(scene, x_min, x_max, y_min, y_max, size);
    glEnd();
}

void draw_floor_ceiling(Scene* scene, float x_min, float x_max, float y_min, float y_max, float size)
{
    float i, j;
    
    glNormal3f(0,0,1);
    glBindTexture(GL_TEXTURE_2D, 0);
    for (i = x_min; i < x_max; i+=size)
    {
        for (j = y_min; j < y_max; j+=size)
        {
            glTexCoord2f(0, 0); glVertex3f(i, j, 0);
            glTexCoord2f(0, 1); glVertex3f(i, j+size, 0);
            glTexCoord2f(1, 1); glVertex3f(i+size, j+size, 0);
            glTexCoord2f(1, 0); glVertex3f(i+size, j, 0);
        }
    }

    glNormal3f(0,0,-1);
    for (i = x_min; i < x_max; i+=size)
    {
        for (j = y_min; j < y_max; j+=size)
        {
            glVertex3f(i, j, X_RANGE);
            glVertex3f(i, j+size, X_RANGE);
            glVertex3f(i+size, j+size, X_RANGE);
            glVertex3f(i+size, j, X_RANGE);
        }
    }
}

void draw_walls(Scene* scene, float x_min, float x_max, float y_min, float y_max, float size)
{
    float i, j;
    glNormal3f(0,1,0);
    for (i = x_min; i < x_max; i+=size)
    {
        for (j = 0; j < X_RANGE; j+=size)
        {
            glVertex3f(i, y_min, j);
            glVertex3f(i, y_min, j+size);
            glVertex3f(i+size, y_min, j+size);
            glVertex3f(i+size, y_min, j);
        }
    }

    glNormal3f(0,-1,0);
    for (i = x_min; i < x_max; i+=size)
    {
        for (j = 0; j < X_RANGE; j+=size)
        {
            glVertex3f(i, y_max, j);
            glVertex3f(i, y_max, j+size);
            glVertex3f(i+size, y_max, j+size);
            glVertex3f(i+size, y_max, j);
        }
    }

    glNormal3f(1,0,0);
    for (i = y_min; i < y_max; i+=size)
    {
        for (j = 0; j < X_RANGE; j+=size)
        {
            glVertex3f(x_min, i, j);
            glVertex3f(x_min, i, j+size);
            glVertex3f(x_min, i+size, j+size);
            glVertex3f(x_min, i+size, j);
        }
    }

    glNormal3f(-1,0,0);
    for (i = y_min; i < y_max; i+=size)
    {
        for (j = 0; j < X_RANGE; j+=size)
        {
            glVertex3f(x_max, i, j);
            glVertex3f(x_max, i, j+size);
            glVertex3f(x_max, i+size, j+size);
            glVertex3f(x_max, i+size, j);
        }
    }
}

void render_predator(const Scene* scene)
{
    render_the_predator(&(scene->predator));
}

void render_all_prey(const Scene* scene)
{
    for(int i = 0; i < MAX_ANIMALS; i++)
    {
        if(scene->prey[i].alive)
        {
            render_prey(&(scene->prey[i]));
        }
    }
    
}

void change_fog(Scene* scene)
{
    if (scene->fog)
    {
        scene->fog = 0;
        glFogf(GL_FOG_DENSITY, 0.3);
    }
    else
    {
        scene->fog = 1;
        glFogf(GL_FOG_DENSITY, 0.1);
    }
}

void predator_caught_prey(Scene* scene)
{
    int i;
    float dist_x, dist_y, dist;
    for (i = 0; i < MAX_ANIMALS; i ++)
    {
        if (scene->prey[i].alive)
        {
            dist_x = scene->predator.real_position.x - scene->prey[i].position.x;
            dist_y = scene->predator.real_position.z - scene->prey[i].position.y;
            dist = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

            if (dist < COLLISION_DISTANCE)
            {
                scene->predator.points += scene->prey[i].points;
                printf("caught %d, points: %f\n", i, scene->predator.points);
                scene->prey[i].alive = false;
                scene->prey_count--;
            }
        }
    }
}

void prey_collision(Scene* scene, float elapsed_time)
{
    int i, j;
    for (i = 0; i < MAX_ANIMALS-1; i++)
    {
        for (j = i+1; j<MAX_ANIMALS; j++)
        {
            if (scene->prey[i].alive && scene->prey[j].alive)
            {
                check_prey_collision(&(scene->prey[i]), &(scene->prey[j]), elapsed_time);    
            }
        }
    }
}

void activate_light_tree(Scene* scene)
{
    if (predator_close_enough(&(scene->predator), &(scene->light_tree)))
    {
        strengthen_light(&(scene->light_tree));
    }
}

bool predator_close_enough(const Predator* predator, Light_tree* light_tree)
{
    float distance = sqrt(pow(predator->real_position.x, 2) + pow(predator->real_position.z, 2));
    return distance < light_tree->activation_distance;
}

void init_limits_textures(Scene* scene)
{
    //.......................
}

void init_SDL_TTF(TTF_Text* ttf_text, SDL_Window* window)
{
    ttf_text->text_to_write = "szoveggg";
    if (TTF_Init() < 0)
    {
        printf("TTF init ERROR\n");
        return;
    }

    ttf_text->font = TTF_OpenFont("assets/fonts/ariblk.ttf", 70);
    if (ttf_text->font == NULL)
    {
        printf("Font ERROR\n");
        return;
    }

    TTF_SetFontStyle(ttf_text->font, TTF_STYLE_NORMAL);
    TTF_SetFontOutline(ttf_text->font, 0);
    TTF_SetFontKerning(ttf_text->font, 1);
    TTF_SetFontHinting(ttf_text->font, TTF_HINTING_NORMAL);
    SDL_Color text_color = { 255.0f, 255.0f, 255.0f, 255};
    ttf_text->text_color = text_color;

    ttf_text->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (ttf_text->renderer == NULL) {
        printf("Renderer ERROR\n");
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(ttf_text->font, ttf_text->text_to_write, ttf_text->text_color);
    if (surface != NULL)
    {
        ttf_text->captionRect.x = 20.0f;
        ttf_text->captionRect.y = 20.0f;
        ttf_text->captionRect.w = surface->w;
        ttf_text->captionRect.h = surface->h;
        ttf_text->caption = SDL_CreateTextureFromSurface(ttf_text->renderer, surface);
        if (ttf_text->caption == NULL)
        {
            printf("create_texture ERROR\n");
        }
    }
    else
    {
        printf("surface ERROR\n");
    }
}

