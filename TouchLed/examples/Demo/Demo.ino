#include<TouchLed.h>

/*
  Touch button with LED highlight on just two wires, for ESP32.
  
  Note: some newer ESP32s don't have the Touch inputs anymore. Sorry, SuperMini fans!
  On boards that do have Touch, some pins are officially 'Touch' but always read 0, 
  so make sure you pick a working one!
  
  You need: ESP32, and a rather bright LED of the color you want. 
  No resistor; I found the internal resistance and current provided by ESP32 in this
  configuration makes the LED with 100 ohm resistor far too dim for my liking.
  
  Pick a working Touch pin, and any other generic GPIO pin ("Sink").

  Connect as follows:

                       Touch
        +-----------+  _____ 	 	
        |  ESP32    |    |    ↗↗ LED
        |      Touch|----o---―>|-o 
        |           |            |
        |       Sink|------------o
        |           |   
        +-----------+
		
  (the Touch pad can be literally any piece of metal, even the trailing LED lead is sufficient.)
		
  First test your circuit with a sketch consisting of:

      setup(){ Serial.begin(115200); pinMode(sinkPin, INPUT); }
      loop() { Serial.println(serialRead(touchPin)); }  

  to see if it works, and what ranges it produces when touched and not touched. Write these down. 
  Every circuit is different and readTouch will produce different 'untouched' and 'touched' values,
  so you must adjust to these. Luckily once it works, it works, no more tweaking needed.
   

  In your project, initialize the instance with:

      TouchButton button(touchPin, sinkPin, threshold_on, threshold_off);

  Set threshold_off a bit below the lowest you observed when not touching the circuit, 
    threshold_on a bit below, good bit above readout 'when touching' (or go with defaults: 30,35)

  Either do button.begin() to start a background FreeRTOS task that does the read/LED operation,
  or just perform button.recalculate(); periodically from loop(); each call is a single touch readout
  and updates the LED state.

  You can poll the state using these:

      button.Pressed()
      button.Toggled()
      button.LongPressed()

  or set callbacks: pressedCallback, releasedCallback, longpressCallback, toggledCallback
  
      void reportPress() { Serial.println("Pressed"); }
      button.pressedCallback = reportPress;

      void reportToggle(bool on){} Serial.printf("Toggle %s\n",on?"on":"off"); }
      button.toggledCallback = reportToggle;

  By default the LED lights up when the button is touched. This can be changed like...

      button.SetLedBehavior(TouchButton::TOGGLE);

  with:
    PUSH - light when touched
    PUSH_INVERTED - light when not touched
    TOGGLE - one touch switches on, one switches off.
    MANUAL - LED lights up depending on button.SetLedState(0|1); regardless of touch

  Typically the touch is polled (and the LED blinks) every 200ms. You can change it with 5th constructor param (periodMS)
  or button.periodMs = ...
  Longpress is by default 3000ms, settable with 6th constructor param or button.longPressMs = ...
  
  If you soldered the LED backwards not all is lost. Set button.reversePolarity = true. Just make sure you're touching
  the part between 'touch' pin and the LED. LED is a diode after all and filters the touch signal away very efficiently.
  

*/

TouchButton button(T0, 5);


TouchButton* button = NULL;
void setup() {
  Serial.begin(115200);
  

  button->pressedCallback = reportPress;
  button->releasedCallback = reportRelease;
  button->longpressCallback = reportLongpress;
  button->toggledCallback = reportToggle;
  
  button->SetLedBehavior(TouchButton::TOGGLE);
}


void loop() 
{
    Serial.printf("TV: %d\n", button->touchValue);

  delay(300);
}



void reportPress()
{
    Serial.println("Pressed");
}

void reportRelease()
{
    Serial.println("Released");
}

void reportLongpress()
{
    Serial.println("Longpressed");
    pinMode(LED_BUILTIN, OUTPUT);
    for(int i=0;i<6;++i)
    {
      digitalWrite(LED_BUILTIN, (i%2 == 0) );
      delay(500);
    }
}

void reportToggle(bool on)
{
    Serial.printf("Toggle %s\n",on?"on":"off");
}
