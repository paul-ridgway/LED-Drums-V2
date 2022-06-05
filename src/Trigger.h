#pragma once
#include <Array.h> // https://github.com/janelia-arduino/Array

class Trigger {
  public:
    //TODO: Store in array not vector
    Trigger(const byte* leds, const float r, const float g, const float b, const int decay) : 
    r_(r),
    g_(g),
    b_(b),
    decay_(decay) {
      ledCount_ = sizeof(leds) / sizeof(leds[0]);
      leds_ = (byte*)malloc(ledCount_);
      for(unsigned int i = 0; i < ledCount_; ++i) {
        leds_[i] = leds[0];
      }
    }

    const float r() const {
      return r_;
    }
    const float g() const {
      return g_;
    }
    const float b() const {
      return b_;
    }
    const int decay() const {
      return decay_;
    }
    const byte* leds() const {
      return leds_;
    }
    const int ledCount() const {
      return ledCount_;
    }
  private:
    const float r_, g_, b_;
    const int decay_;
    byte* leds_;
    int ledCount_;
};