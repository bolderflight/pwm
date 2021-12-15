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

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <algorithm>
#include <cmath>
#include "core/core.h"
#endif

namespace bfs {

template<size_t N, int RESOLUTION>
class PwmTx {
 private:
  /* Pin numbers */
  int8_t pins_[N];
  /* PWM resolution */
  static constexpr int PWM_RESOLUTION_ = RESOLUTION;
  /* PWM bits */
  #if defined(ARDUINO)
  static constexpr float MAX_PWM_VAL_ =
    pow(2.0f, static_cast<float>(PWM_RESOLUTION_)) - 1.0f;
  #else
  static constexpr float MAX_PWM_VAL_ =
    std::pow(2.0f, static_cast<float>(PWM_RESOLUTION_)) - 1.0f;
  #endif
  /* PWM frequency */
  static constexpr float PWM_FREQUENCY_HZ_ = 50;
  /* PWM period */
  static constexpr float PWM_PERIOD_US_ = 1.0f / PWM_FREQUENCY_HZ_ *
                                          1000000.0f;
  /* Number of channels */
  static constexpr int8_t NUM_CH_ = N;
  /* TX data */
  int16_t ch_[N];

 public:
  explicit PwmTx(int8_t pins[N]) {
    memcpy(pins_, pins, N);
  }
  void Begin() {
    /* Set the resolution */
    analogWriteResolution(PWM_RESOLUTION_);
    /* Set the frequency */
    for (size_t i = 0; i < N; i++) {
      analogWriteFrequency(pins_[i], PWM_FREQUENCY_HZ_);
    }
  }
  void Write() {
    for (size_t i = 0; i < N; i++) {
      analogWrite(pins_[i], static_cast<float>(ch_[i]) /
                  PWM_PERIOD_US_ * MAX_PWM_VAL_);
    }
  }
  static constexpr int8_t NUM_CH() {return NUM_CH_;}
  bool ch(const int8_t idx, const int16_t val) {
    if ((idx < 0) || (idx >= NUM_CH_)) {return false;}
    ch_[idx] = val;
    return true;
  }
  int8_t ch(int16_t const * const data, const int8_t len) {
    if (!data) {return -1;}
    #if defined(ARDUINO)
    int8_t cpy_len = min(NUM_CH_, len);
    #else
    int8_t cpy_len = std::min(NUM_CH_, len);
    #endif
    memcpy(ch_, data, cpy_len * sizeof(int16_t));
    return cpy_len;
  }
};

}  // namespace bfs

#endif  // INCLUDE_PWM_PWM_H_
