#ifndef APP_H
#define APP_H

#include "camera.h"
#include "scene.h"

#include <SDL2/SDL.h>

#include <stdbool.h>

#define VIEWPORT_RATIO (4.0 / 3.0)
#define VIEWPORT_ASPECT 50.0
#define PAUSE_IMAGE_COUNT 4

typedef enum {RUNNING, PAUSED, FINISHED, RESTART, EXIT} Status;

typedef struct App
{
    SDL_Window* window;
    SDL_GLContext gl_context;
    Status status;
    double uptime;
    Camera camera;
    Scene scene;

    GLuint pause_textures[PAUSE_IMAGE_COUNT];
    int pause_active_image_index;

    GLuint end_texture;
} App;

/**
 * Initialize the application.
 */
void init_app(App* app, int width, int height);

/**
 * Initialize the OpenGL context.
 */
void init_opengl();

/**
 * Reshape the window.
 */
void reshape(GLsizei width, GLsizei height);

/**
 * Handle the events of the application when not paused.
 */
void handle_app_running_events(App* app);

/**
 * Update the application.
 */
void update_app(App* app);

/**
 * Render the application.
 */
void render_app(App* app);

/**
 * Destroy the application.
 */
void destroy_app(App* app);


void handle_app_paused_events(App* app);
void render_menu(App* app);
void render_text(TTF_Text* ttf_text);
void init_pause_images(App* app);

void link_camera_and_predator(App* app);

void handle_game_end_events(App* app);
void render_end(App* app);
void restart(App* app);


#endif /* APP_H */
