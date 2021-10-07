/** @file   wall.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */
#include "wall.h"
#include "display.h"
#include "character.h"
#include <stdlib.h>

/* Array containing all walls */
/* Walls are added by replacing oldest wall */

static Wall_t COL_WALL_ARRAY[MAX_COL_WALLS]; // Initalise array of walls
static uint8_t OLDEST_WALL_INDEX = 0; // Poionts to oldest wall
static uint8_t RANDOM_SEED = 0; // The starting seed for rand()

/* Creates adds a wall and replaces with oldest one */

Wall_t decide_wall_type(uint8_t rand_num1, uint8_t rand_num2)
{
	bool UP_WALL = (rand_num1) % 2;
	uint8_t wall_size = (rand_num1) % 3;
	uint8_t rand_shift = rand_num2 % (7-wall_size);
	uint8_t rand_bitmap;
	
	if (wall_size == 0) {
		rand_bitmap = ONE_HOLE(rand_shift);
	} else if (wall_size == 1) {
		rand_bitmap = TWO_HOLE(rand_shift);
	} else {
		rand_bitmap = THREE_HOLE(rand_shift);
	};
	Wall_t wall_n;
	if (UP_WALL) {
		wall_n = (Wall_t) UP_COL_WALL(rand_bitmap);
	} else {
		wall_n = (Wall_t) DOWN_COL_WALL(rand_bitmap);
	};
	return wall_n;
}

void wall_create(void)
{
    // Randomly select wall-type
	srand(RANDOM_SEED);
	uint8_t RAND_NUM1 = rand();
	uint8_t RAND_NUM2 = rand();
	
	Wall_t wall = decide_wall_type(RAND_NUM1, RAND_NUM2);
	
	COL_WALL_ARRAY[OLDEST_WALL_INDEX] = wall;
	toggle_wall(wall, 1);
	OLDEST_WALL_INDEX = (OLDEST_WALL_INDEX + 1) % MAX_COL_WALLS;
	RANDOM_SEED++;
}


/*  Toggles display state of a wall
    @param wall, 
    @param display_on, if 0 display is removed 
*/
void toggle_wall(Wall_t col_wall, bool display_on)
{
	uint8_t wall_pos = col_wall.pos;
	uint8_t pattern = (display_on) ? col_wall.bit_data: 0;
	uint8_t col_index;
	for (col_index=0; col_index <= DOWN_BOUNDARY; col_index++) {
		bool state = ((BIT(col_index) & pattern) != 0) ? 1: 0;
		display_pixel_set(wall_pos, col_index, state);
	}
}


/*  Helper function to move_walls
    @param wall, a pointer to wall 
    @brief, moves wall, sets walls=None if out_of_bounds 
*/
void move_wall(Wall_t* wall, position_t character_pos)
{
	uint8_t new_pos = wall->pos + wall->dir; // Calculates new position based on direction
	toggle_wall(*wall, 0);
	if (new_pos > RIGHT_BOUNDARY) {
		wall->dir = OUT_OF_BOUNDS;
		wall->bit_data = 0;
	} else {
		wall->pos = new_pos;
		toggle_wall(*wall, 1);
	};
	display_pixel_set(character_pos.x, character_pos.y, 1);
}

/* Increments the positions of all column walls by one*/

void move_col_walls(position_t character_pos)
{
	uint8_t wall_index;
	for (wall_index=0; wall_index<MAX_COL_WALLS; wall_index++) { // Cycles through STATIC wall array
		move_wall((COL_WALL_ARRAY + wall_index), character_pos); // Gives the pointer to given wall
	}
}



