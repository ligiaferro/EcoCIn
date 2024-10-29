#ifdef testes  

#include <Arduino.h>

const int pinA = 15;  // Pin for Bit 0
const int pinB = 2;  // Pin for Bit 1
const int pinC = 4;  // Pin for Beit 2

void setup() {
  Serial.begin(9600);
  // pinMode(pinA, PULLDOWN);
  // pinMode(pinB, PULLDOWN);
  // pinMode(pinC, PULLDOWN);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinC, INPUT);
}

void loop() {
  int bitA = digitalRead(pinA);
  int bitB = digitalRead(pinB);
  int bitC = digitalRead(pinC);
 
  // Convert the 3 bits to a number
  int number = (bitC << 2) | (bitB << 1) | bitA; // CBA

  Serial.println(number); // Print the number to the serial monitor
 
  delay(10); // Adjust delay as needed
}

#endif