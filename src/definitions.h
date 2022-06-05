#pragma once

#define IDLE_TIME 20000

// Drum note mapping
#define HIHAT_TOP 46
#define HIHAT_TOP_CLOSED 42
#define HIHAT_RIM 26
#define HIHAT_RIM_CLOSED 22
#define HIHAT_PEDAL 44
#define CRASH_TOP 49
#define CRASH_RIM 55
#define RIDE_TOP 51
#define RIDE_RIM 59
#define RIDE_BELL 53
#define SPLASH_TOP 57
#define SPLASH_RIM 52
#define BASS 36
#define SNARE 38
#define SNARE_RIM 37
#define SNARE_RIM2 40
#define TOM1 48
#define TOM1_RIM 50
#define TOM2 45
#define TOM2_RIM 47
#define TOM3 43
#define TOM3_RIM 58
#define TOM4 41
#define TOM4_RIM 39

// Durations
#define SHORT 200
#define MEDIUM 300
#define LONG 400

// Midi state tracking
#define STATE_NONE 0
#define STATE_NOTE_ON 1
#define STATE_NOTE 2
#define MIDI_NOTE_ON 144
#define MIDI_NOTE_OFF 128
