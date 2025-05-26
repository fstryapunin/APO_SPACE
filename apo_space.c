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
#include "io.h"
#include "render.h"

int main(int argc, char *argv[])
{
  unsigned char *memory_base = (unsigned char *)init_memory();

  pthread_t game_thread;
  pthread_t input_thread;
  pthread_t render_thread;

  printf("Hello world\n");

  Input input = init_input(memory_base);
  input.acceleration_input = 100;
 
  GameState state = init_gamestate();

  GameStateArgs game_state_args = { &state, &(input.acceleration_input), &(input.rotation_input), false };
  InputArgs input_args = { &input, false, memory_base };
  RenderArgs render_args = { 
    &(state.remaining_fuel), 
    &(state.rotation_radians),
    &(state.position),
    &(state.player_state),
    &(state.planets),
    &(state.planet_count),
    &(state.nearest_planet),
    false
  };

  pthread_create(&game_thread, NULL, loop_game_state, &game_state_args);
  pthread_create(&input_thread, NULL, loop_input_collection, &input_args);
  pthread_create(&render_thread, NULL, loop_render, &render_args);

  sleep(100);

  game_state_args.stop = true;
  input_args.stop = true;
  render_args.stop = true;

  pthread_join(game_thread, NULL);
  pthread_join(input_thread, NULL);
  pthread_join(render_thread, NULL);

  printf("Bye world\n");

  return 0;
}
