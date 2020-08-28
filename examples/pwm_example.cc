/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2020 Bolder Flight Systems
*/

#include "pwm/pwm.h"

actuators::Pwm<8> pwm({21, 22, 23, 2, 3, 4, 5, 6});

int main() {
  Serial.begin(115200);
  pwm.Begin();
  std::array<uint16_t, 8> cmds;
  cmds[0] = 1000;
  cmds[1] = 1125;
  cmds[2] = 1250;
  cmds[3] = 1375;
  cmds[4] = 1500;
  cmds[5] = 1625;
  cmds[6] = 1750;
  cmds[7] = 2000;
  pwm.tx_channels(cmds);
  pwm.Write();

  while (1) {}
}
