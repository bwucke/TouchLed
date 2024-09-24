#pragma once
#include "Arduino.h"


class TouchButton {
  public:
  TouchButton(int _touchPin, int _sinkPin, int _thresholdOn = 30, int _thresholdOff = 35, int _periodMs = 100, int _longPressMs = 3000) :
      touchPin(_touchPin),
      sinkPin(_sinkPin),
      periodMs(_periodMs),
      thresholdOn(_thresholdOn),
      thresholdOff(_thresholdOff),
      longPressMs(_longPressMs),
      lastPressTime(millis()),
      pressed(false),
      toggleIsOn(false),
      longpressed(false),
      longpresSent(false),
      touchValue(_thresholdOff),
      pressedCallback(NULL),
      toggledCallback(NULL),
      releasedCallback(NULL),
      longpressCallback(NULL),
      taskHandle(NULL),
      ledBehavior(PUSH),
      LedSetState(false),
      reversePolarity(false),
      threeWire(false)
  {}
	  
  ~TouchButton() {
    end();
  }
  
  enum LedBehavior {
      PUSH,
      PUSH_INVERTED,
      TOGGLE,
      MANUAL
  };
  
  void begin() {
    if(taskHandle == NULL)
    {
      xTaskCreate(
        TouchButton::recalculateTask,
        "TouchButtonTask",
        2048,
        this,
        1,
        &taskHandle
      );
    }
  }
  
  void end() {
    if (taskHandle != NULL) {
      vTaskDelete(taskHandle);
      taskHandle = NULL;
    }
  }


  static void recalculateTask(void *pvParameters) {
    TouchButton *button = static_cast<TouchButton *>(pvParameters);
    while(1) {
      button->recalculate();
      //vTaskDelay(pdMS_TO_TICKS(button->periodMs));
      vTaskDelay(button->periodMs / portTICK_PERIOD_MS);
    }
  }

  void recalculate()
  {

    if(!threeWire) 
    {  // Standard 2-wire operation
      pinMode(sinkPin, INPUT); // put sink into high impedance mode, for readout

      touchValue = touchRead(touchPin);
  
      pinMode(sinkPin, OUTPUT); // make sink a GND for the LED. 
      digitalWrite(sinkPin, reversePolarity ); 
      
      pinMode(touchPin, OUTPUT);  // set the LED 
      digitalWrite(touchPin, computeLed());
    }
    else  
    { // 3-wire op: LED between sink and GND (or 3.3V with reversePolarity), touch to touchpad.
      touchValue = touchRead(touchPin);
      pinMode(sinkPin, OUTPUT); // make sink a GND. 
      digitalWrite(sinkPin,  computeLed() );
    }

    // hysteresis processing of on/off readout
    if(! pressed && touchValue < thresholdOn)
    {
      pressed = true;
      toggleIsOn = !toggleIsOn;
      lastPressTime = millis();
      if(pressedCallback != NULL)
      {
        pressedCallback();
      }
      if(toggledCallback != NULL)
      {
        toggledCallback(toggleIsOn);
      }
    }

    if(pressed && touchValue > thresholdOff)
    {
      pressed = false;
      longpressed = false;
      longpresSent = false;
      if(releasedCallback != NULL)
      {
        releasedCallback();
      }
    }

    if(pressed && ( millis() - lastPressTime) >= longPressMs)
    {
      longpressed = true;
      if(!longpresSent && longpressCallback != NULL)
      {
        longpressCallback();
        longpresSent = true;
      }
    }
  }

  bool computeLed()
  {
    bool on = false;
    switch(ledBehavior)
    {
      case PUSH:
        on = pressed;
        break;
      case PUSH_INVERTED:
        on = !pressed;
        break;
      case TOGGLE:
        on = toggleIsOn;
        break;
      case MANUAL:
        on = LedSetState;
        break;
    }
    return on ^ reversePolarity;
  }

  inline bool LongPressed() { return longpressed; };
  inline bool Pressed() { return pressed; };
  inline bool Toggled() { return toggleIsOn; };
  inline void SetLedBehavior(LedBehavior behavior) { ledBehavior = behavior; }
  inline void SetLedState(bool state) { LedSetState = state; }

  int touchPin;
  int sinkPin;
  int periodMs;
  int thresholdOn; 
  int thresholdOff; 
  unsigned long longPressMs;
  unsigned long lastPressTime;
  bool pressed;
  bool toggleIsOn;
  bool longpressed;
  bool longpresSent;
  touch_value_t touchValue;
  void(*pressedCallback)();
  void(*toggledCallback)(bool on);
  void(*releasedCallback)();
  void(*longpressCallback)();
  TaskHandle_t taskHandle;
  LedBehavior ledBehavior;
  bool LedSetState;
  bool reversePolarity;   // AKA "I soldered the LED backwards"
  bool threeWire;   // Set to false for 3-wire operation, Touch only goes to touch pad, Led is between Sink and Gnd.
};

