#include<TouchLed.h>

/* 
  TouchLed library example - How to use Callbacks

  Callbacks make life easier - they trigger on change, not while something happens. 
  No need to manually edge-detect change between not pressed and pressed

 Connect the LED's positive to T0 / GPIO4 
 Connect the LED's negative to GPIO5.
 No resistor needed.

 On popular DoIt ESP32 Devkit C v1 these are pins #24 and #34; looking from the USB side: 5th and 8th pin on the right.
 If you have a different ESP32, look up the pinout and feel free to edit.
*/


// First, let's write some functions to call on button events.

// This one is launched the moment you touch the button
void onPressed()
{
    Serial.println("Pressed");   
}

// This one is launched the moment you remove your finger from the button
void onReleased()
{
  Serial.println("Released");   
}

// This one is launched if you touch the button longer

  // Notice: while the callback functions are running, the button's polling task is paused!
  // These five 'delay(500)' here make the button inert for 2.5 seconds!

void onLongPressed()
{
  Serial.println("Longpress!");   
  // Blink the ESP32's builtin LED three times
  digitalWrite(LED_BUILTIN, 1 );
  delay(500);
  digitalWrite(LED_BUILTIN, 0 );
  delay(500);
  digitalWrite(LED_BUILTIN, 1 );
  delay(500);
  digitalWrite(LED_BUILTIN, 0 );
  delay(500);
  digitalWrite(LED_BUILTIN, 1 );
  delay(500);
  digitalWrite(LED_BUILTIN, 0 );
}

// Unlike the remaining three, this one takes an argument - whether it was toggled On or Off.
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

// assign the callbacks: 
//  function name (without parentheses) is a pointer to that function; 
//  here we teach the button to call the four functions we've written.

  button.pressedCallback = onPressed;
  button.toggledCallback = onToggled;
  button.releasedCallback = onReleased;
  button.longpressCallback = onLongPressed;

  // Let's change duration of what is considered 'long press' - we change the default 3s to 2s. 
  button.longPressMs = 2000;

  // Start polling/control job in the background (a FreeRTOS task)
  button.begin();

  // Just to make things more interesting, make the button LED display the toggle status instead
  button.SetLedBehavior(TouchButton::TOGGLE);

  // We want to show that it works, somehow. 
  // The builtin LED of the ESP32 board will work fine for that.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  // Literally nothing to do! Callbacks do everything for us.
  // sure we CAN still do other stuff, read the button manually, etc. We just don't need to!
}

