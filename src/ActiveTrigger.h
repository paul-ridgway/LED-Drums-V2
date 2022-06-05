#pragma once

#define MIN 1.0f / 256.0f

class ActiveTrigger {

  public:

    ActiveTrigger(const Trigger* trigger, const float force, const unsigned long start) :
      trigger_(trigger) {
      r_ = trigger->r() * force * force;
      g_ = trigger->g() * force * force;
      b_ = trigger->b() * force * force;
      dec_ = 1.0f;
      decay_ = (int)(((float)trigger->decay()) * force);
      if (decay_ < 100) {
        decay_ = 100;
      }
      target_= start + decay_;
    }

    const Trigger* getTrigger() const {
      return trigger_;
    }

    bool decay(unsigned long time) {
      const int delta = target_ - time;
      dec_ = ((float)delta) / ((float)decay_);
      //dec_ *= dec_;
      if (dec_ > 1.0f) {
        dec_ = 1.0f;
      }
      if (dec_ < 0) {
        dec_ = 0;
      }
      return delta > 0;
    }

    const byte r() const {
      return calc(r_);
    }
    const byte g() const {
      return calc(g_);
    }
    const byte b() const {
      return calc(b_);
    }
  private:
    const Trigger* trigger_;
    float r_, g_, b_, dec_;
    unsigned long target_;
    unsigned int decay_;

    const byte calc(const float v) const {
      const float ret = (v * dec_ * dec_) * 255.0f;
      if (ret > 255.0f) {
        return 255;
      }
      if (ret < 0.0f) {
        return 0;
      }
      return (byte) ret;
    }
};

