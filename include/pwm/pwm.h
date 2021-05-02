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
#include <vector>
#include <cmath>
#include <algorithm>
#include "core/core.h"

namespace bfs {

class PwmTx {
 public:
  void Begin(std::span<int> pins) {
    /* Assign the pins */
    pins_.resize(pins.size());
    for (std::size_t i = 0; i < pins.size(); i++) {
      pins_[i] = pins[i];
    }
    /* Set the tx_channels size */
    tx_channels_.resize(pins_.size());
    /* Set the resolution */
    analogWriteResolution(PWM_RESOLUTION_);
    /* Set the frequency */
    for (const auto & pin : pins_) {
      analogWriteFrequency(pin, pwm_frequency_hz_);
    }
  }
  void Write() {
    for (std::size_t i = 0; i < pins_.size(); i++) {
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
  inline std::vector<uint16_t> tx_channels() const {return tx_channels_;}
  void tx_channels(std::span<uint16_t> val) {
    std::size_t len = std::min(tx_channels_.size(), val.size());
    for (std::size_t i = 0; i < len; i++) {
      tx_channels_[i] = val[i];
    }
  }

 private:
  /* Pin numbers */
  std::vector<int> pins_;
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
  std::vector<uint16_t> tx_channels_;
};

}  // namespace bfs

#endif  // INCLUDE_PWM_PWM_H_
