/*
  RobotMotors.h - Library for controlling robot motors.
  Created by Your Name, Month Day, Year.
  Released into the public domain.
*/

#ifndef RobotMotors_h
#define RobotMotors_h

// Check which platform we're using and include appropriate header
#if defined(ENERGIA)  // Energia platform
#include "Energia.h"
#else  // Arduino platform (default)
#include "Arduino.h"
#endif

class RobotMotors {
 private:
  // Motor pins
  int motorLeftFwd;
  int motorLeftBwd;
  int motorRightFwd;
  int motorRightBwd;

  // PWM pins for speed control
  int enLeft;
  int enRight;

  // Default speed value
  int defaultSpeed;

 public:
  // Constructor
  RobotMotors(int leftFwd, int leftBwd, int rightFwd, int rightBwd,
              int enableLeft, int enableRight, int defaultSpd = 50);

  // Initialize pins
  void begin();

  // Set motor speeds
  void setSpeed(int leftSpeed, int rightSpeed);

  // Movement functions
  void forward(int ms = 0, bool stop = false);
  void backward(int ms = 0, bool stop = false);
  void left(int ms = 0, bool stop = false);
  void right(int ms = 0, bool stop = false);
  void stopMotors(int ms = 0);
};

#endif
