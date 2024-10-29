#ifdef testes  

#include <ESP32Servo.h>

//SERVOS:
//Laranja -> Sinal
//Vermelho -> VCC
//Marrom -> GND

// Defina o pino onde o servo está conectado
const int servoPin = 36;

// Crie um objeto Servo
Servo myServo;

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);
  
  // Anexa o servo ao pino definido
  myServo.attach(servoPin);
  
  // Posiciona o servo em 0 graus
  myServo.write(0);
  delay(1000); // Espera 1 segundo
}

void loop() {
  // Gira o servo para 180 graus
  Serial.println("Girando para 180 graus");
  myServo.write(180);
  delay(2000); // Espera 2 segundos
  
  // Gira o servo de volta para 0 graus
  Serial.println("Girando para 0 graus");
  myServo.write(0);
  delay(2000); // Espera 2 segundos
}

#endif
