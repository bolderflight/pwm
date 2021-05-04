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

# Methods
This driver conforms to the [Effector interface](https://github.com/bolderflight/effector); please refer to those documents for information on the *EffectorConfig* and struct.

**bool Init(const EffectorConfig &ref)** Initializes the PWM pins. Returns true on success.

```C++
/* PWM object */
std::array<int8_t, 6> pins = {21, 22, 23, 2, 3, 4};
bfs::PwmTx<pins.size()> pwm;
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
```

**void Cmd(std::span<float> cmds)** Issues angle commands, which are converted to PWM commands and stored.

```C++
cmds[0] = data.throttle;
pwm.Cmd(cmds);
```

**void Write()** Sends the stored commands to the servos. This method should be called every time the commands change.

```C++
pwm.Write();
```

**void EnableMotors()** Enables motors to output commands.

```C++
pwm.EnableMotors();
```

**void DisableMotors()** Disables motors from outputting commands, the failsafe command is sent instead.

**void EnableServos()** Enables servos to output commands.

```C++
pwm.EnableServos();
```

**void DisableServos()** Disables servos from outputting commands, the failsafe command is sent instead.
