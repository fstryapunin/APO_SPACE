#include "render.h"

// void render_state(int fuel, double rotation, Vector position, bool is_stopped, bool is_crashed, Planet *planets);
// void render_map(Vector position, Planet *planets);
// void render_menu(MenuState state);

void *loop_render(RenderArgs *args){
    while (!args->stop)
    {
        

        usleep(RENDER_DELAY_MS * 1000);
    }
};