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
#include <algorithm>
#include <cmath>

namespace bfs {

template<size_t N>
class PwmTx {
 private:
  /* Pin numbers */
  int8_t pins_[N];
  /* PWM resolution */
  int8_t pwm_resolution_ = 16;
  /* PWM bits */
  float max_pwm_val_;
  /* PWM frequency */
  float default_pwm_freq_hz_ = 50;
  float pwm_freq_hz_[N];
  /* PWM period */
  float pwm_period_us_[N];
  /* Number of channels */
  static constexpr int8_t NUM_CH_ = N;
  /* TX data */
  int16_t ch_[N];

 public:
  explicit PwmTx(const int8_t (&pins)[N]) {
    memcpy(pins_, pins, N);
    for (size_t i = 0; i < N; i++) {
      pwm_freq_hz_[i] = default_pwm_freq_hz_;
    }
  }
  void Begin() {
    Begin(pwm_freq_hz_, pwm_resolution_);
  }
  void Begin(const int8_t res) {
    Begin(pwm_freq_hz_, res);
  }
  void Begin(const float freq[N]) {
    Begin(freq, pwm_resolution_);
  }
  void Begin(const float (&freq)[N], const int8_t res) {
    /* Set the resolution */
    pwm_resolution_ = res;
    analogWriteResolution(res);
    /* Max PWM value */
    max_pwm_val_ = std::pow(2.0f, static_cast<float>(pwm_resolution_)) - 1.0f;
    /* Set the period and frequency */
    for (size_t i = 0; i < N; i++) {
      pwm_period_us_[i] = 1.0f / freq[i] * 1000000.0f;
      analogWriteFrequency(pins_[i], freq[N]);
    }
  }
  void Write() {
    for (size_t i = 0; i < N; i++) {
      analogWrite(pins_[i], static_cast<float>(ch_[i]) /
                  pwm_period_us_ * max_pwm_val_);
    }
  }
  static constexpr int8_t NUM_CH() {return NUM_CH_;}
  void ch(const int16_t (&data)[N]) {
    memcpy(ch_, data, N * sizeof(int16_t));
  }
};

}  // namespace bfs

#endif  // SRC_PWM_H_
