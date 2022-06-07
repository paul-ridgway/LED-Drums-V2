#pragma once
#include <Array.h> // https://github.com/janelia-arduino/Array

class Trigger {
  public:
    //TODO: Store in array not vector
    Trigger(const Array<uint16_t, LEDS> *leds, const float r, const float g, const float b, const uint16_t decay) : 
    r_(r),
    g_(g),
    b_(b),
    leds_(leds),
    ledCount_(leds->size()),
    decay_(decay) {
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
    const uint16_t decay() const {
      return decay_;
    }
    const Array<uint16_t, LEDS>* leds() const {
      return leds_;
    }
    const uint16_t ledCount() const {
      return ledCount_;
    }
  private:
    const float r_, g_, b_;
    const Array<uint16_t, LEDS>* leds_;
    const uint16_t ledCount_;
    const uint16_t decay_;
};