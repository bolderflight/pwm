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

#include <array>
#include <cmath>
#include "core/core.h"

namespace bfs {

template<int N>
class PwmTx {
 public:
  explicit PwmTx(std::array<int, N> pins) : pins_(pins) {}
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
  void frequency_hz(const float val) {
    pwm_frequency_hz_ = val;
    pwm_period_us_ = 1.0f / pwm_frequency_hz_ * 1000000.0f;
    for (auto const & pin : pins_) {
      analogWriteFrequency(pin, pwm_frequency_hz_);
    }
  }
  inline float frequency_hz() const {return pwm_frequency_hz_;}
  inline std::array<uint16_t, N> tx_channels() const {return tx_channels_;}
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

}  // namespace bfs

#endif  // INCLUDE_PWM_PWM_H_
