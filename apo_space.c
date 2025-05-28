#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "game_state.h"
#include "input.h"
#include "render.h"
#include "queue.h"

int main(int argc, char *argv[])
{
  printf("Hello space!\n");
  pthread_t game_thread;
  pthread_t input_thread;
  pthread_t render_thread;

  bool stop_game = false;

  Queue input_queue = init_queue();

  InputArgs input_args = (InputArgs) { &input_queue, &stop_game };

  // GameState state = init_gamestate();

  // GameStateArgs game_state_args = { &state, &(input.acceleration_input), &(input.rotation_input), false };
  // InputArgs input_args = { &input, false, memory_base };
  // RenderArgs render_args = { 
  //   &(state.remaining_fuel), 
  //   &(state.rotation_radians),
  //   &(state.position),
  //   &(state.player_state),
  //   &(state.planets),
  //   &(state.planet_count),
  //   &(state.nearest_planet),
  //   false
  // };

  pthread_create(&input_thread, NULL, loop_input_collection, &input_args);
  // pthread_create(&game_thread, NULL, loop_game_state, &game_state_args);
  // pthread_create(&render_thread, NULL, loop_render, &render_args);

  while (!stop_game)
  {

    wait_for_queue_lock(&input_queue);

    if(input_queue.count > 0){
      InputEvent event = dequeu_input_event(&input_queue);
      printf("Input event %d\n", event);
    }

    release_queue_lock(&input_queue);
    usleep(100);
  }

  pthread_join(input_thread, NULL);
  // pthread_join(game_thread, NULL);
  // pthread_join(render_thread, NULL);

  return 0;
}
