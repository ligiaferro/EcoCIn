#ifdef ESP1  // Isso garante que o código a seguir só será compilado no ambiente esp1

#include <Arduino.h>
#include <loramesh.h>
#include <SoftwareSerial.h>
#include <snpes_node.h>

SoftwareSerial SerialCommand(25, 26);
LoRaMESH lora(&SerialCommand);

#define CLT_ID 0x50
uint8_t possivel_gw_id = 0x10; 

// Definindo os pinos onde a raspberry está conectada.
const int pinA = 15;  // Pin for Bit 0
const int pinB = 2;  // Pin for Bit 1
const int pinC = 4;  // Pin for Bit 2

// Definindo pinos onde os sensor ultrassônico está conectado.
const int trigPin = 19;
const int echoPin = 18;

float duration, distance;
int readingIndex = 0;
float totalDistance = 0;

void lora_set_id(uint8_t id) {
    if(lora.setnetworkId(id)) {
    Serial.print("id da lora atualizado com sucesso!\n");
    } else {
    Serial.print("erro ao atualizar id da lora\n");
    }
}

void lora_send_packet(uint8_t id, uint8_t *payload, uint8_t size) {
    uint16_t send_id;
    if (lora.localId == 0) { // master
    send_id = id;
    }
    else { // slave
    send_id = lora.localId;
    }
    lora.PrepareFrameCommand(send_id, 0x7F, payload, size);
    lora.SendPacket();
    delay(100);
}

void lora_recv_packet(uint8_t *id, uint8_t *buffer, uint8_t *size) {
    uint16_t recv_id;
    uint8_t command;
    if (lora.ReceivePacketCommand(&recv_id, &command, buffer, size, 1000)) {
        *id = (uint8_t)recv_id;
    } 
    else {
        // Flush in case of error during packet reception
        //lora.SerialFlush(hSerialCommand);
        SerialCommand.flush();
    }
}

uint8_t lora_packet_available() {
    return (uint8_t)SerialCommand.available();
}

/**
 * CONFIGURATION OF THE LORA INTERFACE NEEDED BY THE SNPES LIBRARY.
 * THIS INTERFACE USES THE LORAMESH FUNCTIONS IMPLEMENTED IN THE DRIVER
 */

// Instantiating the LoRa interface for the SNPES Library
static LoraItf_t Radio = {
    .set_id = lora_set_id,
    .pkt_send = lora_send_packet,
    .pkt_recv = lora_recv_packet,
    .pkt_avail = lora_packet_available
};

// Function wrapper needed by the SNPES Library (millis)
uint32_t TimerMillis() {
    return (uint32_t)millis();
}

// Instantiating the timer interface for the SNPES Library
static TimerItf_t RadioTimer = {
    .delay = delay,
    .millis = TimerMillis
};

void setup() {
    Serial.begin(9600);
    delay(10000);
    SerialCommand.begin(9600);
    lora.begin(true);
    
    // Configurando o pino do sensor ultrassônico como entrada.
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    //Configurando os pinos da RASPBERRY e fazendo um PullDown
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(pinC, INPUT);
    pinMode(pinA, INPUT_PULLDOWN);
    pinMode(pinB, INPUT_PULLDOWN);
    pinMode(pinC, INPUT_PULLDOWN);

    delay(1000);

    bool localread = lora.localread();
    if(localread == true) {
        Serial.print("EH VDD ! ");
    }
    else {
        Serial.print("eh menitra...");
    }

    Serial.print(localread);

/* inicializa o protocolo */
    snpes_node_init(CLT_ID, &Radio, &RadioTimer);

  // Procurar por gateways na área de alcance
    if (snpes_scan(&possivel_gw_id) == SNPES_OK) {
        Serial.println("scan");
    // Conectar no gateway scaneado
        if (snpes_connect(possivel_gw_id) == SNPES_OK) {
            Serial.println("conectou");
        }
    }
}

void loop() {
  // Sincroniza o nó com o gateway
    snpes_node_sync(possivel_gw_id);

// RASPBERRY

    int bitA = digitalRead(pinA);
    int bitB = digitalRead(pinB);
    int bitC = digitalRead(pinC);
 
    // Convert the 3 bits to a number
    int number = (bitC << 2) | (bitB << 1) | bitA; // CBA
   
    Serial.println(number); // Print the number to the serial monitor
 
    delay(1000); // Adjust delay as needed

    if (number != 0) {
        Serial.println("Recebeu!!");

        // ULTRASSÔNICO
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = 0;
        duration = pulseIn(echoPin, HIGH);
        distance = (duration*.0343)/2;

        Serial.print("Distância: ");
        Serial.println(distance);
        delay(1000);

          // Cria o buffer com o valor do sensor
        uint8_t buffer[] = { (uint8_t)number };
        uint8_t bufferzinho[] = { (uint8_t)distance};

        // Envia o valor do sensor para o gateway
        if (snpes_send(possivel_gw_id, buffer, 1) == SNPES_OK) {
            Serial.println("Classe do lixo enviada com sucesso.");
        }
        if (snpes_send(possivel_gw_id, bufferzinho, 1) == SNPES_OK) {
            Serial.println("Valor do sensor enviado com sucesso.");
        }
    }

    delay(1000); // Aguarda 2 segundos antes de enviar novamente
}

#endif  


