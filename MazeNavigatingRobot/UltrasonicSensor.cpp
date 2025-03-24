/*
  UltrasonicSensor.cpp - Library for controlling an ultrasonic sensor.
  Created by Your Name, Month Day, Year.
  Released into the public domain.
*/

#include "UltrasonicSensor.h"

// Constructor with initializer list
UltrasonicSensor::UltrasonicSensor(int trig, int echo, float temp)
    : trigPin(trig),
      echoPin(echo),
      temperature(temp),
      timeOut(30000)  // Default timeout (about 5m max distance)
{
  // Initialize speed of sound
  updateSpeedOfSound();
}

void UltrasonicSensor::begin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void UltrasonicSensor::updateSpeedOfSound() {
  // Speed of sound in air (cm/us) adjusted for temperature
  // Formula: speed = 331.3 + (0.606 * temperature) meters/second
  // Convert to cm/microsecond: divide by 10000
  speedOfSound = (331.3 + (0.606 * temperature)) / 10000.0;
}

float UltrasonicSensor::getDistanceCm() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigger pin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin, return the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, timeOut);

  // If timeout occurred
  if (duration == 0) {
    return -1;
  }

  // Calculate the distance using cached speed of sound
  float distance = (duration * speedOfSound) / 2.0;

  return distance;
}

void UltrasonicSensor::setTemperature(float temp) {
  temperature = temp;
  // Update the speed of sound when temperature changes
  updateSpeedOfSound();
}

void UltrasonicSensor::setTimeout(unsigned long timeout) { timeOut = timeout; }

float UltrasonicSensor::getAverageDistanceCm(int numReadings) {
  float total = 0;
  int validReadings = 0;

  for (int i = 0; i < numReadings; i++) {
    float distance = getDistanceCm();
    if (distance >= 0) {
      total += distance;
      validReadings++;
    }
    delay(10);  // Short delay between readings
  }

  if (validReadings == 0) {
    return -1;
  }

  return total / validReadings;
}

float UltrasonicSensor::getMedianDistanceCm(int numReadings) {
  if (numReadings <= 0) {
    return -1;
  }

  // Create array to store readings
  float readings[numReadings];
  int validReadings = 0;

  // Collect valid readings
  for (int i = 0; i < numReadings; i++) {
    float distance = getDistanceCm();
    if (distance >= 0) {
      // Insertion sort - insert new reading in the right position
      int j = validReadings;
      while (j > 0 && readings[j - 1] > distance) {
        readings[j] = readings[j - 1];
        j--;
      }
      readings[j] = distance;
      validReadings++;
    }
    // delay(10);  // Short delay between readings
  }

  if (validReadings == 0) {
    return -1;
  }

  // Return median value
  if (validReadings % 2 == 0) {
    // Even number of readings, return average of middle two
    return (readings[validReadings / 2 - 1] + readings[validReadings / 2]) /
           2.0;
  } else {
    // Odd number of readings, return middle one
    return readings[validReadings / 2];
  }
}