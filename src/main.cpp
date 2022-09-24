#include "Arduino.h"
#include <MIDI.h>
#include "definitions.h"
#include "triggers.h"
#include "leds.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

TaskHandle_t renderTask;

void renderLoop(void *parameter)
{
  Serial.print("renderLoop running on core ");
  Serial.println(xPortGetCoreID());

  allSet(0, 0, 0);
  writeOut();

  while (true)
  {
    const unsigned long now = millis();
    allSet(0, 0, 0);

    // For all active triggers
    for (unsigned int i = 0; i < activeTriggers.size(); ++i)
    {
      ActiveTrigger *activeTrigger = activeTriggers[i];
      const Trigger *t = activeTrigger->getTrigger();
      const int r = activeTrigger->r();
      const int g = activeTrigger->g();
      const int b = activeTrigger->b();
      // For each LED in the trigger
      const Array<uint16_t, LEDS> *leds = t->leds();
      for (uint16_t i = 0; i < t->ledCount(); ++i)
      {
        const uint16_t j = leds->at(i);
        color_r[j] = clamp((r + color_r[j]), 0, 255);
        color_g[j] = clamp((g + color_g[j]), 0, 255);
        color_b[j] = clamp((b + color_b[j]), 0, 255);
      }
      if (!activeTrigger->decay(millis()))
      {
        activeTriggers.remove(i);
        delete activeTrigger;
      }
    }

    const long idleTime = now - lastNoteAt;
    if (idleTime > IDLE_TIME)
    {
      idleSequence(now);
    }

    writeOut();
  }
}

void handleMidi()
{
  if (MIDI.read()) // Is there a MIDI message incoming ?
  {
    switch (MIDI.getType()) // Get the type of the message we caught
    {
    case midi::MidiType::NoteOn:
      auto note = MIDI.getData1();
      auto velocity = MIDI.getData2();
      Serial.print("NoteOn: ");
      Serial.print(note);
      Serial.print(" - ");
      Serial.println(velocity);

      lastNoteAt = millis();
      {
        const float force = ((float)velocity) / 127.0f; // Max velocity from drums
        handleNote(note, force);
      }

      break;
    }
  }
}

void setup()
{
  MIDI.begin(10);
  Serial.begin(115200); // Debug
  delay(1000);
  Serial.println("Ready!");

  introSetv2();
  Serial.println("setupTriggers");
  setupTriggers();

  Serial.println("setup tasks");
  xTaskCreatePinnedToCore(
      renderLoop,   /* Function to implement the task */
      "RenderTask", /* Name of the task */
      10000,        /* Stack size in words */
      NULL,         /* Task input parameter */
      0,            /* Priority of the task */
      &renderTask,  /* Task handle. */
      0);           /* Core where the task should run */
}

void loop()
{
  Serial.print("midiLoop running on core ");
  Serial.println(xPortGetCoreID());

  while (true)
  {
    handleMidi();
  }
}
