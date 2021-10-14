/** @file   game.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   10 Oct 2021
 *  @brief  Main task control
 */

#include "system.h"
#include "task.h"
#include "display.h"
#include "character.h"
#include "wall.h"
#include "game_manager.h"
#include "tinygl.h"
#include "sound.h"

//Frequency of task execution in hz
#define DISPLAY_UPDATE_RATE            300
#define INPUT_UPDATE_RATE              20
#define MESSAGE_RATE                   20  // Tinygl text scroll speed

//Sound rate constants
#define TWEETER_TASK_RATE              5000
#define MELODY_TASK_RATE               100
#define MELODY_BPM_DEFAULT             200

//Difficulty constants
#define WALL_SPEED_INCREMENT_RATE      20  // Delay before speed increment in seconds
#define WALL_SPEED_INCREMENT_AMOUNT    1   // Amount wall speed increases by (walls/second)
#define DEFAULT_SPEED                  1   // Default starting wall speed

#define WALL_TASK_INDEX                4   //Index of the wall task object within tasks array

static uint8_t WALL_SPEED = DEFAULT_SPEED; // Default wall speed (walls/second)

// Update display
static void display_task(__unused__ void *data)
{
   tinygl_update();  //Update scrolling text
}


// Update walls - move existing wall or create new wall and increment score
static void wall_task(__unused__ void *data)
{
   if (get_game_state() & !get_pause_state())
   {
      if (get_active_wall().wall_type == OUT_OF_BOUNDS)
      {
         wall_create();
         increment_score();
      }
      else
      {
         move_wall();

         // Allows player to move after a potential collision
         toggle_stun(0);
      }
      check_collisions();
   }
}


// Poll input and update character position
static void character_task(__unused__ void *data)
{
   if (get_game_state() & !get_pause_state())
   {
      character_update();
   }
}


// Poll input to start the game
static void start_game_task(void *data)
{
   task_t *task = data;

   if (!get_game_state())
   {
      //Reset difficulty
      WALL_SPEED   = DEFAULT_SPEED;
      task->period = TASK_RATE / WALL_SPEED;
      game_state_update();
   }
	check_pause_button();
}


// Increase game difficulty by incrementing wall speed
static void difficulty_task(void *data)
{
   if (!get_pause_state())
   {
	   static uint8_t counter = 0;
	   task_t         *task   = data;

	   task->period = TASK_RATE / WALL_SPEED;
	   counter++;
	   if (counter >= WALL_SPEED_INCREMENT_RATE)
	   {
		  WALL_SPEED  += WALL_SPEED_INCREMENT_AMOUNT;
		  task->period = TASK_RATE / WALL_SPEED;
		  counter      = 0;
	   }
   }
}


static void tweeter_task(__unused__ void *data)
{
   speaker_update();
}


static void melody_task(__unused__ void *data)
{
   sound_update();
}


int main(void)
{
   // Module initialization
   system_init();
   display_init();
   tinygl_init(DISPLAY_UPDATE_RATE);
   game_init(MESSAGE_RATE);
   sound_init(TWEETER_TASK_RATE, MELODY_TASK_RATE, MELODY_BPM_DEFAULT);
   // Task definitions
   task_t tasks[] =
   {
      { .func = tweeter_task,    .period = TASK_RATE / TWEETER_TASK_RATE   },
      { .func = melody_task,     .period = TASK_RATE / MELODY_TASK_RATE    },
      { .func = display_task,    .period = TASK_RATE / DISPLAY_UPDATE_RATE },
      { .func = character_task,  .period = TASK_RATE / INPUT_UPDATE_RATE   },
      { .func = wall_task,       .period = TASK_RATE / WALL_SPEED          },
      { .func = difficulty_task, .period = TASK_RATE, .data = &(tasks[WALL_TASK_INDEX])},
      { .func = start_game_task, .period = TASK_RATE / INPUT_UPDATE_RATE, .data = &(tasks[WALL_TASK_INDEX])},
   };

   // Run tasks
   task_schedule(tasks, ARRAY_SIZE(tasks));
}
