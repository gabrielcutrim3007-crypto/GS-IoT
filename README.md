# GS-IoT

Nome dos integrantes: Gabriel Cutrim, RM: 568059. 

Link para o Wokwi: https://wokwi.com/projects/448001382191458305

## 🧠 Visão Geral do Projeto

O SmartWork Monitor IoT é um sistema desenvolvido com ESP32, sensores e comunicação MQTT para monitorar condições de trabalho e promover bem-estar, ergonomia e eficiência — alinhado ao tema “O Futuro do Trabalho”.

A solução avalia em tempo real:

🌡️ Temperatura e Umidade (DHT22)

💡 Nível de Luminosidade (LDR)

📏 Postura / Distância do usuário (Ultrassônico HC-SR04)

🚨 Alertas inteligentes com LED e Buzzer

☁️ Envio de dados para MQTT para integração com dashboards e aplicações IoT

## 🎯 Problema Identificado

O futuro do trabalho exige ambientes:

mais seguros

mais inteligentes

mais ergonômicos

Profissionais que trabalham em ambientes híbridos e digitais frequentemente enfrentam:

postura inadequada

iluminação insuficiente

alta temperatura ou baixa umidade

falta de pausas inteligentes

Esses fatores reduzem a produtividade e aumentam o risco de doenças ocupacionais.

## 💡 Solução Proposta

O sistema criado monitora continuamente o ambiente e o comportamento do trabalhador, gerando alertas quando:

Temperatura > 28°C

Umidade < 30%

Luminosidade < 300 (ambiente escuro)

Distância/postura > 60 cm

Condições inadequadas → LEDs + Buzzer + envio MQTT

O ESP32 envia dados IoT para um broker MQTT, permitindo a construção de dashboards em tempo real, automações e relatórios.

## 📥 Como Executar o Projeto
✔ Pré-requisitos

Arduino IDE ou PlatformIO

Extensões instaladas:

ESP32 Board

Biblioteca PubSubClient

Biblioteca DHT sensor library

✔ Passo a Passo

Abra o projeto no Wokwi ou IDE.

Conecte o ESP32 à rede simulada:

SSID: Wokwi-GUEST

Senha: (vazia)

Rode a simulação.

Abra um cliente MQTT e assine: smartwork/#

Observe no terminal os alertas do sistema.

Ajuste luz, temperatura ou postura para testar os sensores.

## 🧩 Lógica de Funcionamento

O ESP32 conecta ao Wi-Fi.

Se conecta ao broker MQTT.

Lê os sensores:

DHT22 → temperatura + umidade

LDR → luz

HC-SR04 → distância/postura

Publica os valores via MQTT.

Ativa LEDs e buzzer se houver risco:

Ambiente quente/seco

Pouca luz

Postura incorreta

Repete a cada 1 segundo.

## 📘 Dependências

O projeto utiliza as seguintes bibliotecas:

WiFi.h           → conexão Wi-Fi do ESP32
PubSubClient.h   → comunicação MQTT
DHT.h            → leitura do sensor de temperatura/umidade
