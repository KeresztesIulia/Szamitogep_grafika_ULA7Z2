#include "app.h"

#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

/**
 * Main function
 */
int main(int argc, char* argv[])
{
    App app;

    init_app(&app, WIDTH, HEIGHT);
    while (app.status != EXIT)
    {
        if (app.status == RUNNING)
        {
            handle_app_running_events(&app);
            update_app(&app);
            render_app(&app);
        }
        if (app.status == PAUSED)
        {
            handle_app_paused_events(&app);
            render_menu(&app);
        }
        if(app.status == FINISHED)
        {
            handle_game_end_events(&app);
            render_end(&app);
        }
        if(app.status == RESTART)
        {
            init_app(&app, WIDTH, HEIGHT);
        }
    }
    destroy_app(&app);

    return 0;
}
