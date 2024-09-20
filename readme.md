
# TouchLed Arduino Library

![Touch Button Activation](img/touch.gif)

The **TouchLed** library lets you to hook up a LED to your ESP32 and have it double as a touch button.
More precisely, the connect the positive lead of the LED to some exposed wire, a piece of metal, whatever, and touching it will act as pushing a button. With LED automatically following depending on the role, pushbutton, toggle (press-on press-off), or whatever you want it to do.

## Features

- **Capacitive Touch Input**: for annoying people with wet hands.
- **LED Control**: 3 automatic and 1 manual mode.
- **Long Press Detection**: Touch, Toggle AND LongPress?!
- **Header-only**: Really small, really simple.
- **Runs on FreeRTOS background thread**: no loop() upkeep! Did you know you got FreeRTOS for free when adding the ESP32 board to Arduino? 
- **Project for 3D printable button**: Easy and pretty.

## Misfeatures
- **Configurable Touch Thresholds**: Not so much feature as annoyance, but hey, these things must be calibrated.
- **Blinking** - Incessant blinking when active, at configurable rate. Touch readout is forcing the LED off for a fraction of second.
- **3-Wire Operation**: Workaround for the blinking, with all the magic of LED being touch-sensitive gone.
 
The schematic couldn't be any easier. Not even a resistor for 2-wire operation. And getting something*working* as a touch pad is beyond easy, anything metal, metallized, conductive, or even wet works. Finding a *pretty* one is the challenge!

![Schematic for 2-wire and 3-wire mode](img/schematic.png)

"Touch" is any GPIO labelled on the pinout as Touch[num]. "Sink" is any GPIO, can be touch, can be non-touch. Must be able to write, not just read.

Why Touch and Sink, and not Touch and GND? Because connecting Touch to GND immediately puts it into 'pressed' state. The Sink is put into High Impedance mode during touch readout, and LOW state while LED is lit (making it act as Ground + some resistance)

Can it be made not to blink and still work with two wires? Probably, but it would require rewriting the builtin `touchRead()` to be non-blocking and perform the reads very, very fast (and aggregate them later to remove noise). Quite a bit above my current skill level.


## Installation

To install the TouchLed library:
 - Find TouchLed in the Arduino library manager and add it to Arduino IDE, 
 
or

 - download [TouchLed.h](https://raw.githubusercontent.com/bwucke/TouchLed/refs/heads/master/TouchLed/src/TouchLed.h) and put it into your sketch directory next to the .ino file
 

## Calibration

Once you have the hardware in place, before you start writing code, you need to know what values the touch input reads to set thresholds between 'pressed' and 'not pressed'.

Run this as a new sketch before using TouchLed with a new button, new board etc. (copy-paste or grab from examples/Callibration)

Write down the value it shows on serial terminal when not touching the button, and when touching it.
Pick the threshold values roughly halfway between these two values, 'on threshold' about 5-10 lower than 'off threshold', and pass them as 3rd (on, lower) and 4th (off, higher) parameter of the constructor. 

```C++
const int touchPin = 4;
const int sinkPin = 5;

void setup() {
  Serial.begin(115200);
  pinMode(sinkPin, INPUT);
}

void loop()
{
  Serial.println(touchRead(touchPin));
  delay(100);
}
```


## Examples

The most basic, typical usage will look something like this:

```cpp
#include <TouchLed.h>

TouchButton button(T0, 13, 30, 35);  // Touch pin T0, LED on pin 13

void setup() {
  Serial.begin(115200);
  button.begin();
  button.pressedCallback = onPressed;
}

bool pressed = false;

void loop() {
  if(pressed)
  {
    pressed = false;
    // do something interesting here too.
  }
}

void onPressed() {
  // do something interesting here
  Serial.println("Button pressed!");
  pressed = true;
}

```

For more detailed examples, refer to the `TouchLed/examples/` folder or examples attached when you install through Arduino Library Manager.

Topics covered:
 - direct reading, continuous and on edge
 - callbacks
 - 3-wire mode
 - manual control of the LED
 - running without FreeRTOS thread
 - an everything and a kitchensink demo

### Why Touch and Sink, not Touch and GND:
- **2-Wire Mode**: Connect a touchpad to the `touchPin` and an LED to the `sinkPin`.
- **3-Wire Mode**: Use separate pins for the touch input and the LED.

Refer to the images below for the button design:

### 3D Printable Button Design
3D printable STLs alongside with instruction how to build a button can be found in `TouchLed/extras` and on [Thingiverse](https://www.thingiverse.com/thing:6772678).

![Button Design (Outside)](img/outside.jpg)
![Button Design (Inside)](img/inside.jpg)

## Public API Reference

### Constructor

```cpp
TouchButton(int touchPin, 
            int sinkPin, 
            int thresholdOn = 30, 
            int thresholdOff = 35, 
            int periodMs = 100, 
            int longPressMs = 3000);
```

- `touchPin`: The pin connected to the touchpad.
- `sinkPin`: The pin controlling the LED.
- `thresholdOn`: Threshold for detecting a press.
- `thresholdOff`: Threshold for detecting a release.
- `periodMs`: The update period in milliseconds.
- `longPressMs`: The time (in milliseconds) to trigger a long press.

### Methods

- `void begin()`: Starts the FreeRTOS touch detection task.
- `void end()`: Stops the FreeRTOS touch detection task.
- `bool Pressed()`: Returns whether the button is currently pressed.
- `bool Toggled()`: Returns true after every even press, false after every odd.
- `bool LongPressed()`: Returns whether a long press has occurred.
- `void SetLedBehavior(LedBehavior behavior)`: See Led Behaviors below.
- `void SetLedState(bool state)`: Manually sets the LED state (only valid for `MANUAL` mode).

### LED behaviors

The library supports different **LED behaviors**, which can be configured using the `SetLedBehavior(TouchLed::BEHAVIOR)` method:
- `PUSH`: LED lights up while the button is pressed.
- `PUSH_INVERTED`: LED is off while pressed.
- `TOGGLE`: LED toggles between on/off states on each press.
- `MANUAL`: Full manual control of the LED state through `SetLedState()`.  

### Callbacks

- `void (*pressedCallback)()`: Set this to a function to handle the button press event.
- `void (*releasedCallback)()`: Set this to a function to handle the button release event.
- `void (*toggledCallback)(bool on)`: Set this to handle toggling.
- `void (*longpressCallback)()`: Set this to handle a long press event.

### Other Properties

-  `int touchPin`: Positive terminal GPIO#
-  `int sinkPin`: Negative terminal GPIO#
-  `int periodMs`: How often the touch reads the button (and annoyingly blinks the LED)
-  `int thresholdOn`: What readout counts as 'button pressed'
-  `int thresholdOff`: What readout counts as 'button released'
-  `unsigned long longPressMs`: How long to hold the button to count as Longpress ()
-  `unsigned long lastPressTime`: the millis() readout from the last press.
-  `bool pressed`: literally that.
-  `bool toggleIsOn`: Flips every other press. LED in TOGGLE mode follows that.
-  `bool longpressed`: literally that.
-  `bool longpresSent`: true after the longpress callback triggered but still pressed
-  `touch_value_t touchValue`: last raw readout
-  `TaskHandle_t taskHandle`: ...if you want to do stuff with the RTOS task.
-  `LedBehavior ledBehavior`: the enum holding currently set behavior.
-  `bool LedSetState`: What's been set by `SetLedState()`
-  `bool reversePolarity`: AKA "I soldered the LED backwards". Touch still needs to be on the 'touch' pin side. LED is a diode after all, and filters the touch signal away quite well, sensitivity goes down the gutter.
-  `bool twoWire;` set to false for 3-wire operation, Touch only goes to touch pad, Led is between Sink and Gnd. Led ceases blinking, reversePolarity doesn't work. 
  
### Private API Reference

None. It's all public. Literally anything can be changed on the run and will work OK and changing it may be useful in some very obscure scenarios, so after thorough consideration I found my `private:` part of the class empty. Changing the pins assigned to Touch and Sink on the fly will leave the old pins in uncertain state but otherwise, want to switch callback? Want to switch polarity? feel free.

## License

This library is released under the [Creative Commons Attribution License](https://creativecommons.org/licenses/by/4.0/). You are free to use, modify, and distribute the library, provided that you give appropriate credit.

