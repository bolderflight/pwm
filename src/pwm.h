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

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include "core/core.h"
#endif
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <array>

namespace bfs {

template<int8_t N, int8_t RES = 16>
class PwmTx {
 private:
  /* Pin numbers */
  std::array<int8_t, N> pins_;
  /* PWM resolution */
  static constexpr int8_t RES_ = RES;
  /* PWM bits */
  static constexpr float MAX_VAL_ =
    std::pow(2.0f, static_cast<float>(RES_)) - 1.0f;
  /* PWM frequency */
  float pwm_freq_hz_ = 50;
  /* PWM period */
  float pwm_period_us_;
  /* TX data */
  std::array<int16_t, N> ch_;

 public:
  explicit PwmTx(const std::array<int8_t, N> &pins) : pins_(pins) {}
  void Begin() {
    Begin(pwm_freq_hz_);
  }
  void Begin(const float freq) {
    pwm_period_us_ = 1.0f / freq * 1000000.0f;
    /* Set the period and frequency */
    for (std::size_t i = 0; i < N; i++) {
      analogWriteFrequency(pins_[i], freq);
    }
  }
  void Write() {
    for (std::size_t i = 0; i < N; i++) {
      analogWrite(pins_[i], static_cast<float>(ch_[i]) /
                  pwm_period_us_ * MAX_VAL_);
    }
  }
  static constexpr int8_t NUM_CH() {return N;}
  inline void ch(const std::array<int16_t, N> &cmds) {ch_ = cmds;}
  inline std::array<int16_t, N> ch() const {return ch_;}
};

}  // namespace bfs

#endif  // SRC_PWM_H_
