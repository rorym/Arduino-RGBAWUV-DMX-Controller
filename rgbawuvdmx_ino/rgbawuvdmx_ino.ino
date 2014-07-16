/*
  DMX RGBWA+UV Controller with Strobe
  Created by Rory McMahon
  Arduino 6 Channel LED Shield 1 Amp 50khz PWM dim frequency
  DMX Controller
  Arduino Mega
*/


#include <Conceptinetics.h>

//
// CTC-DRA-13-1 ISOLATED DMX-RDM SHIELD JUMPER INSTRUCTIONS
//
// If you are using the above mentioned shield you should 
// place the RXEN jumper towards G (Ground), This will turn
// the shield into read mode without using up an IO pin
//
// The !EN Jumper should be either placed in the G (GROUND) 
// position to enable the shield circuitry 
//   OR
// if one of the pins is selected the selected pin should be
// set to OUTPUT mode and set to LOGIC LOW in order for the 
// shield to work
//

//
// The slave device will use a block of 10 channels counting from
// its start address.
//
// If the start address is for example 56, then the channels kept
// by the dmx_slave object is channel 56-66
//
#define DMX_SLAVE_CHANNELS   10 

//
// Pin number to change read or write mode on the shield
// Uncomment the following line if you choose to control 
// read and write via a pin
//
// On the CTC-DRA-13-1 shield this will always be pin 2,
// if you are using other shields you should look it up 
// yourself
//
///// #define RXEN_PIN                2


// Configure a DMX slave controller
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

// If you are using an IO pin to control the shields RXEN
// the use the following line instead
///// DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS , RXEN_PIN );

// Define the pins for each colour (PWM pins 2-7)
const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;
const int whitePin = 5;
const int amberPin = 6;
const int uvPin = 7;

// Define the DMX channels for each colour intensity
const int redDMXch = 1;
const int greenDMXch = 2;
const int blueDMXch = 3;
const int whiteDMXch = 4;
const int amberDMXch = 5;
const int uvDMXch = 6;
const int intensityDMXch = 7;
const int strobeDMXch = 8;

// Default levels for each colour
volatile int redLevel = 0;
volatile int greenLevel = 0;
volatile int blueLevel = 0;
volatile int whiteLevel = 0;
volatile int amberLevel = 0;
volatile int uvLevel = 0;

// Defaults for Strobe function
int strobeOn = 0;
int nextStrobe = millis();

// the setup routine runs once when you press reset:
void setup() {             
  
  // Enable DMX slave interface and start recording
  // DMX data
  dmx_slave.enable ();  
  
  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (1);
  
  // Set the PWM pins to output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(amberPin, OUTPUT);
  pinMode(uvPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() 
{
  // If the over all intensity is greater than 0
  if (dmx_slave.getChannelValue(intensityDMXch) > 0) {
    // Set the global intensity
    int globalIntensity = dmx_slave.getChannelValue(intensityDMXch);
    
    // Move the previous levels to another param
    int prevRedLevel = redLevel;
    int prevGreenLevel = greenLevel;
    int prevBlueLevel = blueLevel;
    int prevWhiteLevel = whiteLevel;
    int prevAmberLevel = amberLevel;
    int prevUvLevel = uvLevel;
    
    // Get the levels for each colour
    redLevel = dmx_slave.getChannelValue(redDMXch);
    greenLevel = dmx_slave.getChannelValue(greenDMXch);
    blueLevel = dmx_slave.getChannelValue(blueDMXch);
    whiteLevel = dmx_slave.getChannelValue(whiteDMXch);
    amberLevel = dmx_slave.getChannelValue(amberDMXch);
    uvLevel = dmx_slave.getChannelValue(uvDMXch);
    
    // Adjust the intensities per the global
    if (redLevel > globalIntensity) {
      redLevel = globalIntensity;
    }
    if (greenLevel > globalIntensity) {
      greenLevel = globalIntensity;
    }
    if (blueLevel > globalIntensity) {
      blueLevel = globalIntensity;
    }
    if (whiteLevel > globalIntensity) {
      whiteLevel = globalIntensity;
    }
    if (amberLevel > globalIntensity) {
      amberLevel = globalIntensity;
    }
    if (uvLevel > globalIntensity) {
      uvLevel = globalIntensity;
    }
    
    // If strobe is active
    if (dmx_slave.getChannelValue(strobeDMXch) > 0) {
      // Set the strobe rate
      int strobeRate = dmx_slave.getChannelValue(strobeDMXch);
      
      // If the strobe is in the ON position
      if (strobeOn > 0) {
        if (millis() - strobeRate >=0) {
          // turn everything down
          analogWrite(redPin, 0);
          analogWrite(greenPin, 0);
          analogWrite(bluePin, 0);
          analogWrite(whitePin, 0);
          analogWrite(amberPin, 0);
          analogWrite(uvPin, 0);
          nextStrobe = millis() + strobeRate;
          strobeOn = 0;
        }
      }
      // If the strobe is in the OFF position
      if (strobeOn < 1) {
        if (millis() - strobeRate >=0) {
          analogWrite(redPin, redLevel);
          analogWrite(greenPin, greenLevel);
          analogWrite(bluePin, blueLevel);
          analogWrite(whitePin, whiteLevel);
          analogWrite(amberPin, amberLevel);
          analogWrite(uvPin, uvLevel);
          nextStrobe = millis() + strobeRate;
          strobeOn = 1;
        }
      }
    }else{
      // Standard fade functions
      
      // Check if the red level has changed
      if (redLevel != prevRedLevel) {
        // set the red level if it's changed
        analogWrite(redPin, redLevel);
      }
      // Check if the green level has changed
      if (greenLevel != prevGreenLevel) {
        // set the green level if it's changed
        analogWrite(greenPin, greenLevel);
      }
      // Check if the blue level has changed
      if (blueLevel != prevBlueLevel) {
        // set the blue level if it's changed
        analogWrite(bluePin, blueLevel);
      }
      // Check if the whilte level has changed
      if (whiteLevel != prevWhiteLevel) {
        // set the whilte level if it's changed
        analogWrite(whitePin, whiteLevel);
      }
      // Check if the amber level has changed
      if (amberLevel != prevAmberLevel) {
        // set the amber level if it's changed
        analogWrite(amberPin, amberLevel);
      }
      // Check if the UV level has changed
      if (uvLevel != prevUvLevel) {
        // set the UV level if it's changed
        analogWrite(uvPin, uvLevel);
      }
    }
  }
}
