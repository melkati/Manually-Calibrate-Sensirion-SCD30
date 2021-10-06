/*
  Simple skech to manually calibrate CO2 sensor Sensirion SCD30
  More informatión at: https://emariete.com/medidor-co2-movil-esp32-sensirion-scd30-basico/
  
  Required libraries:
   - SparkFun_SCD30_Arduino_Library written by Nathan Seidle (SparkFun) at https://github.com/sparkfun/SparkFun_SCD30_Arduino_Library

  To-do:
  - Implement support for TTGO T-Display by showing status and count down timer on display


  Copyright (c) 2021 Mario Elkati - eMariete https://emariete.com
  Code, firmware, and software is released under the MIT License(http://opensource.org/licenses/MIT).  

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
  to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
  DEALINGS IN THE SOFTWARE.
*/

#include "SparkFun_SCD30_Arduino_Library.h"

// #define ALTERNATIVE_I2C_PINS // For the compact build as shown at https://emariete.com/medidor-co2-display-tft-color-ttgo-t-display-sensirion-scd30/
#define LEDPIN 2 // For standard ESP32 Devkit Board the bultin LED GPIO is 2

SCD30 airSensor;

#ifdef ALTERNATIVE_I2C_PINS
#define I2C_SDA 22
#define I2C_SCL 21
#else
#define I2C_SDA 21
#define I2C_SCL 22
#endif

uint16_t calibrationValue = 415;

void waitSeconds(uint16_t seconds)
{
  uint64_t startTime = millis();
  while ((startTime + seconds*1000) < millis())
  {
    delay(500);
    digitalWrite(LEDPIN,HIGH);
    delay(500);
    digitalWrite(LEDPIN,LOW);    
    Serial.print(".");
  }
  Serial.println();
}

void calibrateSensor()
{
  printf("Calibrating SCD30 sensor at %d PPM\n", calibrationValue);
  airSensor.setForcedRecalibrationFactor(calibrationValue);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

  // Set pin mode
  pinMode(LEDPIN,OUTPUT);

  // Initialize the SCD30 driver
  Wire.begin(I2C_SDA, I2C_SCL);
  if (airSensor.begin() == false)
  {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1)
    ;
  }
  Serial.println("Ready.");
}

void loop()
{
  waitSeconds(300);
  calibrateSensor();
  Serial.println("Calibration done.");
  digitalWrite(LEDPIN,HIGH);
  delay(500);
  while (1)
    ;
}
