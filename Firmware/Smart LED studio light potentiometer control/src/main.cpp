#include <Arduino.h>

// PWM configuration
const int pwmFreq = 40000;      // 40 kHz for minimal flicker
const int pwmResolution = 8;    // 8-bit resolution (0-255)
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;

// Pin assignments
const int pwmPin1 = 8;          // Output PWM pin 1
const int pwmPin2 = 9;          // Output PWM pin 2
const int potPin1 = 0;         // Analog input pin for potentiometer 1
const int potPin2 = 1;         // Analog input pin for potentiometer 2

void setup() {
  // Configure PWM channels
  ledcSetup(pwmChannel1, pwmFreq, pwmResolution);
  ledcSetup(pwmChannel2, pwmFreq, pwmResolution);

  // Attach PWM channels to pins
  ledcAttachPin(pwmPin1, pwmChannel1);
  ledcAttachPin(pwmPin2, pwmChannel2);

  // Initialize serial for debugging (optional)
  Serial.begin(115200);
}

int medianAnalogRead(int pin, int samples = 5) {
  int values[5];
  for (int i = 0; i < samples; i++) {
    values[i] = analogRead(pin);
    delay(2); // Small delay between samples
  }
  // Simple bubble sort for small arrays
  for (int i = 0; i < samples - 1; i++) {
    for (int j = i + 1; j < samples; j++) {
      if (values[j] < values[i]) {
        int temp = values[i];
        values[i] = values[j];
        values[j] = temp;
      }
    }
  }
  return values[samples / 2]; // Median value
}

void loop() {
  // Median filter for potentiometer readings
  int potValue1 = medianAnalogRead(potPin1);
  int potValue2 = medianAnalogRead(potPin2);

  // Map to PWM range (0-255)
  int pwmValue1 = map(potValue1, 10, 4095, 0, 255);
  int pwmValue2 = map(potValue2, 10, 4095, 0, 255);

  // Set PWM duty cycle
  ledcWrite(pwmChannel1, pwmValue1);
  ledcWrite(pwmChannel2, pwmValue2);

  // Optional: print values for debugging
  Serial.printf("PWM1: %d, PWM2: %d\n", pwmValue1, pwmValue2);

  delay(10); // Small delay for stability
}