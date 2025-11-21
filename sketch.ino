#include <WiFi.h>                    // Biblioteca responsável pela conexão Wi-Fi do ESP32
#include <PubSubClient.h>            // Biblioteca para comunicação MQTT
#include "DHT.h"                     // Biblioteca do sensor DHT (temperatura e umidade)

// Definições do pino e tipo do sensor DHT
#define DHTPIN 15                    // Pino onde o DHT22 está conectado
#define DHTTYPE DHT22                // Define o tipo do sensor (DHT22)
DHT dht(DHTPIN, DHTTYPE);            // Cria um objeto do tipo DHT para leitura

// Definições dos sensores
#define LDRPIN 34                    // Pino analógico do sensor LDR (luminosidade)
#define TRIG 5                       // Pino trigger do sensor ultrassônico
#define ECHO 18                      // Pino echo do sensor ultrassônico

// LEDs e atuadores
#define LED_TEMP 26                  // LED indicador de alerta de temperatura/umidade
#define LED_LUZ 27                   // LED indicador de baixa luminosidade
#define LED_POSTURA 14               // LED indicador de postura incorreta
#define BUZZER 12                    // Buzzer para alertas sonoros

// Parâmetros de conexão Wi-Fi
const char* ssid = "Wokwi-GUEST";    // Nome da rede Wi-Fi do Wokwi
const char* password = "";           // Senha (fica vazia no Wokwi)

// Parâmetros do broker MQTT
const char* mqttServer = "test.mosquitto.org";  // Endereço do servidor MQTT
WiFiClient espClient;                // Cria cliente Wi-Fi
PubSubClient client(espClient);      // Cria cliente MQTT usando a conexão Wi-Fi

void setupWiFi() {
  WiFi.begin(ssid, password);
  int tentativas = 0;

  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    tentativas++;
    Serial.println("Tentando conectar ao WiFi...");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi conectado!");
  } else {
    Serial.println("Falha ao conectar. Tentando novamente...");
    ESP.restart();   // reinicia o ESP32
  }
}


// Função para reconectar ao MQTT caso desconecte
void reconnectMQTT() {
  while (!client.connected()) {       // Enquanto não estiver conectado...
    client.connect("SmartWorkDevice"); // Tenta conectar e registra nome do dispositivo
    delay(500);                       // Aguarda meio segundo
  }
}

void setup() {
  Serial.begin(115200);               // Inicializa comunicação serial para debug

  dht.begin();                        // Inicializa o sensor DHT
  pinMode(LDRPIN, INPUT);             // Define o LDR como entrada analógica
  pinMode(TRIG, OUTPUT);              // Trigger do ultrassônico como saída
  pinMode(ECHO, INPUT);               // Echo do ultrassônico como entrada

  // Define LEDs e buzzer como saídas digitais
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_LUZ, OUTPUT);
  pinMode(LED_POSTURA, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  setupWiFi();                        // Chama função para conectar ao Wi-Fi
  client.setServer(mqttServer, 1883); // Define o servidor e a porta MQTT
}

// Função para medir distância usando HC-SR04
float readDistance() {
  digitalWrite(TRIG, LOW);            // Garante que TRIG comece desligado
  delayMicroseconds(2);               // Pequeno atraso
  digitalWrite(TRIG, HIGH);           // Sinal de pulso HIGH
  delayMicroseconds(10);              // Mantém por 10 µs
  digitalWrite(TRIG, LOW);            // Finaliza pulso TRIG

  long duration = pulseIn(ECHO, HIGH); // Mede o tempo que o sinal leva para retornar
  return duration * 0.034 / 2;        // Converte tempo para distância em cm
}

void loop() {
  if (!client.connected()) reconnectMQTT();  // Reconecta ao MQTT se desconectado
  client.loop();                             // Mantém a comunicação MQTT ativa

  float temp = dht.readTemperature();        // Lê temperatura do DHT22
  float hum = dht.readHumidity();            // Lê umidade do DHT22
  int luz = analogRead(LDRPIN);              // Lê nível de luminosidade do LDR
  float dist = readDistance();               // Lê distância do ultrassônico

  // Publica dados nos tópicos MQTT
  client.publish("smartwork/ambiente/temperatura", String(temp).c_str());
  client.publish("smartwork/ambiente/umidade", String(hum).c_str());
  client.publish("smartwork/luminosidade", String(luz).c_str());
  client.publish("smartwork/postura", String(dist).c_str());

  // ----- Lógica dos Alertas -----

 bool alerta = false;

  // Temperatura
  if (temp > 28 || hum < 30) {
    digitalWrite(LED_TEMP, HIGH);
    alerta = true;
    Serial.println("Faça uma palsa!");
  } else {
    digitalWrite(LED_TEMP, LOW);
  }

  // Luz
  if (luz < 300) {
    digitalWrite(LED_LUZ, HIGH);
    alerta = true;
    Serial.println("Faça uma palsa!");
  } else {
    digitalWrite(LED_LUZ, LOW);
  }

  // Postura
  if (dist > 60) {
    digitalWrite(LED_POSTURA, HIGH);
    alerta = true;
    Serial.println("Faça uma palsa!");
  } else {
    digitalWrite(LED_POSTURA, LOW);
  }

  // BUZZER
  if (alerta) tone(BUZZER, 1000);
  else noTone(BUZZER);


  delay(1000);  // pequena pausa entre leituras
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.print(" °C | Umidade: ");
  Serial.print(hum);
  Serial.print(" % | Luz: ");
  Serial.print(luz);
  Serial.print(" | Distância: ");
  Serial.println(dist);
}
