/*

  Run this before using TouchLed with a new button, new board etc.

  Write down the value it shows on serial terminal when not touching the button, and when touching it.
  Set the thresholds roughly halfway between these two values, 'on threshold' about 5-10 lower than 'off threshold'
  
*/ 

// Adjust to match where you've connected them.

const int touchPin = 4;   //T0
const int sinkPin = 5;


void setup() {
  Serial.begin(115200);
  pinMode(sinkPin, INPUT);  // high impedance
}

void loop()
{
  Serial.println(touchRead(touchPin));
  delay(100);
}