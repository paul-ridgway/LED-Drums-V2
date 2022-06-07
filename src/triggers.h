#pragma once

#include "definitions.h"
#include "leds.h"
#include <Array.h>
#include "Trigger.h"
#include "ActiveTrigger.h"

int idleCount = 0;
long lastNoteAt = -IDLE_TIME;
long lastIdleAt = -IDLE_TIME;

Array<uint16_t, LEDS>* range(const uint16_t start, const uint16_t end, const uint16_t step) {
  Array<uint16_t, LEDS>* result = new Array<uint16_t, LEDS>();
  uint16_t value = start;
  while (value <= end) {
    result->push_back(value);
    value += step;
  }
  return result;
}

Array<Trigger*, 60>* triggers[60]; //Note is a byte (could be smarter)

Array<ActiveTrigger*, 60> activeTriggers;

void handleNote(const byte note, const float force) {
  Array<Trigger*, 60>* trigs = triggers[note];
  if (trigs) {
    Serial.print("handleNote, note: ");
    Serial.print(note);
    Serial.print(", force: ");
    Serial.println(force);
    Serial.println(trigs->size());
    
    for (Array<Trigger*, 60>::iterator it = trigs->begin(); it != trigs->end(); ++it) {
      Serial.println("new AT");
      activeTriggers.push_back(new ActiveTrigger(*it, force, millis()));
    }
  } else {
    Serial.print("Missing trigger for: ");
    Serial.println(note);
  }
}

void addTrigger(const byte note, const Array<uint16_t, LEDS> *leds, const float r, const float g, const float b, const uint16_t d) {
  Serial.print("Add trigger: ");
  Serial.println(note);
  
  if (!triggers[note]) {
    triggers[note] = new Array<Trigger*, 60>();
  }
  triggers[note]->push_back(new Trigger(leds, r, g, b, d));
  Serial.println(triggers[note]->size());
}

const int clamp(const int value, const int min, const int max) {
  if (value < min) {
    return min;
  } else if (value > max) {
    return max;
  }
  return value;
}


void setupTriggers()
{
  uint16_t snareLeds[] = {129, 132, 54, 57, 14, 97};
  Array<uint16_t, LEDS>* snare = new Array<uint16_t, LEDS>(snareLeds);

  // Configure mapping
  addTrigger(BASS, range(58, 128, 2), 0.1, 0.1, 0.1, SHORT);
  addTrigger(BASS, range(87, 109,  1), 0.0, 1.0, 1.0, MEDIUM);

  addTrigger(SNARE, snare, 1.0, 1.0, 1.0, SHORT);
  addTrigger(SNARE, range(1, 58, 2), 0.1, 0.0, 0.0, 0.4);
  addTrigger(SNARE, range(128, LEDS, 2), 0.1, 0.0, 0.0, 0.4);
  addTrigger(SNARE_RIM, snare, 1.0, 1.0, 0.0, SHORT);
  addTrigger(SNARE_RIM2, snare, 1.0, 0.0, 1.0, SHORT);
  addTrigger(SNARE_RIM, range(1, 58, 2), 0.1, 0.0, 0.0, 0.4);
  addTrigger(SNARE_RIM, range(128, LEDS, 2), 0.1, 0.0, 0.0, 0.4);
  addTrigger(SNARE_RIM2, range(1, 58, 2), 0.1, 0.0, 0.0, 0.4);
  addTrigger(SNARE_RIM2, range(128, LEDS, 2), 0.1, 0.0, 0.0, 0.4);

  Array<uint16_t, LEDS>* hi_hat_round = range(1, 58, 2);
  const Array<uint16_t, LEDS>* two = range(128, LEDS, 2);
  for (unsigned int i = 0; i < two->size(); ++i) {
    hi_hat_round->push_back(two->at(i));
  }
  addTrigger(HIHAT_RIM, range(132, 143, 1), 0.0, 1.0, 0.5, LONG);
  addTrigger(HIHAT_TOP, range(132, 143, 1), 0.0, 0.5, 1.0, LONG);
  addTrigger(HIHAT_PEDAL, range(132, 143, 1), 0.0, 1.0, 1.0, LONG);
  addTrigger(HIHAT_RIM_CLOSED, range(132, 143, 1), 1.0, 0.5, 0.75, LONG);
  addTrigger(HIHAT_TOP_CLOSED, range(132, 143, 1), 0.5, 1.0, 0.75, LONG);
  addTrigger(HIHAT_RIM, hi_hat_round, 0.0, 0.1, 0.0, LONG);
  addTrigger(HIHAT_TOP, hi_hat_round, 0.0, 0.1, 0.0, LONG);
  addTrigger(HIHAT_PEDAL, hi_hat_round, 0.0, 0.1, 0.0, LONG);
  addTrigger(HIHAT_RIM_CLOSED, hi_hat_round, 0.0, 0.1, 0.0, LONG);
  addTrigger(HIHAT_TOP_CLOSED, hi_hat_round, 0.0, 0.1, 0.0, LONG);
  addTrigger(HIHAT_RIM, range(115, 131, 2), 0.0, 1.0, 0.0, MEDIUM);
  addTrigger(HIHAT_TOP, range(115, 131, 2), 0.0, 1.0, 0.0, MEDIUM);
  addTrigger(HIHAT_PEDAL, range(115, 131, 2), 0.0, 1.0, 0.0, MEDIUM);
  addTrigger(HIHAT_RIM_CLOSED, range(115, 131, 2), 0.0, 1.0, 0.0, MEDIUM);
  addTrigger(HIHAT_TOP_CLOSED, range(115, 131, 2), 0.0, 1.0, 0.0, MEDIUM);

  addTrigger(CRASH_TOP, range(0, 3, 1), 0.5, 1.0, 0.0, LONG);
  addTrigger(CRASH_TOP, range(143, LEDS, 1), 0.5, 1.0, 0.0, LONG);
  addTrigger(CRASH_RIM, range(0, 3, 1), 1.0, 0.5, 0.0, LONG);
  addTrigger(CRASH_RIM, range(143, LEDS, 1), 1.0, 0.5, 0.0, LONG);
  addTrigger(CRASH_TOP, range(107, 122, 2) , 1.0, 0.0, 0.0, MEDIUM);
  addTrigger(CRASH_RIM, range(107, 122, 2) , 1.0, 0.0, 0.0, MEDIUM);


  addTrigger(TOM1, range(1, 15, 1), 0.0, 1.0, 0.0, LONG);
  addTrigger(TOM1_RIM, range(1, 15, 1), 1.0, 0.0, 1.0, LONG);
  addTrigger(TOM2, range(12, 27, 1), 0.0, 0.0, 1.0, LONG);
  addTrigger(TOM2_RIM, range(12, 27, 1), 1.0, 1.0, 0.0, LONG);

  addTrigger(RIDE_TOP, range(22, 35, 1), 0.0, 0.3, 1.0, LONG);
  addTrigger(RIDE_BELL, range(22, 35, 1), 1.0, 0.3, 0.0, LONG);
  addTrigger(RIDE_RIM, range(22, 35, 1), 0.3, 1.0, 0.0, LONG);

  addTrigger(RIDE_TOP, range(71, 91, 2), 0.0, 0.0, 1.0, MEDIUM);
  addTrigger(RIDE_BELL, range(71, 91, 2), 0.0, 0.0, 1.0, MEDIUM);
  addTrigger(RIDE_RIM, range(71, 91, 2), 0.0, 0.0, 1.0, MEDIUM);

  addTrigger(TOM3, range(27, 45, 1), 1.0, 0.0, 1.0, LONG);
  addTrigger(TOM3_RIM, range(27, 45, 1), 0.0, 1.0, 0.0, LONG);

  addTrigger(SPLASH_TOP, range(43, 46, 1), 1.0, 0.5, 0.0, LONG);
  addTrigger(SPLASH_RIM, range(43, 46, 1), 0.5, 1.0, 0.0, LONG);

  addTrigger(SPLASH_TOP, range(57, 73, 2), 0.0, 1.0, 0.0, MEDIUM);
  addTrigger(SPLASH_RIM, range(57, 73, 2), 0.0, 1.0, 0.0, MEDIUM);

  addTrigger(TOM4, range(45, 67, 1), 0.0, 1.0, 1.0, LONG);
  addTrigger(TOM4_RIM, range(45, 67, 1), 1.0, 0.0, 0.0, LONG);

}

void idleSequence(const unsigned long now) {
  if ((now - lastIdleAt) > 300) {
    lastIdleAt = now;
    if (idleCount == 0) {
      handleNote(BASS, 1.0f);
    } else if (idleCount == 1) {
      handleNote(SNARE, 1.0f);
    } else if (idleCount == 2) {
      handleNote(TOM1, 1.0f);
    } else if (idleCount == 3) {
      handleNote(TOM2, 1.0f);
    }
    ++idleCount;
    if (idleCount >= 6) {
      idleCount = 0;
    }
  }

}