#ifndef SOUND_H
#define SOUND_H

#include "system.h"
#include "../../extra/tweeter.h"

// Activate speaker
void toggle_speaker(bool state);

// Check required speaker state and toggle
void speaker_update(void);

// Update current sound
void sound_update(void);

// Stop playing current sound
void sound_stop(void);

//Play song
void sound_play(char song[]);

// Single beep used for testing
void beep(void);

// Changes the BPM of a song
void BPM_change(uint16_t tune_bpm_rate);

// Initialize sound drivers
void sound_init(uint16_t tweeter_task_rate, uint16_t tune_task_rate, uint16_t tune_bpm_rate);
#endif
