#include "app.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <GL/gl.h>

void init_app(App* app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->status = PAUSED;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Hunt!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);
    init_camera(&(app->camera));
    init_scene(&(app->scene));
    link_predator_to_camera(&(app->camera), &(app->scene.predator));
    init_pause_images(app);

    app->end_texture = load_texture("assets/textures/end.png");


    init_SDL_TTF(&(app->scene.ttf_text), app->window);

    app->uptime = 0;
    app->status = RUNNING;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.2, 0.5, 0.2, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
        
    //glEnable(GL_FOG);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 25
    );
}

void handle_app_running_events(App* app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->status = EXIT;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_SPACE:
                app->status = PAUSED;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down) {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
                rotate_around_predator(&(app->camera), mouse_x - x);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->status = EXIT;
            break;
        default:
            break;
        }
    }
}

void update_app(App* app)
{
    double current_time;
    double elapsed_time;

    if(app->scene.prey_count == 0)
    {
        app->status = FINISHED;
    }

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene), &(app->camera.position), &(app->camera.rotation), elapsed_time);
}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    render_predator(&(app->scene));

    glLoadIdentity();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    
    render_all_prey(&(app->scene));

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    render_text(&(app->scene.ttf_text));
    SDL_GL_SwapWindow(app->window);
}

void handle_app_paused_events(App* app)
{
    SDL_Event event;
    double current_time;

    current_time = (double)SDL_GetTicks() / 1000;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    app->status = EXIT;
                    break;
                case SDL_SCANCODE_SPACE:
                    app->uptime = current_time;
                    app->status = RUNNING;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    if (app->pause_active_image_index != 0)
                    {
                        app->pause_active_image_index--;
                    }
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    if (app->pause_active_image_index != PAUSE_IMAGE_COUNT - 1)
                    {
                        app->pause_active_image_index++;
                    }
                    break;
                case SDL_SCANCODE_R:
                    restart(app);
                    break;
                default:
                    break;
            }
        }  
    }
}

void render_menu(App* app)
{
    glDisable(GL_FOG);
    glBindTexture(GL_TEXTURE_2D, app->pause_textures[app->pause_active_image_index]);
    show_texture_preview();
    SDL_GL_SwapWindow(app->window);
    glEnable(GL_FOG);
}

void render_text(TTF_Text* ttf_text)
{
    SDL_SetRenderDrawColor(ttf_text->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ttf_text->renderer);
    SDL_RenderCopy(ttf_text->renderer, ttf_text->caption, NULL, &(ttf_text->captionRect));
    SDL_RenderPresent(ttf_text->renderer);
}

void init_pause_images(App* app)
{
    int i;
    char texture_path[30];
    for (i = 0; i < PAUSE_IMAGE_COUNT; i++)
    {
        snprintf(texture_path, 30, "assets/textures/pause-%d.png", i);
        app->pause_textures[i] = load_texture(texture_path);
    }
    app->pause_active_image_index = 0;
}

void destroy_app(App* app)
{
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}

void handle_game_end_events(App* app)
{
    SDL_Event event;
    double current_time;

    current_time = (double)SDL_GetTicks() / 1000;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    app->status = EXIT;
                    break;
                case SDL_SCANCODE_SPACE:
                case SDL_SCANCODE_R:
                    restart(app);
                    break;
                default:
                    break;
            }
        }  
    }
}

void render_end(App* app)
{
    glDisable(GL_FOG);
    glBindTexture(GL_TEXTURE_2D, app->end_texture);
    show_texture_preview();
    SDL_GL_SwapWindow(app->window);
}

void restart(App* app)
{
    SDL_DestroyWindow(app->window);
    app->status = RESTART;
}