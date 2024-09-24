#include<TouchLed.h>

/* 
  TouchLed library example: Manual control

  In this example we'll cover three different, independent meanings of 'Manual Control'
  In comments {1},{2},{3} mark fragments of code relevant to respective topics.

    - {1} running the polling 'manually' without help of the RTOS task
    - {2} manual control of the button LED
    - {3} manual edge detection (press/release) if you're alergic to callbacks.


 Connect the LED's positive to T0 / GPIO4 
 Connect the LED's negative to GPIO5.
 No resistor needed.

           ESP32
        +-----------+
        |           |  Touch
        |           | _____          
        |           |   |
        |           |   |   ↗↗ LED
        |      Touch|---o--->|---o 
        |           |            |
        |       Sink|------------o
        |           |
        +-----------+

 On popular DoIt ESP32 Devkit v1 these are pins #24 and #34; looking from the USB side: 5th and 8th pin on the right.
 If you have a different ESP32, look up the pinout and feel free to edit.
 
*/


// {1} Unlike in the Callback example, this time the function will be called from the context of the sketch's loop()
// not from a background thread. That may be desirable to solve race condition issues and similar.
void onToggled( bool on )
{
  if(on)
  {
    Serial.println("Toggled On!");   
  }
  else
  {
    Serial.println("Toggled Off!");   
  }
}


// create a TouchButton object that utilizes Touch0 (GPIO4) and GPIO5 pins.
TouchButton button(T0, 5);

void setup() {
  Serial.begin(115200);

// {1} assign a callback function as always
// see Callbacks example for more details.
  button.toggledCallback = onToggled;

  // {1} We do not start the helper thread this time! 
  // button.begin(); 

  // {2} Completely decouple the LED behavior from the button touch state.
  button.SetLedBehavior(TouchButton::MANUAL);

  // We want to show that it works, somehow. 
  // The builtin LED of the ESP32 board will work fine for that.
  pinMode(LED_BUILTIN, OUTPUT);
}

// Blink the LED on the ESP32 for 100ms.
void blink_builtin()
{
    digitalWrite(LED_BUILTIN, 1);
    delay(100);
    digitalWrite(LED_BUILTIN, 0);
    delay(100);
}

void loop() 
{
  // {1} This causes a single touch readout and sets the LED to whatever it should be set to.
  button.recalculate(); 

  // {2} We make the LED blink steadily, one second on, one second off.
  int blink = ( (millis() / 1000) % 2 ) == 0; // little wizardry to extract even/odd seconds from the timer
  button.SetLedState(blink);

  // {3} Manual edge detection - if you're not comfortable with callbacks.
  // Blink the ESP's builtin LED once on touch. Blink twice on release

  // static variables, like globals, survive between subsequent function ( loop() ) calls but are local to the function. 
  static bool pressed = false; 

  // {3} on press
  if(!pressed && button.Pressed())
  {
    pressed = true;
    blink_builtin();
  }

  // {3} on release. If you have 'on press', you need this too, doesn't have to do anything, except for 'pressed = false;'
  if(pressed && !button.Pressed())
  {
    pressed = false; 
    blink_builtin();
    blink_builtin();
  }

  // {1} Delay IS important when running without manual recalculate().
  // Every call of the button.recalculate makes the LED go dark for duration of touch readout.
  // If you read non-stop without any delay, the LED will remain dark at all times.
  delay(200);
}

