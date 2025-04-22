#include "RobotMotors.h"
#include "UltrasonicSensor.h"

const int MOTOR_LEFT_FWD = 35;
const int MOTOR_LEFT_BWD = 34;
const int MOTOR_RIGHT_FWD = 33;
const int MOTOR_RIGHT_BWD = 32;
const int EN_RIGHT = 31;
const int EN_LEFT = 36;

const int LEFT_SENSOR_TRIGGER = 17;
const int LEFT_SENSOR_ECHO = 15;
const int FRONT_SENSOR_TRIGGER = 14;
const int FRONT_SENSOR_ECHO = 13;
const int RIGHT_SENSOR_TRIGGER = 12;
const int RIGHT_SENSOR_ECHO = 11;

const float FRONT_OBSTACLE_THRESHOLD = 18.0;
const float SIDE_OBSTACLE_THRESHOLD = 2.5;

const int TURN_DURATION = 80;
const int SOFT_TURN_DURATION = 25;

// Robot state
enum State { OFF, BT, AUTO };
State state = OFF;

RobotMotors robot(
  MOTOR_LEFT_FWD, MOTOR_LEFT_BWD,
  MOTOR_RIGHT_FWD, MOTOR_RIGHT_BWD,
  EN_LEFT, EN_RIGHT, 60
);

UltrasonicSensor leftSensor(LEFT_SENSOR_TRIGGER, LEFT_SENSOR_ECHO);
UltrasonicSensor frontSensor(FRONT_SENSOR_TRIGGER, FRONT_SENSOR_ECHO);
UltrasonicSensor rightSensor(RIGHT_SENSOR_TRIGGER, RIGHT_SENSOR_ECHO);

float distanceLeft, distanceFront, distanceRight;

void updateSensorDistances() {
  distanceLeft = leftSensor.getDistanceCm();
  distanceFront = frontSensor.getDistanceCm();
  distanceRight = rightSensor.getDistanceCm();
}

void printSensorDistances() {
  Serial.print("Left: ");
  Serial.print(distanceLeft);
  Serial.print(" cm, Front: ");
  Serial.print(distanceFront);
  Serial.print(" cm, Right: ");
  Serial.print(distanceRight);
  Serial.println(" cm");
}

void navigateRobot() {
  if (distanceFront < FRONT_OBSTACLE_THRESHOLD && (distanceLeft > distanceRight && distanceLeft > 50 && distanceRight < 20)) {
      robot.left(TURN_DURATION, false);
      Serial.println("Avoiding: Turning RIGHT");
  } else {
    if (distanceLeft == 0) {
      robot.right(SOFT_TURN_DURATION + 10);
    } else if (distanceRight == 0) {
      robot.left(SOFT_TURN_DURATION + 10);
    } else {
      float diff = distanceLeft - distanceRight;
      if (diff > 4) {
        robot.left(SOFT_TURN_DURATION, false);
        robot.forward();
      } else if (diff < -4) {
        robot.right(SOFT_TURN_DURATION - 5, false);
        robot.forward();
      } else {
        robot.forward(20, false);
      }
    }
  }
}

void handleBluetoothControl() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    Serial.println("BT CMD Received: " + cmd);

    if (cmd == "F") {
      robot.forward();
      Serial.println("Action: Moving FORWARD");
    } else if (cmd == "B") {
      robot.backward();
      Serial.println("Action: Moving BACKWARD");
    } else if (cmd == "L") {
      robot.left();
      Serial.println("Action: Turning LEFT");
    } else if (cmd == "R") {
      robot.right();
      Serial.println("Action: Turning RIGHT");
    } else if (cmd == "S") {
      robot.stopMotors();
      Serial.println("Action: STOP");
    } else {
      Serial.println("Unknown Command: " + cmd);
    }
  }
}


void checkStateSwitch() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toUpperCase();

    if (input == "OFF") {
      state = OFF;
      robot.stopMotors();
      Serial.println("Switched to OFF state");
    } else if (input == "MODE BT") {
      state = BT;
      robot.stopMotors();
      Serial.println("Switched to BLUETOOTH mode");
    } else if (input == "MODE AUTO") {
      state = AUTO;
      robot.stopMotors();
      Serial.println("Switched to AUTONOMOUS mode");
    }
  }
}

void setup() {
  Serial.begin(9600);
  robot.begin();
  robot.setSpeed(50, 58);

  leftSensor.begin();
  frontSensor.begin();
  rightSensor.begin();

  delay(2000);
  Serial.println("Robot Ready. Send 'MODE BT', 'MODE AUTO', or 'OFF'.");
}

void loop() {
  checkStateSwitch();
//  state = OFF;

  switch (state) {
    case OFF:
      robot.stopMotors();
      break;

    case BT:
      handleBluetoothControl();
      break;

    case AUTO:
      updateSensorDistances();
      printSensorDistances();
      navigateRobot();
      break;
  }
}
