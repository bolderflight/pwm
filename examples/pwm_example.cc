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

#include "pwm/pwm.h"

/* PWM object */
std::array<int8_t, 6> pins = {21, 22, 23, 2, 3, 4};
bfs::PwmTx<pins.size()> pwm;

int main() {
  /* Serial to display data */
  Serial.begin(115200);
  while(!Serial) {}
  /* Config */
  bfs::EffectorConfig<pins.size()> config = {
    .hw = pins,
    .effectors = {
      {
        .type = bfs::SERVO,
        .ch = 1,
        .min = -20,
        .max = 20,
        .failsafe = 0,
        .num_coef = 2,
        .poly_coef = {500, 1500}
      }
    }
  };
  if (!pwm.Init(config)) {
    Serial.println("Unable to init PWM output");
    while (1) {}
  }
  /* Enable motors and servos */
  pwm.EnableMotors();
  pwm.EnableServos();
  /* Issue a command */
  std::array<float, 1> cmd = {0.5};
  pwm.Cmd(cmd);
  pwm.Write();
  while (1) {}
}
