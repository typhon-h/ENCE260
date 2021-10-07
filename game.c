#include "system.h"
#include "pacer.h"
#include "display.h"
#include "character.h"
#include "wall.h"


//Frequency of task execution in hz
#define DISPLAY_UPDATE_RATE      275
#define CHARACTER_UPDATE_RATE    20
#define WALL_MOVEMENT_RATE       1    // ****Liable to change****
#define WALL_CREATION_RATE       0.25 // ****Liable to change****
#define PACER_RATE               500  // Total ticks in a second


int main(void)
{
   // Module initialization
   system_init();
   display_init();
   character_init();
   pacer_init(PACER_RATE);

   // Task delay times
   uint16_t character_time     = PACER_RATE / CHARACTER_UPDATE_RATE;
   uint16_t display_time       = PACER_RATE / DISPLAY_UPDATE_RATE;
   uint16_t wall_movement_time = PACER_RATE / WALL_MOVEMENT_RATE;   //Wall speed
   uint16_t wall_creation_time = PACER_RATE / WALL_CREATION_RATE;   //Wall speed
   uint16_t ticks = 0;

   while (1)
   {
      pacer_wait();

      if ((ticks % display_time) == 0) //Display refresh
      {
         display_update();
      }

      if ((ticks % wall_movement_time) == 0) //Move wall
      {
         move_col_walls();
      }

      if ((ticks % wall_creation_time) == 0) //Generate new wall
      {
         wall_create();
      }

      if (ticks % character_time == 0)  //Character input polling
      {
         character_update();
      }

      ticks++;
   }
}
