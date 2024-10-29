#ifdef ESP2  // Isso garante que o código a seguir só será compilado no esp2

#include <Arduino.h>
#include <loramesh.h>
#include <SoftwareSerial.h>
#include <snpes_gateway.h>
#include <string.h>

#include <WiFi.h>
#include <PubSubClient.h>

SoftwareSerial SerialCommand(25, 26); // pinos da esp na serial 
LoRaMESH lora(&SerialCommand); // inicializando serial lora
uint16_t deviceId = 0;
uint32_t deviceUniqueId;

const char* ssid = "rede";
const char* password = "senha";
const char* mqtt_server = "ip"; // ip do wlp2s0 em ifconfig 

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// FUNCOES LORA !

void lora_set_id(uint8_t id)
{
  if(lora.setnetworkId(id)) {
    Serial.print("id da lora atualizado com sucesso!\n");
  } else {
    Serial.print("erro ao atualizar id da lora\n");
  }
}

void lora_send_packet(uint8_t id, uint8_t *payload, uint8_t size)
{
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


void lora_recv_packet(uint8_t *id, uint8_t *buffer, uint8_t *size)
{
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

uint8_t lora_packet_available()
{
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

// FUNCOES PARA REDE WIFI ESP + MQTT

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    delay(100);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  Serial.begin(9600);
  delay(10000);
  SerialCommand.begin(9600);
  lora.begin(true);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  delay(1000);
  bool localread = lora.localread();
  if(localread == true) {
    Serial.print("EH VDD ! "); 
  }
  else {
    Serial.print("eh menitra...");
  }

  Serial.print(localread);

  static const uint8_t GW_ID = 0x10; // Choose a Unique ID for this Gateway
  snpes_gw_init(GW_ID, &Radio, &RadioTimer);

  // uint16_t id, net;
  // uint32_t uniqueId;
  // LocalRead(&id,&net,&uniqueId);

}

  int i;
  int n = 0;
  int m=0,nr=0,pl=0,v=0,p=0,o=0; 

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buffer[255];
  uint16_t msg_size;
  uint8_t id_clt;

  if (!client.connected()) { // conexao mqtt
    reconnect();
  }
  client.loop();

  // run the state machine every interation
  snpes_compute();

  // for(i=0;i<5;i++){
  //   buffer[0] = i;
  //   snprintf(msg, MSG_BUFFER_SIZE, "%ld", int(buffer[0]));
  //   client.publish("plástico", msg);
  //   delay(100);
  //   snprintf(msg, MSG_BUFFER_SIZE, "%ld", int(buffer[0]));
  //   client.publish("papel", msg);
  //   delay(100);

  //   snprintf(msg, MSG_BUFFER_SIZE, "%ld", int(buffer[0]));
  //   client.publish("orgânico", msg);
  //   delay(100);

  //   snprintf(msg, MSG_BUFFER_SIZE, "%ld", int(buffer[0]));
  //   client.publish("vidro", msg);
  //   delay(100);

  //   snprintf(msg, MSG_BUFFER_SIZE, "%ld", int(buffer[0]));
  //   client.publish("metal", msg);
  //   delay(100);

  //   snprintf(msg, MSG_BUFFER_SIZE, "%ld", int(buffer[0]));
  //   client.publish("não reciclável", msg);
  //   delay(100);
  // }
  // n++;
  
  // buffer[0] = n;
  // snprintf(msg, MSG_BUFFER_SIZE, "%ld", int(buffer[0]));
  // client.publish("type", msg);

    
  if (snpes_data_available()) {
    Serial.printf("alo");

    snpes_read(&id_clt, buffer, &msg_size);

    Serial.print("\nchegou uma mensagem do cliente id_clt, com tamanho msg_size e guardada no buffer");
    snprintf(msg, MSG_BUFFER_SIZE, "%ld", int(buffer[0]));
    if(msg[0] == '1' and strlen(msg) < 2) {
      m++;
      char string[2];
      string[0] = m;
      strcpy(msg,string);
      client.publish("metal", msg);
    }
    else if(msg[0] == '2' and strlen(msg) < 2) {
      nr++;
      msg[0] = nr;
      client.publish("não-reciclável", msg);
    }
    else if(msg[0] == '3' and strlen(msg) < 2) {
      o++;
      msg[0] = o;
      client.publish("orgânico", msg);
    }
    else if(msg[0] == '4') {
      p++;
      msg[0] = p;
      client.publish("papel", msg);
    }
    else if(msg[0] == '5') {
      pl++;
      msg[0] = pl;
      client.publish("plástico", msg);
    }
    else if(msg[0] == '6') {
      v++;
      msg[0] = v;
      client.publish("vidro", msg);
    }
    else {  
      client.publish("type", msg);
    }

    for(i=0;i<(int)msg_size;i++) {
      Serial.print(buffer[i],DEC);
      Serial.print(" ");
    }

    // Serial.write(buffer,255);

  }
}

#endif  // Adicione o fechamento aqui para encerrar o #ifdef ESP2