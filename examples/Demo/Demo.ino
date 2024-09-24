#include<TouchLed.h>


/* 
  TouchLed library example - Literally Everything Else and Kitchensink.


  Watch Serial monitor for info.
  Longpress your touch button to advance to next phase.


 Connect the LED's positive to T0 / GPIO4 
 Connect the LED's negative to GPIO5.
 No resistor needed.
Optionally connect second LED in parallel with the first, in opposite direction.


           ESP32
        +-----------+
        |           |  Touch
        |           | _____          
        |           |   |
        |           |   |   
        |      Touch|---o--------o ---o
        |           |            |    |
        |           |       LED1 v    -
        |           |            -    ^ LED2
        |           |            |    |
        |       Sink|------------o----o
        |           |
        +-----------+

 On popular DoIt ESP32 Devkit C v1 these are pins #24 and #34; looking from the USB side: 5th and 8th pin on the right.
 If you have a different ESP32, look up the pinout and feel free to edit.
*/

TouchButton button(T0, 5);

int bootButton = 0; // normally the Boot button on the board is GPIO0, active low.

int state = 0;
int delayTime = 100;
int saveSensitivityOn;
int saveSensitivityOff;
int demoSelect = 1;


// Longpress advances to next demo stage.
void onLongPressed()
{
  blink_builtin();
  delay(100);
  blink_builtin();
  delay(100);
  blink_builtin();
  state = state + 1; // demo_mode++; if(demo_mode==10){ demo_mode = 0;}. 
}

void setup() {
  Serial.begin(115200);
  pinMode(bootButton, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  button.longpressCallback = onLongPressed;
  button.begin();
  Serial.println("Touch the button for 3 seconds to advance to advance to next phase.");
}


void loop() 
{
  switch(state)
  {
    case 0: // Setup toggle
      Serial.println("1. Just normal touch using callback.");
      button.SetLedBehavior(TouchButton::PUSH);
      button.pressedCallback = press_callback0;
      state = 1;
    break;
    case 1: // Loop first mode
      // await longpress, touch is doing the work.
    break;
    case 2: // Toggle demo.
      //cleanup after previous
      button.pressedCallback = NULL;
      //new demo
      Serial.println("2. Toggle mode");
      button.SetLedBehavior(TouchButton::TOGGLE);
      state = 3;
    //fall through
    case 3: // This time direct, no callbacks.
      digitalWrite(LED_BUILTIN, button.Toggled() );
    break;
    case 4:
      //new demo
      Serial.println("3. Touch and Release. Highlight inverted");
      button.SetLedBehavior(TouchButton::PUSH_INVERTED);
      button.pressedCallback = press_callback4;
      button.releasedCallback = release_callback4;
    //fall through
      state = 5;
    case 5:
      digitalWrite(LED_BUILTIN, button.Pressed() );
    break;
    case 6:
      //cleanup after previous
      button.pressedCallback = NULL;
      button.releasedCallback = NULL;
      //new demo
      Serial.println("4. Manual control. The touch button controls built-in LED, the Boot button controls touch LED.");
      button.SetLedBehavior(TouchButton::MANUAL);
      state = 7;
    //fall through
    case 7:
      digitalWrite(LED_BUILTIN, button.Pressed());
      button.SetLedState( !digitalRead(bootButton) );
    break;
    case 8: 
      button.SetLedBehavior(TouchButton::PUSH);
      //new demo
      Serial.println("5. Looooong press. To enter next phase hold the button for 10 seconds.");
      button.longPressMs=10000;
      state = 9;
      //fall through
    case 9:
    break;
    case 10:
      //cleanup after previous
      button.longPressMs=3000;  // restore sane length
      //new demo
      Serial.println("6. Switching polarity on every other toggle.");
      Serial.println("If you have a two-LED setup, the other LED will light up.");
      button.toggledCallback = toggle_callback11;
      button.SetLedBehavior(TouchButton::TOGGLE);
      state = 11;
      //fall through
    case 11:
    break;
    case 12:
      //cleanup after previous
      button.toggledCallback = NULL;
      button.reversePolarity = false;
      //new demo
      Serial.println("7. Touch to change blink (and sample) period.");
      button.SetLedBehavior(TouchButton::MANUAL);
      button.SetLedState( 1 );
      button.pressedCallback = press_callback12;
      button.periodMs = 50;
      state = 13;
      //fall through
    case 13:
    break;
    case 14:
      //cleanup after previous
      button.SetLedBehavior(TouchButton::PUSH);
      button.periodMs = 100;
      button.pressedCallback = NULL;
      //new demo
      Serial.println("8. Read touch period.");
      button.releasedCallback = release_callback14;
      state = 15;
      //fall through
    case 15:
    break;
    case 16:
      //cleanup after previous
      button.releasedCallback = NULL;
      //new demo
      Serial.println("9. Running without RTOS thread");
      button.end();
      button.pressedCallback = blink_builtin;
      state = 17;
      //fall through
    case 17:
      button.recalculate();
    break;
    case 18:
      //cleanup after previous
      button.begin();
      button.pressedCallback = NULL;
      //new demo
      Serial.println("10. Readout and change of sensitivity on the fly.");
      Serial.println("Take your hand away from the button and wait.");
      // wait for user to release the button.
      while(button.pressed)
      {
        delay(200);
      }
      delay(2000);  // Give the user time to move hand away, and readout of 'no touch' to settle.
      saveSensitivityOn = button.thresholdOn;
      saveSensitivityOff = button.thresholdOff;
      button.thresholdOn = button.touchValue - 2;
      button.thresholdOff = button.thresholdOn + 10;
      button.pressedCallback = press_callback18;
      Serial.println("Don't touch the button, just get your finger close to it.");
      state = 19;
      //fall through
    case 19:
    break;
    case 20:
      //cleanup after previous
      button.thresholdOn = saveSensitivityOn;
      button.thresholdOff = saveSensitivityOff;
      button.pressedCallback = NULL;
      //new demo
      Serial.println("11. Thank you.");
      Serial.println("Select demo number by touching the button that number of times, then longpress.");
      button.releasedCallback = release_callback20;
      button.longpressCallback = longpress_callback20;
      demoSelect = 0; // releasing the longpress will select 1.
      state = 21;
      //fall through
    case 21:
    break;
    

  }
  delay(delayTime);
}

void blink_builtin()
{
    digitalWrite(LED_BUILTIN, 1 );
    delay(100);
    digitalWrite(LED_BUILTIN, 0 );
}


void press_callback0()
{
  blink_builtin();
  Serial.println("Touched.");
}

void press_callback4()
{
  Serial.println("Touched.");
}

void release_callback4()
{
  Serial.println("Released.");
}

void toggle_callback11(bool on)
{
  if(on)
  {
    button.reversePolarity = ! button.reversePolarity;
  }
}

void press_callback12()
{
  button.periodMs += 100;
  if(button.periodMs > 1000) button.periodMs = 50;
}

void release_callback14()
{
  Serial.printf("Press lasted %d milliseconds.\n", millis() - button.lastPressTime);
}

void press_callback18()
{
  Serial.println("Detected.");
  blink_builtin();
  blink_builtin();
  blink_builtin();
}

void release_callback20()
{
  demoSelect++ ;
  if(demoSelect > 11) demoSelect = 1;
  Serial.printf("Currently selected demo: %d.\n", demoSelect);
}

void longpress_callback20()
{
  state = (demoSelect-1) * 2;
  button.longpressCallback = onLongPressed;
  button.releasedCallback = NULL;
}

