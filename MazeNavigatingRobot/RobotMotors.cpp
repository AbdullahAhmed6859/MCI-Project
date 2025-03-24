/*
  RobotMotors.cpp - Library for controlling robot motors.
  Created by Your Name, Month Day, Year.
  Released into the public domain.
*/

#include "RobotMotors.h"

RobotMotors::RobotMotors(int leftFwd, int leftBwd, int rightFwd, int rightBwd,
                         int enableLeft, int enableRight, int defaultSpd) {
  motorLeftFwd = leftFwd;
  motorLeftBwd = leftBwd;
  motorRightFwd = rightFwd;
  motorRightBwd = rightBwd;
  enLeft = enableLeft;
  enRight = enableRight;
  defaultSpeed = defaultSpd;
}

void RobotMotors::begin() {
  // Set all motor pins as outputs
  pinMode(motorLeftFwd, OUTPUT);
  pinMode(motorLeftBwd, OUTPUT);
  pinMode(motorRightFwd, OUTPUT);
  pinMode(motorRightBwd, OUTPUT);

  // Set enable pins as outputs
  pinMode(enLeft, OUTPUT);
  pinMode(enRight, OUTPUT);

  // Set default speed
  analogWrite(enLeft, defaultSpeed);
  analogWrite(enRight, defaultSpeed);
}

void RobotMotors::setSpeed(int leftSpeed, int rightSpeed) {
  analogWrite(enLeft, leftSpeed);
  analogWrite(enRight, rightSpeed);
}

void RobotMotors::forward(int ms) {
  digitalWrite(motorLeftFwd, HIGH);
  digitalWrite(motorLeftBwd, LOW);
  digitalWrite(motorRightFwd, HIGH);
  digitalWrite(motorRightBwd, LOW);

  if (ms > 0) {
    delay(ms);
    stop();
  }
}

void RobotMotors::backward(int ms) {
  digitalWrite(motorLeftFwd, LOW);
  digitalWrite(motorLeftBwd, HIGH);
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightBwd, HIGH);

  if (ms > 0) {
    delay(ms);
    stop();
  }
}

void RobotMotors::left(int ms) {
  digitalWrite(motorLeftFwd, LOW);
  digitalWrite(motorLeftBwd, HIGH);
  digitalWrite(motorRightFwd, HIGH);
  digitalWrite(motorRightBwd, LOW);

  if (ms > 0) {
    delay(ms);
    stop();
  }
}

void RobotMotors::right(int ms) {
  digitalWrite(motorLeftFwd, HIGH);
  digitalWrite(motorLeftBwd, LOW);
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightBwd, HIGH);

  if (ms > 0) {
    delay(ms);
    stop();
  }
}

void RobotMotors::stop(int ms) {
  digitalWrite(motorLeftFwd, LOW);
  digitalWrite(motorLeftBwd, LOW);
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightBwd, LOW);

  if (ms > 0) {
    delay(ms);
  }
}