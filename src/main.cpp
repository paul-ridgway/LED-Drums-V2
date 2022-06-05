#include "Arduino.h"
#include "definitions.h"
#include "triggers.h"
#include "leds.h"

void setup()
{
  Serial2.begin(31250); // MIDI
  Serial.begin(115200); // Debug
  delay(1000);
  Serial.println("Ready!");

  introSetv2();
  Serial.println("setupTriggers");
  setupTriggers();

  Serial.println("setup done");
}

void handleMidi()
{
  while (Serial2.available() > 2)
  {
    auto val = Serial2.read();
    if (val == 0)
    {
      continue;
    }
    if (val & 0x80)
    {
      auto cmdStart = val & 0xF0;
      // auto channel = val & 0x0F;
      if (cmdStart == 0xF0)
      {
        // Non-musical command (ignore)
        continue;
      }
      if (cmdStart == 0x90)
      {
        auto note = Serial2.read();
        auto velocity = Serial2.read();

        Serial.print("NoteOn:  ");
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
      // else if (cmdStart == 0x80)
      // {
      //   Serial.print("NoteOff: ");
      //   Serial.print(Serial2.read());
      //   Serial.print(" - ");
      //   Serial.println(Serial2.read());
      // }
    }
  }
  Serial.flush();
}

void loop()
{
  const unsigned long now = millis();
  digitalWrite(13, ((now / 100) % 2) ? HIGH : LOW);

  allSet(0, 0, 0);
  for (unsigned int i = 0; i < activeTriggers.size(); ++i)
  {
    ActiveTrigger *activeTrigger = activeTriggers[i];
    const Trigger *t = activeTrigger->getTrigger();
    const int r = activeTrigger->r();
    const int g = activeTrigger->g();
    const int b = activeTrigger->b();
    const byte *leds = t->leds();
    for (byte i = 0; i < t->ledCount(); ++i)
    {
      const byte j = leds[i];
      color_r[j] = clamp((r + color_r[j]), 0, 255);
      color_g[j] = clamp((g + color_g[j]), 0, 255);
      color_b[j] = clamp((b + color_b[j]), 0, 255);
    }
    if (!activeTrigger->decay(millis()))
    {
      activeTriggers.remove(i);
      delete activeTrigger;
    }
    handleMidi(); // Midi doesnt like external delays, seems to drop data
  }
  const long idleTime = now - lastNoteAt;
  if (idleTime > IDLE_TIME)
  {
    idleSequence(now);
  }

  writeOut();

  const unsigned long start = millis();
  while (millis() - start < 10)
  {
    handleMidi();
  }
}
