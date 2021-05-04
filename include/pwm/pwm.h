/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2021 Bolder Flight Systems Inc
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#ifndef INCLUDE_PWM_PWM_H_
#define INCLUDE_PWM_PWM_H_

#include <span>
#include <array>
#include <variant>
#include <algorithm>
#include "core/core.h"
#include "effector/effector.h"
#include "polytools/polytools.h"

namespace bfs {

template<std::size_t N>
class PwmTx {
 public:
  bool Init(const EffectorConfig<N> &cfg) {
    /* Copy the configuration */
    config_ = cfg;
    /* Get the pins */
    if (std::holds_alternative<std::array<int8_t, N>>(config_.hw)) {
      pins_ = std::get<std::array<int8_t, N>>(config_.hw);
    } else {
      return false;
    }
    /* Set the resolution */
    analogWriteResolution(PWM_RESOLUTION_);
    /* Set the frequency */
    for (const auto &pin : pins_) {
      analogWriteFrequency(pin, PWM_FREQUENCY_HZ_);
    }
    return true;
  }
  void Cmd(std::span<float> cmds) {
    std::size_t len = std::min(cmds.size(), N);
    for (std::size_t i = 0; i < len; i++) {
      /* Saturation */
      if (cmds[i] > config_.effectors[i].max) {
        val_ = config_.effectors[i].max;
      } else if (cmds[i] < config_.effectors[i].min) {
        val_ = config_.effectors[i].min;
      } else {
        val_ = cmds[i];
      }
      /* Motor check */
      if ((config_.effectors[i].type == MOTOR) && (!motors_enabled_)) {
        val_ = config_.effectors[i].failsafe;
      }
      /* Servo check */
      if ((config_.effectors[i].type == SERVO) && (!servos_enabled_)) {
        val_ = config_.effectors[i].failsafe;
      }
      /* polyval */
      std::span<float> coef{config_.effectors[i].poly_coef,
        static_cast<std::size_t>(config_.effectors[i].num_coef)};
      ch_[config_.effectors[i].ch] = static_cast<uint16_t>(
                                     polyval<float>(coef, val_));
    }
  }
  void Write() {
    for (std::size_t i = 0; i < pins_.size(); i++) {
      analogWrite(pins_[i], static_cast<float>(ch_[i]) /
        PWM_PERIOD_US_ * MAX_PWM_VAL_);
    }
  }
  void EnableMotors() {motors_enabled_ = true;}
  void DisableMotors() {motors_enabled_ = false;}
  void EnableServos() {servos_enabled_ = true;}
  void DisableServos() {servos_enabled_ = false;}

 private:
  /* Configuration */
  EffectorConfig<N> config_;
  /* Pin numbers */
  std::array<int8_t, N> pins_;
  /* PWM resolution */
  static constexpr int PWM_RESOLUTION_ = 16;
  /* PWM bits */
  static constexpr float MAX_PWM_VAL_ =
    std::pow(2.0f, static_cast<float>(PWM_RESOLUTION_)) - 1.0f;
  /* PWM frequency */
  static constexpr float PWM_FREQUENCY_HZ_ = 50;
  /* PWM period */
  static constexpr float PWM_PERIOD_US_ = 1.0f / PWM_FREQUENCY_HZ_ *
                                          1000000.0f;
  /* TX data */
  bool motors_enabled_ = false, servos_enabled_ = false;
  float val_;
  std::array<uint16_t, N> ch_;
};

}  // namespace bfs

#endif  // INCLUDE_PWM_PWM_H_
