/*
  DESAFIO FINAL IoT - Monitor do Depósito (Caminho A - HTTP)
  Baseado na Base 2 do Prof. Arnott
*/

#include <WiFi.h>
#include "DHTesp.h"
#include "ThingSpeak.h"   // sempre depois de WiFi.h

// ===================== CONFIGURAÇÃO =====================
const char*   MATRICULA     = "000000";     // <-- Coloque sua matrícula aqui
unsigned long CANAL_ID      = 0;            // <-- Número do seu canal (Channel ID)
const char*   CHAVE_ESCRITA = "SUA_CHAVE";  // <-- Write API Key (NUNCA salve a real no Wokwi público!)
const int     LIMITE_TEMP   = 28;           // <-- Escolha entre 26 e 32 °C (Regra do Desafio)
// ========================================================

const unsigned long INTERVALO_ENVIO_MS = 20000; // 20 segundos (Regra do ThingSpeak gratuito)

const int PINO_DHT    = 15;
const int PINO_LED    = 2;
const int PINO_BUZZER = 4;

DHTesp     dht;
WiFiClient cliente;
int  limite;
bool alarme = false;
unsigned long ultimaLeitura = 0;
unsigned long ultimoEnvio   = 0;
float temperatura = 0, umidade = 0;

// Regra resolvida da Base 1: liga acima do limite e só desliga abaixo de (limite - 1).
bool decidirAlarme(float t, bool alarmeAtual) {
  if (!alarmeAtual && t > limite)      return true;
  if (alarmeAtual  && t < limite - 1)  return false;
  return alarmeAtual;
}

void atualizarSaidas(bool ligado) {
  digitalWrite(PINO_LED, ligado ? HIGH : LOW);
  if (ligado) tone(PINO_BUZZER, 1000, 200);
}

void conectarWiFi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" conectado!");
}

void enviarParaNuvem() {
  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, umidade);
  
  // RF4: Envia Field 3 = 1 (ALERTA) ou 0 (NORMAL)
  if (alarme) {
    ThingSpeak.setField(3, 1);
    ThingSpeak.setStatus("ALERTA");
  } else {
    ThingSpeak.setField(3, 0);
    ThingSpeak.setStatus("NORMAL");
  }

  int codigo = ThingSpeak.writeFields(CANAL_ID, CHAVE_ESCRITA);
  Serial.printf("COMPROVANTE A2 | mat=%s | limite=%d | canal=%lu | intervalo=%lus | HTTP=%d\n",
                MATRICULA, limite, CANAL_ID, INTERVALO_ENVIO_MS / 1000, codigo);
}

void setup() {
  Serial.begin(115200);
  pinMode(PINO_LED, OUTPUT);
  dht.setup(PINO_DHT, DHTesp::DHT22);
  
  limite = LIMITE_TEMP; // Usando o limite fixo escolhido
  
  Serial.printf("DESAFIO FINAL | matricula=%s | limite=%d C\n", MATRICULA, limite);
  conectarWiFi();
  ThingSpeak.begin(cliente);
}

void loop() {
  unsigned long agora = millis();

  // RF1: Leitura a cada 2s com tratamento de erro
  if (agora - ultimaLeitura >= 20000) {
    ultimaLeitura = agora;
    TempAndHumidity leitura = dht.getTempAndHumidity();
    if (dht.getStatus() == DHTesp::ERROR_NONE) {
      temperatura = leitura.temperature;
      umidade     = leitura.humidity;
      alarme = decidirAlarme(temperatura, alarme);
      atualizarSaidas(alarme);
      Serial.printf("T=%.1f C | H=%.0f %% | alarme=%s\n",
                    temperatura, umidade, alarme ? "LIGADO" : "desligado");
    } else {
      Serial.println("Erro na leitura do DHT22. Ignorando...");
    }
  }

  // RF3: Envio para a nuvem a cada 20s
  if (agora - ultimoEnvio >= INTERVALO_ENVIO_MS) {
    ultimoEnvio = agora;
    enviarParaNuvem();
  }
}