/** @file   character.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   3 Oct 2021
 *  @brief  Character profile
 */

#include "character.h"
#include "display.h"
#include "navswitch.h"

// Character properties
static CharacterInfoStruct character_info;


/* Initialisation for character module
 * @param life_count: creates player with respective number of lives
 * @brief: Character creation, spawns character at default coordinates
 */
void character_init(uint8_t life_count)
{
	character_info = (CharacterInfoStruct){
		.x = DEFAULT_X, .y = DEFAULT_Y, .lives = life_count
	};

	display_pixel_set(character_info.x, character_info.y, true);

	if (get_stun_condition())     //Prevent character being stunned on respawn
	{
		toggle_stun(false);
	}
}


/*  Return current character information
 *  @return CharacterInfoStruct with coordinates and number of lives
 */
CharacterInfoStruct get_character_info(void)
{
	return character_info;
}


/*  Returns the stun state of the player
 *  @return 1 if stunned else 0
 */
bool get_stun_condition()
{
	return character_info.is_stunned;
}


/*  Changes the stun state of the player
 *  @param stun_set: bool variable to which character_stun is set
 */
void toggle_stun(bool stun_set)
{
	character_info.is_stunned = stun_set;
}


/* Reduces character lives by 1
 *  @return: returns 1 if no lives left else 0
 */
bool decrease_character_lives()
{
	character_info.lives--;
	return(character_info.lives == 0);
}


/* Turns off character
 */
void character_disable()
{
	display_pixel_set(character_info.x, character_info.y, false);
}


/*  Move character STEP_SIZE unit west
 *  @return true if character moving off boundary else false*/
bool move_west()
{
	if ((WEST_CHARACTER_BOUNDARY < character_info.x) && !display_pixel_get(character_info.x - STEP_SIZE, character_info.y))
	{
		display_pixel_set(character_info.x, character_info.y, false);

		character_info.x -= STEP_SIZE;

		if (!display_pixel_get(character_info.x, character_info.y))         // Don't move character if destination is on
		{
			display_pixel_set(character_info.x, character_info.y, true);
		}

		return false;
	}

	return true;
}


/*  Move character STEP_SIZE unit east
 *  @return true if character moving off boundary else false*/
bool move_east()
{
	if ((EAST_CHARACTER_BOUNDARY > character_info.x) && !display_pixel_get(character_info.x + STEP_SIZE, character_info.y))
	{
		display_pixel_set(character_info.x, character_info.y, false);

		character_info.x += STEP_SIZE;

		if (!display_pixel_get(character_info.x, character_info.y))         // Don't move character if destination is on
		{
			display_pixel_set(character_info.x, character_info.y, true);
		}

		return false;
	}

	return true;
}


/*  Move character STEP_SIZE unit north
 *  @return true if character moving off boundary else false*/
bool move_north()
{
	if ((NORTH_CHARACTER_BOUNDARY < character_info.y) && !display_pixel_get(character_info.x, character_info.y - STEP_SIZE))
	{
		display_pixel_set(character_info.x, character_info.y, false);

		character_info.y -= STEP_SIZE;

		if (!display_pixel_get(character_info.x, character_info.y))         // Don't move character if destination is on
		{
			display_pixel_set(character_info.x, character_info.y, true);
		}

		return false;
	}

	return true;
}


/*  Move character STEP_SIZE unit south
 *  @return true if character moving off boundary else false*/
bool move_south()
{
	if ((SOUTH_CHARACTER_BOUNDARY > character_info.y) && !display_pixel_get(character_info.x, character_info.y + STEP_SIZE))
	{
		display_pixel_set(character_info.x, character_info.y, false);

		character_info.y += STEP_SIZE;

		if (!display_pixel_get(character_info.x, character_info.y))         // Don't move character if destination is on
		{
			display_pixel_set(character_info.x, character_info.y, true);
		}

		return false;
	}

	return true;
}


/* Poll navswitch input and move character
 *  @brief: Doesn't allow movement is player is stunned
 */
void character_update()
{
	navswitch_update();     // Update navswitch input

	//Restores character state if passed by wall
	if (!display_pixel_get(character_info.x, character_info.y))
	{
		display_pixel_set(character_info.x, character_info.y, true);
	}

	// Move character in direction of navswitch input
	// Doesn't allow movement if character is stunned
	if (!character_info.is_stunned)
	{
		if (navswitch_push_event_p(NAVSWITCH_NORTH))
		{
			move_north();
		}
		else if (navswitch_push_event_p(NAVSWITCH_SOUTH))
		{
			move_south();
		}
		else if (navswitch_push_event_p(NAVSWITCH_EAST))
		{
			move_east();
		}
		else if (navswitch_push_event_p(NAVSWITCH_WEST))
		{
			move_west();
		}
	}
}
