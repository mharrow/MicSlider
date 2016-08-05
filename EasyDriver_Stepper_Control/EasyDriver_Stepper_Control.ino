/*
Source code http://www.bristolwatch.com/arduino/easy_driver.htm
Table 1.   Microstep Resolution Truth Table
 MS_1  MS_2   Resolution
 L    L     Full step (2 phase)
 H    L     Half step
 L    H     Quarter step
 H    H     Eighth step
 
 DIR = D6
 Step = D5  // LOW to HIGH
 SLEEP = D7 // active LOW
 MS_1 =   D8
 MS_2 = D9
 ENABLE_MOTOR = D10 // active LOW
 
 */

#define DIR_PIN 6
#define STEP_PIN 5
#define SLEEP 7
#define MS_1 8
#define MS_2 9
#define ENABLE_MOTOR 10

void setup() 
{
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(SLEEP, OUTPUT);
  pinMode(MS_1, OUTPUT);
  pinMode(MS_2, OUTPUT);
  pinMode(ENABLE_MOTOR, OUTPUT);

  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(SLEEP, HIGH);
  digitalWrite(MS_1, LOW); // set for single step
  digitalWrite(MS_2, LOW);
  digitalWrite(ENABLE_MOTOR, HIGH);  // motor off

}  // end setup

void loop()
{

  int i;

  motorOff();
  delay(2000); // wait for 3 seconds
  digitalWrite(DIR_PIN, LOW);     // Set the direction.
  fullStep();
  motorOn();

  for (i = 0; i< 200; i++)       // Iterate for 200 microsteps.
  {
    pulseOut(STEP_PIN);
    delay(5);
  }

  motorOff();
  delay(2000); // wait for 3 seconds
  digitalWrite(DIR_PIN, HIGH);     // Set the direction.
  halfStep();
  motorOn();

  for (i = 0; i< 400; i++)       // Iterate for 400 microsteps.
  {
    pulseOut(STEP_PIN);
    delay(2);
  }

  motorOff();
  delay(2500); // wait for 3 seconds
  digitalWrite(DIR_PIN, LOW);     // Set the direction.
  quarterStep();
  motorOn();

  for (i = 0; i< 800; i++)       // Iterate for 800 microsteps.
  {
    pulseOut(STEP_PIN);
    delay(1);
  }

  motorOff();
  delay(2000); // wait for 3 seconds
  digitalWrite(DIR_PIN, HIGH);     // Set the direction.
  eighthStep();
  motorOn();

  for (i = 0; i< 1600; i++)       // Iterate for 1600 microsteps.
  {
    pulseOut(STEP_PIN);
    delay(2);
  }

} // end loop

// inverts state of pin, delays, then reverts state back
void    pulseOut(byte x)   {
  byte z = digitalRead(x);
  delayMicroseconds(10);
  z = !z; // reverse state
  digitalWrite(x, z);
  z = !z; // return to original state
  digitalWrite(x, z);
} // end pulsout()


void fullStep(void)   {
  digitalWrite(MS_1, LOW);
  digitalWrite(MS_2, LOW);
}

void halfStep(void)   {
  digitalWrite(MS_1, HIGH);
  digitalWrite(MS_2, LOW);
}

void quarterStep(void)   {
  digitalWrite(MS_1, LOW);
  digitalWrite(MS_2, HIGH);
}

void eighthStep(void)   {
  digitalWrite(MS_1, HIGH);
  digitalWrite(MS_2, HIGH);
}


void motorOff(void)   {
  digitalWrite(ENABLE_MOTOR, HIGH);
}

void motorOn(void)   {
  digitalWrite(ENABLE_MOTOR, LOW);
}

