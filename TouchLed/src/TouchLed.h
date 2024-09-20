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
      ledBehavior(PUSH),
      LedSetState(false),
      reversePolarity(false),
      twoWire(true)
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
    xTaskCreate(
      TouchButton::recalculateTask,
      "TouchButtonTask",
      2048,
      this,
      1,
      &taskHandle
    );
  }
  
  void end() {
    if (taskHandle != NULL) {
      vTaskDelete(taskHandle);
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

    if(twoWire) 
    { // Two
      pinMode(sinkPin, INPUT); // put sink into high impedance mode

      touchValue = touchRead(touchPin);
  
      pinMode(sinkPin, OUTPUT); // make sink a GND. 
      digitalWrite(sinkPin, 0);// !reversePolarity ); // make sink a GND. 
      
      pinMode(touchPin, OUTPUT);  // set the LED
      digitalWrite(touchPin, computeLed() ); //^ reversePolarity);
    }
    else  
    { // 3-wire op: LED between sink and GND, touch to touchpad.
      threeWireOp();
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
    switch(ledBehavior)
    {
      case PUSH:
        return pressed;
      case PUSH_INVERTED:
        return !pressed;
      case TOGGLE:
        return toggleIsOn;
      case MANUAL:
        return LedSetState;
    }
  }

  bool threeWireOp()
  {
    touchValue = touchRead(touchPin);
    pinMode(sinkPin, OUTPUT); // make sink a GND. 
    digitalWrite(sinkPin, computeLed() );// !reversePolarity ); // make sink a GND. 
  }

  inline bool LongPressed() { return longpressed; };
  inline bool Pressed() { return pressed; };
  inline bool Toggled() { return toggleIsOn; };
  inline void SetLedBehavior(LedBehavior behavior) { ledBehavior = behavior; }
  inline void SetLedState(bool state) { LedSetState = state; if(!twoWire){threeWireOp();} }

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
  bool twoWire;   // Set to false for 3-wire operation, Touch only goes to touch pad, Led is between Sink and Gnd.
};

