/*
  UltrasonicSensor.h - Library for controlling an ultrasonic sensor.
  Created by Your Name, Month Day, Year.
  Released into the public domain.
*/

#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

// Check which platform we're using and include appropriate header
#if defined(ENERGIA)  // Energia platform
#include "Energia.h"
#else  // Arduino platform (default)
#include "Arduino.h"
#endif

class UltrasonicSensor {
 private:
  const int trigPin;   // Trigger pin
  const int echoPin;   // Echo pin
  float timeOut;       // Timeout in microseconds (for limiting max distance)
  float temperature;   // Temperature in Celsius (for speed of sound adjustment)
  float speedOfSound;  // Cached speed of sound in cm/microsecond
  float threshold;

  // Update speed of sound based on temperature
  void updateSpeedOfSound();

 public:
  // Constructor with initializer list
  UltrasonicSensor(int trig, int echo, float temp = 20.0);

  // Initialize pins
  void begin();

  // Get distance in centimeters
  float getDistanceCm();

  // Set the temperature for more accurate readings
  void setTemperature(float temp);

  // Set timeout (useful for limiting max distance)
  void setTimeout(unsigned long timeout);

  // Perform multiple readings and return average (for better accuracy)
  float getAverageDistanceCm(int numReadings = 5);

  // Perform multiple readings and return median using insertion sort
  float getMedianDistanceCm(int numReadings = 5);
};

#endif
