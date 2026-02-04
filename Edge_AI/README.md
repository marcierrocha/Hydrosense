# Hydrosense - Edge AI 🌊🧠

Sistema de análise inteligente de qualidade da água usando TensorFlow Lite no ESP32.

## 📋 Ficheiros do Projeto

```
Edge AI/
├── modelo_treino.py          # Script de treino do modelo
├── converter_para_tflite.py  # Conversor para TFLite
├── teste_modelo_tflite.py    # Testes de validação
├── modelo.tflite             # Modelo TensorFlow Lite (1.8KB)
├──modelo_tflite.h           # Modelo em array C para ESP32
└── esp32_edge_ai.ino         # Código principal ESP32 com IA
```

## 🚀 Início Rápido

### Opção 1: Usar SEM TensorFlow Lite (Recomendado para começar)

O código `esp32_edge_ai.ino` já funciona **imediatamente** com uma versão fallback (simulação da IA). Ideal para testar antes de instalar bibliotecas.

**Passos:**
1. Abra `esp32_edge_ai.ino` no Arduino IDE
2. Conecte seu ESP32
3. Faça upload do código
4. Acesse `http://192.168.1.237` no navegador

✅ **Pronto!** O sistema já exibirá análise de qualidade da água.

### Opção 2: Usar COM TensorFlow Lite (IA Real)

Para usar o modelo TensorFlow Lite real no ESP32:

#### 1️⃣ Instalar Biblioteca

**Arduino IDE:**
```
Sketch → Include Library → Manage Libraries
Pesquisar: "TensorFlow Lite"
Instalar: "Arduino_TensorFlowLite" ou "TensorFlowLite_ESP32"
```

**PlatformIO:**
Adicione ao `platformio.ini`:
```ini
lib_deps =
    DHT sensor library
    tflite-micro
```

#### 2️⃣ Descomentar Código TFLite

No ficheiro `esp32_edge_ai.ino`, descomente:

1. **Includes (linhas ~27-32)**:
```cpp
#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
// ... etc
#include "modelo_tflite.h"
```

2. **Variáveis globais (linhas ~70-80)**:
```cpp
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  // ... etc
}
```

3. **Função `analisarQualidadeAgua()` (linhas ~120-145)**: Use a versão com TensorFlow

4. **Função `setupTensorFlowLite()` (linhas ~190-220)**: Descomente todo o código

#### 3️⃣ Copiar Ficheiro do Modelo

Copie `modelo_tflite.h` para a mesma pasta do `.ino`:
```bash
cp modelo_tflite.h ~/Arduino/esp32_edge_ai/
```

#### 4️⃣ Recompilar e Upload

Compile e faça upload para o ESP32. No Serial Monitor verá:
```
✅ Modelo TFLite carregado
✅ TensorFlow Lite inicializado com sucesso!
   Memória usada: 1856 bytes de 2048
```

## 📊 Como Funciona

### 1. Modelo de IA

O modelo foi treinado para classificar água baseado em:
- **Entrada**: Temperatura (°C) e Umidade (%)
- **Saída**: Score de 0 a 1
  - `0.0 - 0.3` = 🟢 Excelente
  - `0.3 - 0.5` = 🟡 Boa
  - `0.5 - 0.7` = 🟠 Moderada
  - `0.7 - 1.0` = 🔴 Baixa

### 2. Fluxo de Dados

```
DHT11 Sensor → Leitura (5s) → Normalização → Modelo IA → Classificação → Dashboard Web
```

### 3. API REST

**Endpoint**: `GET /api/sensor-data`

**Resposta:**
```json
{
  "temperatura": 23.5,
  "umidade": 65.0,
  "pH": 6.8,
  "turbidez": 1.2,
  "ultimaAtualizacao": "Hoje às 09:45",
  "qualidadeIA": {
    "score": 0.2345,
    "classificacao": "Excelente",
    "confianca": "Alta"
  }
}
```

## 🔧 Configuração

### Hardware

| Componente | GPIO | Descrição |
|------------|------|-----------|
| DHT11 Data | 4    | Sensor de temperatura/umidade |
| DHT11 VCC  | 3.3V | Alimentação |
| DHT11 GND  | GND  | Terra |

### Wi-Fi AP

- **SSID**: `AquaAnalyzer_AP`
- **Senha**: `12345678`
- **IP**: `192.168.1.237`

Para alterar, edite as linhas 35-37 em `esp32_edge_ai.ino`.

## 🧪 Teste do Modelo (Python)

Para testar o modelo antes de usar no ESP32:

```bash
cd "Edge AI"
source ../hydrosense/.venv/bin/activate
python teste_modelo_tflite.py
```

**Output esperado:**
```
📊 Tamanho do modelo: 1816 bytes (1.77 KB)
✅ Modelo compatível com ESP32: Sim

🧪 Testes de Inferência:

Temp: 22°C, Umid: 60%
   Água fria, umidade média
   Score: 0.4126 → 🟡 Boa

Temp: 35°C, Umid: 20%
   Condições extremas
   Score: 0.5697 → 🟠 Moderada

✅ Teste concluído com sucesso!
```

## 📝 Retreinar o Modelo

O modelo atual foi treinado com apenas 6 amostras de exemplo. Para melhor precisão:

### 1. Coletar Dados Reais

Modifique `modelo_treino.py` com seus próprios dados:

```python
X = np.array([
    [temp1, umid1],  # Suas medições reais
    [temp2, umid2],
    # ... mais amostras
], dtype=np.float32)

y = np.array([
    0,  # 0 = boa qualidade
    1,  # 1 = baixa qualidade
    # ... labels correspondentes
], dtype=np.float32)
```

### 2. Retreinar

```bash
python modelo_treino.py
python converter_para_tflite.py
xxd -i modelo.tflite > modelo_tflite.h
```

### 3. Recarregar no ESP32

Copie o novo `modelo_tflite.h` e faça upload novamente.

## 🐛 Troubleshooting

### ❌ "Falha na leitura do DHT11"
- Verifique conexões do sensor
- Adicione resistor pull-up de 10kΩ no pino de dados
- Aguarde 2 segundos após inicialização

### ❌ "Erro na inferência!"
- Verifique se `modelo_tflite.h` está na pasta correta
- Confirme que a biblioteca TFLite está instalada
- Aumente `kTensorArenaSize` se houver erro de memória

### ❌ "Versão do modelo incompatível"
- Reconverta o modelo com a mesma versão do TensorFlow
- Use TensorFlow Lite Micro compatível com ESP32

### ⚠️ Dashboard não carrega
- Conecte ao Wi-Fi `AquaAnalyzer_AP`
- Use exatamente o IP `http://192.168.1.237`
- Verifique o Serial Monitor para confirmar que o servidor iniciou

## 📈 Próximos Passos

1. **Adicionar Sensores Reais**:
   - Sensor de pH (ex: Atlas Scientific)
   - Sensor de turbidez (ex: DFRobot SEN0189)

2. **Melhorar Modelo**:
   - Coletar dataset maior (>100 amostras)
   - Adicionar mais features (pH, turbidez, condutividade)
   - Usar arquiteturas mais complexas

3. **Cloud Integration**:
   - Enviar dados para servidor via MQTT
   - Dashboard cloud (Blynk, ThingSpeak)
   - Alertas por e-mail/SMS

4. **Otimizações**:
   - Quantização do modelo (int8)
   - Deep sleep para economizar bateria
   - OTA (Over-The-Air) updates

## 📄 Licença

Este projeto faz parte do Hydrosense, desenvolvido por estudantes do Instituto Médio Privado Politécnico do Huambo.

## 👥 Autores

- Amorim Caculo
- Marcier Rocha  
- Onezimo Cambuta

---

**💡 Dica**: Comece com a versão fallback, teste tudo, e só depois ative o TensorFlow Lite real!
