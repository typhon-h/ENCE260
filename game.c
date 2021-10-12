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

//Surpress warnings for unused function parameters
#define UNUSED(PARAM)    (void)(PARAM)

//Frequency of task execution in hz
#define DISPLAY_UPDATE_RATE            300
#define INPUT_UPDATE_RATE              20
#define MESSAGE_RATE                   20  // Tinygl text scroll speed

//Difficulty constants
#define WALL_SPEED_INCREMENT_RATE      20  // Delay before speed increment in seconds
#define WALL_SPEED_INCREMENT_AMOUNT    1   // Amount wall speed increases by (walls/second)
#define DEFAULT_SPEED                  1   // Default starting wall speed

#define WALL_TASK_INDEX                2   //Index of the wall task object within tasks array

static uint8_t WALL_SPEED = DEFAULT_SPEED; // Default wall speed (walls/second)

// Update display
static void display_task(void *data)
{
   UNUSED(data);
   tinygl_update();  //Update scrolling text
   display_update();
}


// Update walls - move existing wall or create new wall and increment score
static void wall_task(void *data)
{
   UNUSED(data);

   if (get_game_state())
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
static void character_task(void *data)
{
   UNUSED(data);

   if (get_game_state())
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
}


// Increase game difficulty by incrementing wall speed
static void difficulty_task(void *data)
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


int main(void)
{
   // Module initialization
   system_init();
   display_init();
   tinygl_init(DISPLAY_UPDATE_RATE);
   game_init(MESSAGE_RATE);

   // Task definitions
   task_t tasks[] =
   {
      { .func = display_task,    .period = TASK_RATE / DISPLAY_UPDATE_RATE },
      { .func = character_task,  .period = TASK_RATE / INPUT_UPDATE_RATE   },
      { .func = wall_task,       .period = TASK_RATE / WALL_SPEED          },
      { .func = difficulty_task, .period = TASK_RATE, .data = &(tasks[WALL_TASK_INDEX])},
      { .func = start_game_task, .period = TASK_RATE / INPUT_UPDATE_RATE, .data = &(tasks[WALL_TASK_INDEX])},
   };

   // Run tasks
   task_schedule(tasks, 5);
}
