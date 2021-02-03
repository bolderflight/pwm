/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2021 Bolder Flight Systems
*/

#ifndef INCLUDE_PWM_PWM_H_
#define INCLUDE_PWM_PWM_H_

#include <array>
#include <cmath>
#include "core/core.h"

namespace actuators {

template<int N>
class Pwm {
 public:
  explicit Pwm(std::array<int, N> pins) : pins_(pins) {}
  void Begin() {
    /* Set the resolution */
    analogWriteResolution(PWM_RESOLUTION_);
    /* Set the frequency */
    for (auto const & pin : pins_) {
      analogWriteFrequency(pin, pwm_frequency_hz_);
    }
  }
  void Write() {
    for (std::size_t i = 0; i < N; i++) {
      analogWrite(pins_[i], static_cast<float>(tx_channels_[i]) /
        pwm_period_us_ * MAX_PWM_VAL_);
    }
  }
  void frequency_hz(float val) {
    pwm_frequency_hz_ = val;
    pwm_period_us_ = 1.0f / pwm_frequency_hz_ * 1000000.0f;
    for (auto const & pin : pins_) {
      analogWriteFrequency(pin, pwm_frequency_hz_);
    }
  }
  float frequency_hz() {return pwm_frequency_hz_;}
  std::array<uint16_t, N> tx_channels() {return tx_channels_;}
  void tx_channels(const std::array<uint16_t, N> &val) {tx_channels_ = val;}

 private:
  /* Pin numbers */
  std::array<int, N> pins_;
  /* PWM resolution */
  static constexpr int PWM_RESOLUTION_ = 16;
  /* PWM bits */
  static constexpr float MAX_PWM_VAL_ =
    std::pow(2.0f, static_cast<float>(PWM_RESOLUTION_)) - 1.0f;
  /* PWM frequency */
  float pwm_frequency_hz_ = 50;
  /* PWM period */
  float pwm_period_us_ = 1.0f / pwm_frequency_hz_ * 1000000.0f;
  /* TX data */
  std::array<uint16_t, N> tx_channels_;
};

}  // namespace actuators

#endif  // INCLUDE_PWM_PWM_H_
