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
This library presents a similar interface as our [SBUS library](https://github.com/bolderflight/sbus/) and uses [analogWrite](https://www.pjrc.com/teensy/td_pulse.html) to generate the PWM pulses. *analogWrite* **does not** have a well defined interface across Arduino devices and this library is not anticipated to work with boards other than Teensy 3.x, 4.x, and LC. Compared to the *Servo* library, *analogWrite* sends the pulses at the same rising pulse, whereas *Servo* sends them one after another, which causes an undesirable and variable latency, especially in later channels.

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

**PwmTx(int8_t pins[N])** This class is templated with the number of PWM pins and takes an array of pin numbers as input..

```C++
int8_t pins[6] = {21, 22, 23, 2, 3, 4};
bfs::PwmTx<sizeof(pins)> pwm(pins);
```

Optional template parameters are the PWM frequency and the PWM resolution. Note that the pins are grouped by timer, changing the PWM frequency on one pin, changes it for all of the pins in that group. Only a single frequency is supported per *PwmTx* object. Refer to the [analogWrite](https://www.pjrc.com/teensy/td_pulse.html) documentation for the groupings.

```C++
/* 333 Hz PWM Frequency */
int8_t pins[6] = {21, 22, 23, 2, 3, 4};
bfs::PwmTx<sizeof(pins), 333> pwm(pins);
```

```C++
/* 800 Hz PWM Frequency, 12 bit PWM resolution */
int8_t pins[6] = {21, 22, 23, 2, 3, 4};
bfs::PwmTx<sizeof(pins), 800, 12> pwm(pins);
```

By default the PWM frequency is 50 Hz and the resolution is 16 bits.

**void Begin** Initializes the PWM pins.

```C++
pwm.Begin();
```

**void Write()** Sends the PWM commands to the servos.

```C++
pwm.Write();
```

**static constexpr int8_t NUM_CH()** A constant defining the number of PWM channels, useful for defining arrays to write the data from.

**bool ch(const int8_t idx, const int16_t val)** Sets the channel data to be transmitted, given a channel index and corresponding value. Returns true on success and false on failure.

```C++
/* Set channel 3 to a value of 1200 */
pwm.ch(3, 1200);
```

**int8_t ch(int16_t const * const data, const int8_t len)** Sets the channel data to be transmitted given a pointer to an array of commands, *data*, and the array length, *len*. Returns the number of channels copied on success or -1 on failure. Note that the maximum number of channels is the smaller of the *len* or *NUM_CH*.

```C++
int16_t cmd[bfs::PwmTx::NUM_CH()];
sbus.ch(cmd, bfs::PwmTx::NUM_CH());
```
