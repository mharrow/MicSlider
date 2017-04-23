/*
      Microphone X-Y Position Control Program
  Developed for Teensy 3.6 with 2 Easy Driver Boards
      Targus Numpad connected to Host Port
 
        BUTTON FUNCTIONS
        
  KEY   Dec      FUNCTION   
---------------------------------------------------  
  1 -   86     Save Present x,y Position
  2 -   87     Move Y axis -
  3 -   88     Move to save x,y Position
  4 -   89     Move x axis -
  5 -   90
  6 -   33     Move x axis +
  7 -   64     Display Current x,y Position
  8 -   35     Move y axis +
  9 -   36     Move to centre x,y of each axis
 TAB     0
  /     81
  * -   82     Reference x,y to 0,0 position Home
BACK   127
  -     83
  +     84
ENTER    0

*/
#include <AccelStepper.h>
#include "USBHost_t36.h"

// Config AccelStepper for 2 Motor objects 
AccelStepper xAxis(1,2,5);
AccelStepper yAxis(1,3,6);

// Pin 13 has the LED on Teensy 3.0 use for status
int led = 13;

// Config Limit Switches
const byte xHome = 8;
const byte yHome = 9;
byte hBval; //status of home switch
byte stepperHomeReq;
byte homeSet;
byte xCentreReq;
byte storePositionReq;
int xSavedPosition;
int ySavedPosition;
byte moveToSavedPositionReq;
byte dumpCurrentPositionReq;

USBHost myusb;
KeyboardController keyboard1(myusb);

boolean xMove = LOW;
boolean yMove = LOW;

void setup() {

  // initialize the x and y home switch input pins
  pinMode(xHome,INPUT_PULLUP);
  pinMode(yHome,INPUT_PULLUP);

  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  
  xAxis.setMaxSpeed(1000);
  yAxis.setMaxSpeed(1000);
  xAxis.setAcceleration(100.0);
  yAxis.setAcceleration(100.0);
  
  myusb.begin();
  keyboard1.attachPress(OnPress);
  keyboard1.attachRelease(OnRelease);

  // Light LED to show system setup
  digitalWrite(led,HIGH);

  // Set homeSet LOW to indicate home position not set
  homeSet = LOW;
  stepperHomeReq = LOW;
  xCentreReq = LOW;
  storePositionReq = LOW;
  moveToSavedPositionReq = LOW;
  dumpCurrentPositionReq = LOW;
  
}

void loop() {
  
  myusb.Task();
  if (stepperHomeReq)
    {
      stepperHome();
    }
     
  if (xMove)
    {
      xAxis.runSpeed();
    }
  if (yMove)
    {
      yAxis.runSpeed(); 
    }
  if (xCentreReq)
    {
      xCentre();
    }
  if (storePositionReq)
    {
      storePosition();
    }
  if (moveToSavedPositionReq)
    {
      moveToSavedPosition();
    }
  if (dumpCurrentPositionReq)
    {
      dumpCurrentPosition();
    }
}

void OnPress(int key)
{
  Serial.print("key ");
  Serial.println(key);

  // test for Keypad 6 right arrow setup x move positive
  if (key == 33)
  {
    xMove = HIGH;
    xAxis.setSpeed(500);
  }

  // test for Keypad 8 up arrow setup y move positive
  if (key == 35)
  {
    yMove = HIGH;
    yAxis.setSpeed(500);
  }

  //test for Keypad * execute Home Routine
  if (key == 82)
  {
    stepperHomeReq = HIGH;
  }
  
  //test for Keypad 2 down arrow setup y move negative
  if (key == 87)
  {
    yMove = HIGH;
    yAxis.setSpeed(-500);
  }

  //test for Keypad 4 left arrow setup x move negative
  if (key == 89)
  {
    xMove = HIGH;
    xAxis.setSpeed(-500);
  }

  //test for Keypad 9 - move to centre of X axis
  if (key == 36)
  {
  xCentreReq = HIGH; 
  }
}

void OnRelease(int key)
{
  Serial.print("key end ");
  Serial.println(key);
  if (key == 33)
  {
    xMove = LOW;
  }
  if (key == 35)
  {
    yMove = LOW;
  }
  // keypad 7 = 64 dump current position
  if (key == 64)
  {
    dumpCurrentPositionReq = HIGH;
  }
  // keypad 1 = 86 store position of x and y axis
  if (key == 86)
  {
    storePositionReq = HIGH;   
  }
  if (key == 87)
  {
    yMove = LOW;
  }

  // keypad 3 = 88 move to saved position
  if (key == 88)
  {
    moveToSavedPositionReq = HIGH;
  }
  if (key == 89)
  {
    xMove = LOW;
  }
}


//Function to display current Position of x and y axis
void dumpCurrentPosition()
{
  Serial.print("X axis Position = ");
  Serial.println(xAxis.currentPosition());
  Serial.print("Y axis Position = ");
  Serial.println(yAxis.currentPosition());
  dumpCurrentPositionReq = LOW;  
}

//Function to store current Position of x and y axis
void storePosition()
{
  xSavedPosition = xAxis.currentPosition();
  ySavedPosition = yAxis.currentPosition();
  Serial.print("X axis Position Saved = ");
  Serial.println(xSavedPosition);
  Serial.print("Y axis Position Saved = ");
  Serial.println(ySavedPosition);
  storePositionReq = LOW;
}

//TODO
//Function to move to saved x and y position
void moveToSavedPosition()
{
  Serial.print("Move to saved position");
  
  xAxis.moveTo(xSavedPosition); 
  xAxis.setSpeed(500);
  while (xAxis.distanceToGo() != 0)
      xAxis.runSpeedToPosition();
 
  yAxis.moveTo(ySavedPosition);
  yAxis.setSpeed(500); 
  while (yAxis.distanceToGo() != 0)
      yAxis.runSpeedToPosition();
  
  moveToSavedPositionReq = LOW;
  
}

//TODO
//Function to move to centre of X axis
void xCentre()
{
  if (homeSet == LOW)
  {
    Serial.println("Unable to centre X axis Home not valid");
  }
  if (homeSet == HIGH)
  {
    Serial.println("Move to centre of X axis");  
  
  xAxis.moveTo(2500); 
  xAxis.setSpeed(500);
  while (xAxis.distanceToGo() != 0)
      xAxis.runSpeedToPosition();
 
  yAxis.moveTo(2500);
  yAxis.setSpeed(500); 
  while (yAxis.distanceToGo() != 0)
      yAxis.runSpeedToPosition();
    
  }
  
  xCentreReq = LOW;
}


// Function to home X axis then Y axis
void stepperHome()
{ 
  Serial.println("Home X axis routine started");
  hBval = digitalRead(xHome); 
  
  digitalWrite(led, LOW);   // turn off led to indicate x axis home start
  
  xAxis.setSpeed(-100);     // set low speed movement for home switch detect
  while (hBval == HIGH)
    {
      xAxis.runSpeed();
      hBval = digitalRead(xHome);
    }

  digitalWrite(led,HIGH);   // turn on led home switch detected
  xAxis.setCurrentPosition(0);    // zero the x axis
  
  Serial.println("X axis Home position set to 0");
  delay(4000);    // pause before y axis home start

  hBval = digitalRead(yHome);

  digitalWrite(led,LOW);    // turn off led to indicate y axis home start
  Serial.println("Home Y axis routine started");
  yAxis.setSpeed(-100);   // set low speed for y axis home switch detect

  while (hBval == HIGH)
    {
      yAxis.runSpeed();
      hBval = digitalRead(yHome);
    }

  yAxis.setCurrentPosition(0);
  Serial.println("Y axis Home position set to 0");
  digitalWrite(led,HIGH);   // led on home function complete

  stepperHomeReq = LOW;     // clear request for Home Function
   
  homeSet = HIGH;    // homeSet HIGH to indicate Axis Hardware Zero is set
}
