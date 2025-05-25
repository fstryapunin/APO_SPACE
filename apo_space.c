/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  change_me.c      - main file

  include your name there and license for distribution.

  Remove next text: This line should not appear in submitted
  work and project name should be change to match real application.
  If this text is there I want 10 points subtracted from final
  evaluation.

 *******************************************************************/

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

int main(int argc, char *argv[])
{
  pthread_t game_thread;
  printf("Hello world\n");

  Input input = init_input();
  input.acceleration_input = 100;
 
  GameState state = init_gamestate();

  GameStateArgs args = { &state, &(input.acceleration_input), &(input.rotation_input) };
  
  pthread_create(&game_thread, NULL, loop_game_state, &args);

  sleep(10);

  args.stop = true;

  pthread_join(game_thread, NULL);

  printf("Bye world\n");

  return 0;
}
