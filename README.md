[![Pipeline](https://gitlab.com/bolderflight/software/pwm/badges/main/pipeline.svg)](https://gitlab.com/bolderflight/software/pwm/) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![Bolder Flight Systems Logo](img/logo-words_75.png) &nbsp; &nbsp; ![Arduino Logo](img/arduino_logo_75.png)

# Pwm
This library sends commands to PWM servos and is compatible with Arduino and CMake build systems. 
   * [License](LICENSE.md)
   * [Changelog](CHANGELOG.md)
   * [Contributing guide](CONTRIBUTING.md)

# Description
Pulse Width Modulation (PWM) is a protocol for receivers to send commands to servos. Each servo is sent a high pulse with the width of the pulse dictating the commanded position. Traditionally, PWM commands are sent at a 50 Hz interval with pulse widths varying between 1000 us and 2000 us. Some servos and Electronic Speed Controllers (ESCs) support higher frequencies, which typically requires a shorter pulse width range. PWM is the most common protocol for sending servo and ESC commands, but they require a PWM capable pin for each servo being controlled.

# Usage
This library presents a similar interface as our [SBUS library](https://github.com/bolderflight/sbus/) and uses [analogWrite](https://www.pjrc.com/teensy/td_pulse.html) to generate the PWM pulses. *analogWrite* **does not** have a well defined interface across Arduino devices and this library is not anticipated to work with boards other than Teensy 3.x, 4.x, and LC. Compared to the [Servo library](https://www.arduino.cc/reference/en/libraries/servo/), *analogWrite* sends all the pulses at the same rising pulse, whereas *Servo* sends them one after another, which causes an undesirable and variable latency, especially in later channels.

# Installation

## Arduino
Simply clone or download and extract the zipped library into your Arduino/libraries folder. The library is added as:

```C++
#include "pwm.h"
```

An example is located in *examples/arduino/pwm_example/pwm_example.ino*. This library is tested with Teensy 3.x, 4.x, and LC devices and is **not** expected to work with other Arduino devices.

## CMake
CMake is used to build this library, which is exported as a library target called *pwm*. The header is added as:

```C++
#include "pwm.h"
```

The library can be also be compiled stand-alone using the CMake idiom of creating a *build* directory and then, from within that directory issuing:

```
cmake .. -DMCU=MK66FX1M0
make
```

This will build the library and example executable called *pwm_example*. The example executable source file is located at *examples/cmake/pwm_example.cc*. Notice that the *cmake* command includes a define specifying the microcontroller the code is being compiled for. This is required to correctly configure the code, CPU frequency, and compile/linker options. The available MCUs are:
   * MK20DX128
   * MK20DX256
   * MK64FX512
   * MK66FX1M0
   * MKL26Z64
   * IMXRT1062_T40
   * IMXRT1062_T41

These are known to work with the same packages used in Teensy products. Also switching the packages is known to work well, as long as it's only a package change.

The *pwm_example* target creates an executable for communicating with PWM servos. This target also has a *_hex* for creating the hex file and an *_upload* for using the [Teensy CLI Uploader](https://www.pjrc.com/teensy/loader_cli.html) to flash the Teensy. Please note that the CMake build tooling is expected to be run under Linux or WSL, instructions for setting up your build environment can be found in our [build-tools repo](https://github.com/bolderflight/build-tools). 

# Namespaces
This library is within the namespace *bfs*

# PwmTx

**PwmTx(const std::array<int8_t, N> &pins)** This class is templated with the number of PWM pins and takes an array of pin numbers as input.

```C++
std::array<int8_t, 6> pins = {21, 22, 23, 2, 3, 4};
bfs::PwmTx<pins.size()> pwm(pins);
```

An optional template parameter is the PWM resolution.

```C++
/* 12 bit PWM resolution */
std::array<int8_t, 6> pins = {21, 22, 23, 2, 3, 4};
bfs::PwmTx<pins.size(), 12> pwm(pins);
```

By default the resolution is 16 bits.

**void Begin()** Initializes the PWM pins at the default frequency of 50 Hz.

```C++
pwm.Begin();
```

**void Begin(const float freq)** Initializes the PWM pins at the given frequency in Hz. Note that each *PwmTx* object only supports a single frequency. PWM pins are grouped by timer, changing the frequency of any pin in the group changes the frequency for all pins in the group. Refer to the [analogWrite](https://www.pjrc.com/teensy/td_pulse.html) for the groupings.

**void Write()** Updates the PWM commands sent to the servos.

```C++
pwm.Write();
```

**static constexpr int8_t NUM_CH()** A constant defining the number of PWM channels, useful for defining arrays to write the data from.

**void ch(const std::array<int16_t, N> &cmds)** Sets the channel data to be transmitted, given an array of PWM commands.

```C++
std::array<int16_t, 6> cmd = {1000, 1200, 1300, 1400, 1500, 1600};
pwm.ch(cmd);
```

**std::array<int16_t, N> ch()** Returns the channel data to be transmitted.

```C++
std::array<int16_t, 6> cmd = pwm.ch();
```
