#include <Arduino.h>

const int trigPin = 18;
const int echoPin = 19;
const int numReadings = 5; // Número de leituras para calcular a média

float duration, distance;
float distanceReadings[numReadings];
int readingIndex = 0;
float totalDistance = 0;

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);

    // Inicializa o array de leituras
    for (int i = 0; i < numReadings; i++) {
        distanceReadings[i] = 0;
    }
}

void loop() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.0343) / 2;

    // Subtrai a leitura mais antiga do total
    totalDistance -= distanceReadings[readingIndex];
    // Adiciona a nova leitura ao total
    distanceReadings[readingIndex] = distance;
    totalDistance += distance;

    // Atualiza o índice da leitura
    readingIndex = (readingIndex + 1) % numReadings;

    // Calcula a média
    float averageDistance = totalDistance / numReadings;

    Serial.print("Average Distance: ");
    Serial.println(averageDistance);

    delay(1000);
}
