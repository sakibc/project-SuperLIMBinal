// Copyright 2018 Sakib Chowdhury and Claudia Lutfallah

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "outputStage-arduino.h"

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

#define SERVOMIN 130  // hard limits for our servos
#define SERVOMAX 550

// for synergies, min is extension, max is flexion
// degrees from 0 to 180 will be mapped as full extension-full flexion
#define THUMBMIN 480
#define THUMBMAX 260
#define INDEXMIN 500
#define INDEXMAX 200
#define MIDDLEMIN 500
#define MIDDLEMAX 160
#define RINGMIN 500
#define RINGMAX 200
#define PINKIEMIN 450
#define PINKIEMAX 150
#define WRISTMIN 550
#define WRISTMAX 220

#define SYNNUM 2 // number of synergies

// Synergy test0: hand open/close
// Synergy test1: wrist left/right
// Synergy  0: flex thumb
// Synergy  1: extend thumb
// Synergy  2: flex index
// Synergy  3: extend index
// Synergy  4: flex 
// Synergy  5:
// Synergy  6:
// Synergy  7:
// Synergy  8:
// Synergy  9:
// Synergy 10:
// Synergy 11:

uint16_t synergies[SYNNUM];
int mult = 1;

uint16_t readUnsignedInt()
{
  uint8_t dataBuffer[2];
  Serial.readBytes(dataBuffer, 2);
  return (dataBuffer[0] << 8) | dataBuffer[1];
}

int getPulseLength(int deg)
{
  // Convert from degrees to pulse length in ticks
  return map(deg, 0, 180, SERVOMIN, SERVOMAX);
}

void setup()
{
  Serial.begin(BAUDRATE);
  Serial.println("Serial OK");

  servos.begin();
  servos.setPWMFreq(60);

  delay(10);

  servos.setPWM(THUMB, 0, THUMBMIN);
  servos.setPWM(INDEX, 0, INDEXMIN);
  servos.setPWM(MIDDLE, 0, MIDDLEMIN);
  servos.setPWM(RING, 0, RINGMIN);
  servos.setPWM(PINKIE, 0, PINKIEMIN);
  servos.setPWM(WRIST, 0, WRISTMIN);

  servos.setPWM(ELBOW, 0, 335); // the elbow's not strong enough but this is
  // enough to prop the arm up a bit

  synergies[0] = 500;
  synergies[1] = 500;
}

void setHandOpenClose() {
  uint16_t level = synergies[0];

  servos.setPWM(THUMB, 0, map(level, 0, 1000, THUMBMIN, THUMBMAX));
  servos.setPWM(INDEX, 0, map(level, 0, 1000, INDEXMIN, INDEXMAX));
  servos.setPWM(MIDDLE, 0, map(level, 0, 1000, MIDDLEMIN, MIDDLEMAX));
  servos.setPWM(RING, 0, map(level, 0, 1000, RINGMIN, RINGMAX));
  servos.setPWM(PINKIE, 0, map(level, 0, 1000, PINKIEMIN, PINKIEMAX));
}

void setWristPosition() {
  uint16_t level = synergies[1];

  servos.setPWM(WRIST, 0, map(level, 0, 1000, WRISTMIN, WRISTMAX));
}

void loop()
{
  synergies[0] = readUnsignedInt();
  synergies[1] = readUnsignedInt();

  setHandOpenClose();
  setWristPosition();
}