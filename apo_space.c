#define _POSIX_C_SOURCE 200112L

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "game_state.h"
#include "input.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "queue.h"
#include "render.h"

int main(int argc, char *argv[])
{
    pthread_t game_thread;
    pthread_t input_thread;
    pthread_t render_thread;

    bool stop_game = false;
    GameState state = init_gamestate();
    Queue input_queue = init_queue();

    InputArgs input_args = (InputArgs){&input_queue, &stop_game};
    GameStateArgs game_state_args = (GameStateArgs){&state, &input_queue, &stop_game};
    RenderArgs render_args = (RenderArgs){&state.rotation_set_point, &state.position,       &state.planets,
                                          &state.planet_count,       &state.current_planet, &state.remaining_fuel,
                                          &state.player_state,       &state.show_map,       &stop_game};

    pthread_create(&input_thread, NULL, loop_input_collection, &input_args);
    pthread_create(&game_thread, NULL, loop_game_state, &game_state_args);
    pthread_create(&render_thread, NULL, loop_render, &render_args);

    while (!stop_game)
    {
        usleep(100);
    }

    pthread_join(input_thread, NULL);
    pthread_join(game_thread, NULL);
    pthread_join(render_thread, NULL);

    return 0;
}
