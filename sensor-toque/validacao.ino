// Defina o pino onde o sensor está conectado
const int collisionPin = 21; // Exemplo: GPIO 21

void setup() {
  // Inicialize a comunicação serial para monitorar os eventos
  Serial.begin(9600);
  
  // Configure o pino do sensor como entrada
  pinMode(collisionPin, INPUT);
}

void loop() {
  // Leia o estado do sensor de colisão
  int sensorValue = digitalRead(collisionPin);

  // Verifique se uma colisão foi detectada
  if (sensorValue == HIGH) {
    // Colisão detectada
    Serial.println("Colisão detectada!");
  } else {
    // Sem colisão
    Serial.println("Nenhuma colisão.");
  }

  // Aguarde um pouco antes de ler novamente
  delay(100);
}
