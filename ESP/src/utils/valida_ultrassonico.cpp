#ifdef testes  

#include <Arduino.h>

const int trigPin = 19;
const int echoPin = 18;

float duration, distance;

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

  duration = 0;
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  Serial.print("Distance: ");
    Serial.println(distance);
    delay(1000);
}

#endif