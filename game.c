/** @file   game.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   15 Oct 2021
 *  @brief  Main task control
 */

#include "system.h"
#include "tinygl.h"
#include "task.h"
#include "character.h"
#include "wall.h"
#include "game_manager.h"
#include "sound.h"

//Frequency of task execution in hz
#define DISPLAY_UPDATE_RATE            300
#define INPUT_UPDATE_RATE              20
#define TWEETER_TASK_RATE              TWEETER_SWITCH_RATE
#define MELODY_TASK_RATE               100
#define MESSAGE_RATE                   20  // Tinygl text scroll speed

//Difficulty constants
#define WALL_SPEED_INCREMENT_RATE      20  // Delay before speed increment in seconds
#define WALL_SPEED_INCREMENT_AMOUNT    1   // Amount wall speed increases by (walls/second)
#define DEFAULT_SPEED                  1   // Default starting wall speed

#define WALL_TASK_INDEX                4   //Index of the wall task object within tasks array

static uint8_t wall_speed = DEFAULT_SPEED; // Default wall speed (walls/second)


/* Update LED Matrix display
 *  @param unused void pointer passed by task scheduler */
static void display_task(__unused__ void *data)
{
	tinygl_update(); //Update scrolling text
}


/*  Wall update task moves existing wall or
 *  creates new wall and increments score
 *  @param unused void pointer passed by task scheduler */
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
			toggle_stun(0); // Reset stun condition when wall moves over character
		}

		check_collisions();
	}
}


/*  Character movement input polling
 *  @param unused void pointer passed by task scheduler */
static void character_task(__unused__ void *data)
{
	if (get_game_state() & !get_pause_state())
	{
		character_update();
	}
}


/*  Start/Resume game and reset difficulty on restart
 *  @param task_t pointer of task to reset period*/
static void start_game_task(void *data)
{
	task_t *task = data;

	if (!get_game_state())
	{
		// Reset difficulty
		wall_speed   = DEFAULT_SPEED;
		task->period = TASK_RATE / wall_speed;
		game_state_update();
	}

	check_pause_button();
}


/*  Wall update task moves existing wall or
 *  creates new wall and increments score
 *  @param task_t pointer of task to increase rate*/
static void difficulty_task(void *data)
{
	if (!get_pause_state())
	{
		static uint8_t counter = 0; // Tracks number of function calls
		task_t         *task   = data;

		task->period = TASK_RATE / wall_speed;
		counter++;

        // Increases speed after WALL_SPEED_INCREMENT_RATE times the task frequency
		if (counter >= WALL_SPEED_INCREMENT_RATE)
		{
			wall_speed  += WALL_SPEED_INCREMENT_AMOUNT;
			task->period = TASK_RATE / wall_speed;
			counter      = 0;
		}
	}
}


/*  Tweeter update task to make speaker sound
 *  @param unused void pointer passed by task scheduler */
static void tweeter_task(__unused__ void *data)
{
	speaker_update();
}


/*  Melody update task to advance current melody
 *  @param unused void pointer passed by task scheduler */
static void melody_task(__unused__ void *data)
{
	sound_update();
}


int main(void)
{
	// Module initialization
	system_init();
	tinygl_init(DISPLAY_UPDATE_RATE);
	game_init(MESSAGE_RATE);
	sound_init(MELODY_TASK_RATE);

	// Task definitions
	task_t tasks[] =
	{
		{ .func = tweeter_task,    .period = TASK_RATE / TWEETER_TASK_RATE   },
		{ .func = melody_task,     .period = TASK_RATE / MELODY_TASK_RATE    },
		{ .func = display_task,    .period = TASK_RATE / DISPLAY_UPDATE_RATE },
		{ .func = character_task,  .period = TASK_RATE / INPUT_UPDATE_RATE   },
		{ .func = wall_task,       .period = TASK_RATE / wall_speed          },
		{ .func = difficulty_task, .period = TASK_RATE, .data = &(tasks[WALL_TASK_INDEX])},
		{ .func = start_game_task, .period = TASK_RATE / INPUT_UPDATE_RATE, .data = &(tasks[WALL_TASK_INDEX])},
	};

	// Run tasks
	task_schedule(tasks, ARRAY_SIZE(tasks));

	return 0;
}
