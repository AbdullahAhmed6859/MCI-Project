#include "RobotMotors.h"
#include "UltrasonicSensor.h"

const int MOTOR_LEFT_FWD = 35;
const int MOTOR_LEFT_BWD = 34;
const int MOTOR_RIGHT_FWD = 33;
const int MOTOR_RIGHT_BWD = 32;
const int EN_LEFT = 31;
const int EN_RIGHT = 36;

const int LEFT_SENSOR_TRIGGER = 12;
const int LEFT_SENSOR_ECHO = 11;
const int FRONT_SENSOR_TRIGGER = 14;
const int FRONT_SENSOR_ECHO = 13;
const int RIGHT_SENSOR_TRIGGER = 17;
const int RIGHT_SENSOR_ECHO = 15;

const float FRONT_OBSTACLE_THRESHOLD = 20.0;
const float SIDE_OBSTACLE_THRESHOLD = 10.0;

const int TURN_DURATION = 200;
const int SOFT_TURN_DURATION = 50;

RobotMotors robot(
  MOTOR_LEFT_FWD, MOTOR_LEFT_BWD,
  MOTOR_RIGHT_FWD, MOTOR_RIGHT_BWD,
  EN_LEFT, EN_RIGHT, 50
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
  if (distanceFront < FRONT_OBSTACLE_THRESHOLD) {
    if (distanceLeft < distanceRight) {
      robot.left(TURN_DURATION);
      Serial.println("Avoiding: Turning LEFT");
    } else {
      robot.right(TURN_DURATION);
      Serial.println("Avoiding: Turning RIGHT");
    }
  } else {
    if (distanceLeft < SIDE_OBSTACLE_THRESHOLD) {
      robot.right(SOFT_TURN_DURATION);
      Serial.println("Soft turn RIGHT");
    }
    if (distanceRight < SIDE_OBSTACLE_THRESHOLD) {
      robot.left(SOFT_TURN_DURATION);
      Serial.println("Soft turn LEFT");
    }
    
    robot.forward();
    Serial.println("Moving FORWARD");
  }
}

void setup() {
  Serial.begin(9600);
  robot.begin();
  
  leftSensor.begin();
  frontSensor.begin();
  rightSensor.begin();
  
  delay(2000);
  robot.forward();
}

void loop() {
  updateSensorDistances();
  printSensorDistances();
  navigateRobot();
  
  delay(20);
}
