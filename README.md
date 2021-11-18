# pwm
This library communicates with PWM servos. 
   * [License](LICENSE.md)
   * [Changelog](CHANGELOG.md)
   * [Contributing guide](CONTRIBUTING.md)

# Description
Pulse Width Modulation (PWM) is a protocol for receivers to send commands to servos. Each servo is sent a high pulse with the width of the pulse dictating the commanded position. Traditionally, PWM commands are sent at a 50 Hz interval with pulse widths varying between 1000 us and 2000 us. Some servos and Electronic Speed Controllers (ESCs) support higher frequencies, which typically requires a shorter pulse width range. PWM is the most common protocol for sending servo and ESC commands, but they require a PWM capable pin for each servo being controlled.

## Installation
CMake is used to build this library, which is exported as a library target called *pwm*. The header is added as:

```
#include "pwm/pwm.h"
```

The library can be also be compiled stand-alone using the CMake idiom of creating a *build* directory and then, from within that directory issuing:

```
cmake .. -DMCU=MK66FX1M0
make
```

This will build the library and example executable called *pwm_example*. The example executable source file is located at *examples/pwm_example.cc*. Notice that the *cmake* command includes a define specifying the microcontroller the code is being compiled for. This is required to correctly configure the code, CPU frequency, and compile/linker options. The available MCUs are:
   * MK20DX128
   * MK20DX256
   * MK64FX512
   * MK66FX1M0
   * MKL26Z64
   * IMXRT1062_T40
   * IMXRT1062_T41

These are known to work with the same packages used in Teensy products. Also switching the packages is known to work well, as long as it's only a package change.

The *pwm_example* target creates an executable for communicating with PWM servos. This target also has a *_hex* for creating the hex file to upload to the microcontroller. 

# Namespaces
This library is within the namespace *bfs*

# PwmTx

**PwmTx** This class is templated with the number of PWM pins.

```C++
std::array<int8_t, 6> pins = {21, 22, 23, 2, 3, 4};
bfs::PwmTx<pins.size()> pwm;
```

**void Init(const std::array<int8_t, N> &pins)** Initializes the PWM pins given an array of pin numbers.

```C++
pwm.Init(pins);
```

**void Write()** Sends the PWM commands to the servos.

```C++
pwm.Write();
```

**static constexpr int8_t NUM_CH** Returns the number of PWM channels.

**void ch(const std::array<int16_t, N> &ref)** Updates the PWM commands.

```C++
std::array<int16_t, pwm.NUM_CH> cmd = {1000, 1200, 1300, 1400, 1500, 1600};
pwm.ch(cmd);
```

**std::array<int16_t, N> ch()** Returns an array of the PWM commands.
