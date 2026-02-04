#!/usr/bin/env python3
"""
Script de teste para validar o modelo TFLite
Testa a inferência com diferentes valores de temperatura e umidade
"""

import tensorflow as tf
import numpy as np
import os

def testar_modelo():
    # Carregar modelo
    model_path = "modelo.tflite"
    
    if not os.path.exists(model_path):
        print(f"❌ Erro: Ficheiro {model_path} não encontrado!")
        return
    
    # Informações do modelo
    size = os.path.getsize(model_path)
    print(f"📊 Tamanho do modelo: {size} bytes ({size/1024:.2f} KB)")
    print(f"✅ Modelo compatível com ESP32: {'Sim' if size < 50000 else 'Não (muito grande)'}\n")
    
    # Carregar interpreter
    interpreter = tf.lite.Interpreter(model_path=model_path)
    interpreter.allocate_tensors()
    
    # Detalhes do modelo
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    
    print("📋 Detalhes do Modelo:")
    print(f"   Input shape: {input_details[0]['shape']}")
    print(f"   Input type: {input_details[0]['dtype']}")
    print(f"   Output shape: {output_details[0]['shape']}")
    print(f"   Output type: {output_details[0]['dtype']}\n")
    
    # Casos de teste
    print("🧪 Testes de Inferência:\n")
    
    casos_teste = [
        {"temp": 22, "umid": 60, "desc": "Água fria, umidade média"},
        {"temp": 24, "umid": 58, "desc": "Temperatura ideal, umidade boa"},
        {"temp": 26, "umid": 55, "desc": "Temperatura morna, umidade baixa"},
        {"temp": 30, "umid": 30, "desc": "Água quente, umidade muito baixa"},
        {"temp": 32, "umid": 25, "desc": "Água muito quente, ar seco"},
        {"temp": 35, "umid": 20, "desc": "Condições extremas"},
    ]
    
    for caso in casos_teste:
        temp = caso["temp"]
        umid = caso["umid"]
        
        # Normalizar (mesma normalização do treino)
        temp_norm = temp / 50.0
        umid_norm = umid / 100.0
        
        # Preparar input
        input_data = np.array([[temp_norm, umid_norm]], dtype=np.float32)
        
        # Executar inferência
        interpreter.set_tensor(input_details[0]['index'], input_data)
        interpreter.invoke()
        output = interpreter.get_tensor(output_details[0]['index'])
        
        score = output[0][0]
        
        # Interpretar resultado
        if score < 0.3:
            classificacao = "🟢 Excelente"
            cor = "\033[92m"
        elif score < 0.5:
            classificacao = "🟡 Boa"
            cor = "\033[93m"
        elif score < 0.7:
            classificacao = "🟠 Moderada"
            cor = "\033[93m"
        else:
            classificacao = "🔴 Baixa"
            cor = "\033[91m"
        
        reset = "\033[0m"
        
        print(f"{cor}Temp: {temp}°C, Umid: {umid}%{reset}")
        print(f"   {caso['desc']}")
        print(f"   Score: {score:.4f} → {classificacao}")
        print()
    
    print("\n✅ Teste concluído com sucesso!")
    print("\n💡 Próximos passos:")
    print("   1. Ficheiro 'modelo_tflite.h' já foi gerado")
    print("   2. Use este ficheiro no código ESP32")
    print("   3. Implemente a mesma lógica de classificação no ESP32")

if __name__ == "__main__":
    testar_modelo()
