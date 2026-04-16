/*
 * Hydrosense - Sistema de Análise de Qualidade da Água com Edge AI
 *
 * Este código integra TensorFlow Lite Micro no ESP32 para análise inteligente
 * de qualidade da água usando dados de sensores em tempo real.
 *
 * Hardware:
 * - ESP32
 * - DallasTemperature (para DS18B20)
 * - TensorFlow Lite Micro para ESP32
 * - Sensor de pH
 */

#include "webpage.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <WebServer.h>
#include <WiFi.h>

// TFLite removido

// ========== CONFIGURAÇÕES DO AP ==========
const char *ssid = "Hydrosense_AP";

IPAddress localIP(192, 168, 1, 237);
IPAddress gateway(192, 168, 1, 237);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

const byte DNS_PORT = 53;
DNSServer dnsServer;

// ========== CONFIGURAÇÃO DOS SENSORES ==========

// DS18B20 (Temperatura da Água)
#define ONE_WIRE_BUS 15 // GPIO onde o DS18B20 está conectado
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Sensor de Turbidez (Módulo Laser + LDR)
#define LDR_PIN 35
#define LASER_PIN 32 // Pino recomendado para output (Ex: 32)
#define PH_PIN 34

// Calibração do sensor DE PH
float m = -5.976;
float b = 14.853;

float readPH() {
  long soma = 0;
  for (int i = 0; i < 10; i++) {
    soma += analogRead(PH_PIN);
    delay(10);
  }
  float voltage = (soma / 10.0) * (3.3 / 4095.0);

  // Se a voltagem for quase zero (ex: < 0.1V), o sensor provavelmente está
  // desconectado
  if (voltage < 0.1) {
    return 0.0;
  }

  float ph = m * voltage + b;
  return ph;
}

// ========== VARIÁVEIS DE DADOS ==========
float temperatura = 0.0;

float pH = 0;
float turbidez = 1.2;
String ultimaAtualizacao = "";



extern const char index_html[] PROGMEM;


unsigned long ultimaLeitura = 0;
const unsigned long intervaloLeitura = 1000;



// ========== LEITURA DE SENSORES ==========
float lerTurbidez() {
  int raw = analogRead(LDR_PIN);

  // O ESP32 possui ADC de 12-bits (0-4095) e opera a 3.3V máximo de conversão
  // nativo!
  float voltage = raw * (3.3 / 4095.0);

  String qualidade;
  float ntu_estimado = 0;

  // A lógica do módulo LDR é invertida: muita luz (água limpa) = baixa tensão.
  // Verificámos que com água limpa, a tensão ronda os 0.17V.
  if (voltage >= 2.2) {
    qualidade = "MUITO TURVA"; // Bloqueio total / escuro (Tensão alta)
  } else if (voltage >= 0.8) {
    qualidade = "TURVA"; // Água suja
  } else {
    qualidade = "LIMPA"; // Água limpa (Baixa tensão, perto de 0V)
  }

  // Calibração ajustada para zerar o NTU na água limpa (0.17V medidos).
  float voltagem_ajustada = voltage - 0.17;
  if (voltagem_ajustada < 0)
    voltagem_ajustada = 0;

  // Assumimos que no escuro total chegue a ~3.0V
  // Mapeamento: 0.17V = 0 NTU | 3.0V = 100 NTU
  // 3.0V - 0.17V = 2.83V de diferença
  ntu_estimado = (voltagem_ajustada / 2.83) * 100.0;

  // Garantir que o valor fique entre 0 e 100 NTU
  if (ntu_estimado < 0.0)
    ntu_estimado = 0.0;
  if (ntu_estimado > 100.0)
    ntu_estimado = 100.0;

  Serial.print("   LDR RAW: ");
  Serial.print(raw);
  Serial.print(" | Tensão: ");
  Serial.print(voltage, 3);
  Serial.print("V | Água: ");
  Serial.println(qualidade);

  // Retornamos um número estimado para não quebrar os gráficos e lógicas
  // existentes
  return ntu_estimado;
}

void lerSensores() {
  // 1. Temperatura da Água (DS18B20)
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Verificar erro de leitura
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Erro: DS18B20 desconectado! Retornando 0.");
    temperatura = 0.0;
  } else {
    temperatura = tempC;
  }

  // 3. Turbidez
  turbidez = lerTurbidez();

  // 4. pH (Leitura real do sensor)
  pH = readPH();

  char buffer[20];

  /* Atualizar timestamp
  snprintf(buffer, sizeof(buffer), "Hoje às %02d:%02d",
           (millis() / 3600000) % 24, (millis() / 60000) % 60);
  ultimaAtualizacao = String(buffer);
*/
  Serial.println("\n Dados atualizados:");
  Serial.print("   Temp Água (DS18B20): ");
  Serial.print(temperatura);
  Serial.println("°C");
  Serial.print("   pH: ");
  Serial.println(pH);
  Serial.print("   Turbidez: ");
  Serial.print(turbidez);
  Serial.println(" NTU");

}

// ========== ENDPOINT API - DADOS DOS SENSORES ==========
void handleSensorData() {
  String json = "{";
  json += "\"temperatura\":" + String(temperatura, 1) + ",";
  json += "\"pH\":" + String(pH, 1) + ",";
  json += "\"turbidez\":" + String(turbidez, 1) + ",";
  json += "\"ultimaAtualizacao\":\"" + ultimaAtualizacao + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleRoot() { server.send_P(200, "text/html", index_html); }

// Redireciona todos os acessos desconhecidos para a página principal (Captive
// Portal)
void handleNotFound() {
  server.sendHeader("Location", "http://192.168.1.237/", true);
  server.send(302, "text/plain", "Redirecionando para a Dashboard...");
}

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  Serial.println("\n\n Hydrosense - Sistema Edge AI");
  Serial.println("================================");

  sensors.begin();
  Serial.println("Sensor DS18B20 inicializado");

  // Inicializar o Módulo Laser
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, HIGH);
  Serial.println("Módulo Laser Inicializado");



  // Configurar Wi-Fi AP (Aberta / Sem Senha)
  WiFi.softAPConfig(localIP, gateway, subnet);
  WiFi.softAP(ssid);

  Serial.print("AP criado: ");
  Serial.println(ssid);
  Serial.print("   IP: ");
  Serial.println(WiFi.softAPIP());

  // Iniciar DNS Server (Captive Portal) - Roteia tudo para o IP local
  dnsServer.start(DNS_PORT, "*", localIP);
  Serial.println("Portal Cativo (DNS) iniciado");

  // Configurar rotas
  server.on("/", handleRoot);
  server.on("/api/sensor-data", handleSensorData);
  server.onNotFound(handleNotFound); // Interceta e redireciona

  server.begin();
  Serial.println("Servidor HTTP iniciado");
  Serial.println("\n Acesse: http://192.168.1.237");
  Serial.println("================================\n");

  // Primeira leitura
  lerSensores();
  ultimaLeitura = millis();
}

// ========== LOOP ==========
void loop() {
  // Lidar com requisições DNS (Captive Portal) e Web
  dnsServer.processNextRequest();
  server.handleClient();

  // Ler sensores periodicamente
  if (millis() - ultimaLeitura >= intervaloLeitura) {
    lerSensores();
    ultimaLeitura = millis();
  }

  delay(1);
}

// HTML movido para webpage.h para melhor organização
