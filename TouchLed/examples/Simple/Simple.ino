#include<TouchLed.h>

/* 
  TouchLed library example - simplest usecase example.

 Connect the LED's positive to T0 / GPIO4 
 Connect the LED's negative to GPIO5.
 No resistor needed.

 On popular DoIt ESP32 Devkit C v1 these are pins #24 and #34; looking from the USB side: 5th and 8th pin on the right.
 If you have a different ESP32, look up the pinout and feel free to edit.
*/


// create a TouchButton object that utilizes Touch0 (GPIO4) and GPIO5 pins.
TouchButton button(T0, 5);

void setup() {
  Serial.begin(115200);

  // Start polling/control job in the background (a FreeRTOS task)
  button.begin();

  // We want to show that it works, somehow. 
  // The builtin LED of the ESP32 board will work fine for that.
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() 
{
  Serial.printf("Touch readout: %d. Pressed: %s\n", 
    button.touchValue, 
    button.Pressed() ? "yes" : "no" 
    );

  // Every other touch of the button will toggle the LED on the ESP32.
  // The LED on the button follows default behavior of lighting up while the button is touched.
  if( button.Toggled() )
  {
    digitalWrite(LED_BUILTIN, 1);
  }
  else
  {
    digitalWrite(LED_BUILTIN, 0);
  }
  delay(300);
}

