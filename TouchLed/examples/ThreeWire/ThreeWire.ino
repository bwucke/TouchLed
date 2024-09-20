#include<TouchLed.h>

/* 
  TouchLed library example - Simple example with three-wire operation, Touch and Led separate.
  At cost of extra complexity the inherent LED blinking is gone. 
  Also 'Sink' definitely becomes a misnomer in this case.


      ESP32
  +-----------+
  |           |  Touch
  |           |  _____          
  |           |    |
  |      Touch|----o
  |           |
  |           |     ↗↗ LED
  |       Sink|----->|-----o 
  |           |            |
  |        GND|---/\/\/\/--o
  |           |    100Ω
  +-----------+
        
 On popular DoIt ESP32 Devkit C v1 Touch and Sink are pins #24 and #34; looking from the USB side: 5th and 8th pin on the right. GND is 2nd. 
 If you have a different ESP32, look up the pinout and feel free to edit.

*/

// create a TouchButton object that utilizes Touch0 (GPIO4) and GPIO5 pins.
TouchButton button(T0, 5);


// Event handler. Blink the button LED 3 times then return it to the default state.
void onTouch()
{
  button.SetLedBehavior(TouchButton::MANUAL);
  for(int i=0; i<3;++i)
  {
    digitalWrite(LED_BUILTIN, 1);
    delay(50);
    digitalWrite(LED_BUILTIN, 0);
    delay(50);
  }
  // restore the default behavior.
  button.SetLedBehavior(TouchButton::PUSH);
}


void setup() {
  Serial.begin(115200);

  // Disable the 3-wire op.
  button.twoWire = false;
  // initialize Callback
  button.pressedCallback = onTouch;
  // Start polling/control job in the background (a FreeRTOS task)
  button.begin();

}


void loop() 
{
  // nothing needed. Thread and Callback handle all. 
}

