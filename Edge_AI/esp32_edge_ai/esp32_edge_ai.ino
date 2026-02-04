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
 *
 * Instalação do TensorFlow Lite:
 * 1. Arduino IDE: Instalar "Arduino_TensorFlowLite" via Library Manager
 * 2. PlatformIO: adicionar 'tflite-micro' ao platformio.ini
 */

#include "webpage.h"
#include <DallasTemperature.h>
#include <OneWire.h>
#include <WebServer.h>
#include <WiFi.h>

#include "modelo_tflite.h" // Array com o modelo convertido
#include <TensorFlowLite_ESP32.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

// ========== CONFIGURAÇÕES DO AP ==========
const char *ssid = "AquaAnalyzer_AP";
const char *password = "12345678";

IPAddress localIP(192, 168, 1, 237);
IPAddress gateway(192, 168, 1, 237);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

// ========== CONFIGURAÇÃO DOS SENSORES ==========

// DS18B20 (Temperatura da Água)
#define ONE_WIRE_BUS 15 // GPIO onde o DS18B20 está conectado
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Sensor de Turbidez
#define TURBIDITY_PIN 34 // GPIO Analógico para o sensor de turbidez

// ========== VARIÁVEIS DE DADOS ==========
float temperatura = 0.0;
// float umidade = 0.0;
float pH = 6.8;
float turbidez = 1.2;
String ultimaAtualizacao = "";

// Variáveis da análise IA
float iaScore = 0.0;
String iaClassificacao = "Calculando...";
String iaConfianca = "Alta";

extern const char index_html[] PROGMEM;

// ========== HISTÓRICO PARA GRÁFICOS ==========
const int historicoSize = 5;
float tempHistorico[historicoSize] = {0};
float pHHistorico[historicoSize] = {0};
float turbidezHistorico[historicoSize] = {0};
String tempoHistorico[historicoSize] = {""};
int historicoIndex = 0;
unsigned long ultimaLeitura = 0;
const unsigned long intervaloLeitura = 5000;

// ========== TENSORFLOW LITE - VARIÁVEIS GLOBAIS ==========
// Descomente após instalar a biblioteca
namespace {
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;

//   // Buffer de memória para TensorFlow Lite (2KB deve ser suficiente)
constexpr int kTensorArenaSize = 2 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
} // namespace

// ========== FUNÇÃO DE ANÁLISE IA ==========
/**
 * Analisa a qualidade da água usando o modelo TensorFlow Lite
 *
 * @param temp Temperatura em °C
 * @param umid Umidade em %
 * @return Score entre 0 e 1 (0 = Excelente, 1 = Baixa)
 */
float analisarQualidadeAgua(float temp) {
  // ========== VERSÃO COM TENSORFLOW ==========
  // Normalizar inputs
  float temp_norm = temp / 50.0;
  // float umid_norm = umid / 100.0;

  // Preencher tensor de entrada
  input->data.f[0] = temp_norm;
  // input->data.f[1] = umid_norm; // REMOVED

  // Executar inferência
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    Serial.println("❌ Erro na inferência!");
    return 0.5; // Valor médio em caso de erro
  }

  // Obter resultado
  float score = output->data.f[0];

  Serial.print("🧠 Inferência IA: Temp=");
  Serial.print(temp);
  // Serial.print("°C, Umid=");
  // Serial.print(umid);
  Serial.print("% → Score=");
  Serial.println(score, 4);

  return score;
}

/**
 * Interpreta o score da IA e retorna uma classificação
 */
String interpretarResultado(float score) {
  if (score < 0.3)
    return "Excelente";
  else if (score < 0.5)
    return "Boa";
  else if (score < 0.7)
    return "Moderada";
  else
    return "Baixa";
}

/**
 * Calcula a confiança da predição baseada no score
 */
String calcularConfianca(float score) {
  // Se o score está muito próximo dos limites (0.3, 0.5, 0.7)
  // a confiança é menor
  float distanciaLimite =
      min(min(abs(score - 0.3), abs(score - 0.5)), abs(score - 0.7));

  if (distanciaLimite > 0.15)
    return "Alta";
  else if (distanciaLimite > 0.05)
    return "Média";
  else
    return "Baixa";
}

// ========== SETUP TENSORFLOW LITE ==========
void setupTensorFlowLite() {
  // Descomente após instalar a biblioteca

  // Configurar error reporter
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Carregar modelo
  model = tflite::GetModel(modelo_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("❌ Versão do modelo incompatível!");
    return;
  }

  Serial.println("✅ Modelo TFLite carregado");

  // Configurar operações
  static tflite::AllOpsResolver resolver;

  // Criar interpreter
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Alocar memória para tensors
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    Serial.println("❌ Falha ao alocar tensors!");
    return;
  }

  // Obter ponteiros para input e output
  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("✅ TensorFlow Lite inicializado com sucesso!");
  Serial.print("   Memória usada: ");
  Serial.print(interpreter->arena_used_bytes());
  Serial.print(" bytes de ");
  Serial.println(kTensorArenaSize);
}

// ========== LEITURA DE SENSORES ==========
float lerTurbidez() {
  int sensorValue = analogRead(TURBIDITY_PIN);
  float voltage = sensorValue * (3.3 / 4095.0); // ESP32 ADC (0-3.3V)

  // Conversão simples de voltagem para NTU (ajustar conforme curva do sensor)
  // Exemplo genérico: Maior voltagem (água limpa) -> Menor NTU
  // Menor voltagem (água suja) -> Maior NTU
  // Esta fórmula precisa ser calibrada com o sensor específico!
  // Supondo: 2.5V+ = 0 NTU, <2.5V aumenta NTU

  float ntu = 0;
  if (voltage < 2.5) {
    ntu = 3000.0 * (1.0 - (voltage / 2.5)); // Exemplo grosseiro
  }

  // Como estamos usando simulação/adaptação, vamos retornar um valor baseado na
  // leitura Mas limitar para evitar negativos
  if (ntu < 0)
    ntu = 0;

  // debug
  // Serial.print("Turbidity Volt: "); Serial.println(voltage);

  return ntu;
}

void lerSensores() {
  // 1. Temperatura da Água (DS18B20)
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Verificar erro de leitura
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("❌ Erro: DS18B20 desconectado!");
    // Manter valor anterior ou usar backup do DHT se necessário
  } else {
    temperatura = tempC;
  }

  // 3. Turbidez
  turbidez = lerTurbidez();

  // 4. pH (Ainda simulado pois não foi fornecido sensor de pH no prompt)
  pH = 6.5 + (random(0, 20) / 10.0);

  // ========== ANÁLISE IA ==========
  // Usamos a temp da água E umidade ambiente?
  // O modelo original parecia usar temp e umidade. Vamos manter a lógica.
  // iaScore = analisarQualidadeAgua(temperatura, umidade);
  iaScore = analisarQualidadeAgua(temperatura);
  iaClassificacao = interpretarResultado(iaScore);
  iaConfianca = calcularConfianca(iaScore);

  // Atualizar histórico
  tempHistorico[historicoIndex] = temperatura;
  pHHistorico[historicoIndex] = pH;
  turbidezHistorico[historicoIndex] = turbidez;

  // Timestamp
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", (millis() / 3600000) % 24,
           (millis() / 60000) % 60, (millis() / 1000) % 60);
  tempoHistorico[historicoIndex] = String(buffer);

  historicoIndex = (historicoIndex + 1) % historicoSize;

  // Atualizar timestamp
  snprintf(buffer, sizeof(buffer), "Hoje às %02d:%02d",
           (millis() / 3600000) % 24, (millis() / 60000) % 60);
  ultimaAtualizacao = String(buffer);

  Serial.println("\n📊 Dados atualizados:");
  Serial.print("   Temp Água (DS18B20): ");
  Serial.print(temperatura);
  Serial.println("°C");
  // Serial.print("   Umidade Ar (DHT11): ");
  // Serial.print(umidade);
  // Serial.println("%");
  Serial.print("   pH: ");
  Serial.println(pH);
  Serial.print("   Turbidez: ");
  Serial.print(turbidez);
  Serial.println(" NTU");
  Serial.print("   🧠 IA Score: ");
  Serial.print(iaScore, 4);
  Serial.print(" → ");
  Serial.print(iaClassificacao);
  Serial.print(" (Confiança: ");
  Serial.print(iaConfianca);
  Serial.println(")");
}

// ========== ENDPOINT API - DADOS DOS SENSORES ==========
void handleSensorData() {
  String json = "{";
  json += "\"temperatura\":" + String(temperatura, 1) + ",";
  // json += "\"umidade\":" + String(umidade, 1) + ",";
  json += "\"pH\":" + String(pH, 1) + ",";
  json += "\"turbidez\":" + String(turbidez, 1) + ",";
  json += "\"ultimaAtualizacao\":\"" + ultimaAtualizacao + "\",";

  // ========== NOVO: Dados da análise IA ==========
  json += "\"qualidadeIA\":{";
  json += "\"score\":" + String(iaScore, 4) + ",";
  json += "\"classificacao\":\"" + iaClassificacao + "\",";
  json += "\"confianca\":\"" + iaConfianca + "\"";
  json += "}";

  json += "}";

  server.send(200, "application/json", json);
}

void handleRoot() { server.send_P(200, "text/html", index_html); }

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  Serial.println("\n\n🌊 Hydrosense - Sistema Edge AI");
  Serial.println("================================");

  Serial.println("✅ Sensor DS18B20 inicializado");

  // Inicializar TensorFlow Lite
  setupTensorFlowLite();

  // Inicializar histórico
  for (int i = 0; i < historicoSize; i++) {
    tempHistorico[i] = 23.0 + (i * 0.1);
    pHHistorico[i] = 6.8;
    turbidezHistorico[i] = 1.2;
    tempoHistorico[i] = String(i * 30) + "s";
  }

  // Configurar Wi-Fi AP
  WiFi.softAPConfig(localIP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.print("✅ AP criado: ");
  Serial.println(ssid);
  Serial.print("   IP: ");
  Serial.println(WiFi.softAPIP());

  // Configurar rotas
  server.on("/", handleRoot);
  server.on("/api/sensor-data", handleSensorData);

  server.begin();
  Serial.println("✅ Servidor HTTP iniciado");
  Serial.println("\n💡 Acesse: http://192.168.1.237");
  Serial.println("================================\n");

  // Primeira leitura
  lerSensores();
  ultimaLeitura = millis();
}

// ========== LOOP ==========
void loop() {
  server.handleClient();

  // Ler sensores periodicamente
  if (millis() - ultimaLeitura >= intervaloLeitura) {
    lerSensores();
    ultimaLeitura = millis();
  }

  delay(10);
}

// HTML movido para webpage.h para melhor organização
