#ifndef SCENE_H
#define SCENE_H

#include "predator.h"
#include "prey.h"
#include "game_info.h"
#include "light_tree.h"

#include <obj/model.h>
#include <SDL2/SDL_TTF.h>
#include <SDL2/SDL.h>

#include <stdbool.h>

#include "texture.h"
#include "utils.h"


typedef struct TTF_Text
{
    TTF_Font* font;
    char* text_to_write;
    SDL_Color text_color;
    SDL_Texture *caption;
    SDL_FRect captionRect;
    SDL_Renderer* renderer;
} TTF_Text;

typedef struct Scene
{
    Material general_material;
    GLuint ground_texture;
    GLuint wall_texture;
    GLuint ceiling_texture;
    Light_tree light_tree;
    Predator predator;
    int fog;
    Prey prey[MAX_ANIMALS];
    int prey_count;

    TTF_Text ttf_text; //...
} Scene;



/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, const vec3* camera_position, const vec3* camera_rotation, double elapsed_time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

void render_the_light_tree(const Scene* scene);

void init_prey(Scene* scene);
void init_fog(Scene* scene);
void render_predator(const Scene* scene);
void render_all_prey(const Scene* scene);
void draw_limits(const Scene* scene);
void draw_floor_ceiling(Scene* scene, float x_min, float x_max, float y_min, float y_max, float size);
void draw_walls(Scene* scene, float x_min, float x_max, float y_min, float y_max, float size);
void change_fog(Scene* scene);
void predator_caught_prey(Scene* scene);
void prey_collision(Scene* scene, float elapsed_time);

void activate_light_tree(Scene* scene);
bool predator_close_enough(const Predator* predator, Light_tree* light_tree);

void init_limits_textures(Scene* scene);
void init_SDL_TTF(TTF_Text* ttf_text, SDL_Window* window);




#endif /* SCENE_H */
