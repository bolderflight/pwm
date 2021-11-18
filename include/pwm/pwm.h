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
#include "core/core.h"

namespace bfs {

template<std::size_t N>
class PwmTx {
 public:
  void Init(const std::array<int8_t, N> &pins) {
    pins_ = pins;
    /* Set the resolution */
    analogWriteResolution(PWM_RESOLUTION_);
    /* Set the frequency */
    for (const auto &pin : pins_) {
      analogWriteFrequency(pin, PWM_FREQUENCY_HZ_);
    }
  }
  void Write() {
    for (std::size_t i = 0; i < pins_.size(); i++) {
      analogWrite(pins_[i], static_cast<float>(ch_[i]) /
                  PWM_PERIOD_US_ * MAX_PWM_VAL_);
    }
  }
  static constexpr int8_t NUM_CH = N;
  inline void ch(const std::array<int16_t, N> &ref) {ch_ = ref;}
  inline std::array<int16_t, N> ch() const {return ch_;}

 private:
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
  std::array<int16_t, N> ch_;
};

}  // namespace bfs

#endif  // INCLUDE_PWM_PWM_H_
