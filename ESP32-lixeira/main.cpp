#include <Arduino.h>

const int collisionPin = 21; // Pino do sensor de colisão
const int trigPin = 2; // Pino do Trigger do sensor ultrassônico
const int echoPin = 4; // Pino do Echo do sensor ultrassônico
const int numReadings = 5; // Número de leituras para calcular a média

float duration, distance;
float distanceReadings[numReadings];
int readingIndex = 0;
float totalDistance = 0;

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(collisionPin, INPUT);
    Serial.begin(9600);

    // Inicializa o array de leituras
    for (int i = 0; i < numReadings; i++) {
        distanceReadings[i] = 0;
    }
}

void loop() {
    // Medição do sensor ultrassônico
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.0343) / 2;

    // Atualiza as leituras e calcula a média
    totalDistance -= distanceReadings[readingIndex];
    distanceReadings[readingIndex] = distance;
    totalDistance += distance;
    readingIndex = (readingIndex + 1) % numReadings;
    float averageDistance = totalDistance / numReadings;

    // Leitura do sensor de colisão
    int sensorValue = digitalRead(collisionPin);
    if (sensorValue == LOW) {
        Serial.println("Colisão detectada!");
    }

    // Imprime a distância média
    Serial.print("Average Distance: ");
    Serial.println(averageDistance);

    // Aguarda um pouco antes de ler novamente
    delay(1000); // Ajuste o delay conforme necessário
}
