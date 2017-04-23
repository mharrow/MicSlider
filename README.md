# MicSlider

     Microphone X-Y Position Control Program
     =======================================
  Teensy 3.6 with 2 Easy Driver Boards. Targus
  Numpad connected to USB Host Port of Teensy 3.6.
  
  First version simple control of 2 stepper motors
  to move a microphone on x and y axis.
  
  Libraries used by this program.
  
  Accelstepper 
  
  (http://www.airspayce.com/mikem/arduino/AccelStepper/)
  
  USBHost_t36
 
  USB EHCI Host for Teensy 3.6
  Copyright 2017 Paul Stoffregen (paul@pjrc.com)

  
  
  Teensy 3.6
  ==========
  (img/teensy36.jpg)
  
  Easy Driver Board
  =================
  (img/EasyDriver_v45.jpg)
  
  
      Targus Numpad Function Chart
      ============================
            
  KEY   Dec      FUNCTION   
---------------------------------------------------  
  1 -   86     Save Present x,y Position
  2 -   87     Move Y axis -
  3 -   88     Move to saved x,y Position
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
