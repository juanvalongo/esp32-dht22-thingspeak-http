# 🌡️ Monitor de Depósito IoT (ESP32 + ThingSpeak)

[![Wokwi](https://img.shields.io/badge/Simulação-Wokwi-orange?logo=arduino)](https://wokwi.com/projects/480126608498479105)
[![Status](https://img.shields.io/badge/Status-Concluído-success)]()

## 📖 Sobre o Projeto
Sistema de monitoramento ambiental desenvolvido para o Desafio Final de IoT da Faculdade Senac. O projeto utiliza um ESP32 e um sensor DHT22 para medir temperatura e umidade em tempo real, acionando um alarme local (com lógica de histerese) e enviando telemetria para a nuvem via protocolo HTTP.

## 📂 Estrutura dos Arquivos
O repositório contém os seguintes arquivos essenciais para a replicação do projeto:

- **`sketch.ino`**: Código-fonte principal em C++ (Arduino). Contém a lógica de leitura do sensor, histerese do alarme e comunicação HTTP com o ThingSpeak.
- **`diagram.json`**: Arquivo de configuração do circuito do Wokwi. Permite visualizar e simular as conexões físicas (ESP32, DHT22, LED, Buzzer e Resistor).
- **`libraries.txt`**: Lista de dependências necessárias para compilar o projeto (ex: `DHT sensor library for ESPx`, `ThingSpeak`).

## 🚀 Como Executar o Projeto

### Opção 1: Simulação Online (Recomendado)
A maneira mais rápida de ver o projeto em funcionamento é através do simulador Wokwi:
1. Acesse o projeto neste link: **[Simulação no Wokwi](https://wokwi.com/projects/480126608498479105)**
2. Abra o **Monitor Serial** na parte inferior da tela.
3. Clique no sensor DHT22 e ajuste o slider de temperatura para testar os cenários (Normal, Alarme e Volta ao Normal).
4. Observe os códigos `HTTP=200` no terminal e a atualização dos gráficos.

### Opção 2: Execução Local (Placa Real ESP32)
Se desejar compilar e fazer o deploy em uma placa física:
1. Instale a [Arduino IDE](https://www.arduino.cc/en/software) e o suporte à placa ESP32.
2. Instale as bibliotecas listadas no arquivo `libraries.txt` pelo Gerenciador de Bibliotecas da IDE.
3. Abra o arquivo `sketch.ino`.
4. **⚠️ IMPORTANTE:** No bloco `CONFIGURAÇÃO`, substitua os placeholders pelas suas credenciais reais:
   ```cpp
   const char* MATRICULA     = "SUA_MATRICULA";
   unsigned long CANAL_ID    = SEU_CHANNEL_ID;      // Ex: 2456789
   const char* CHAVE_ESCRITA = "SUA_WRITE_API_KEY"; // Ex: ABC123XYZ
   const int LIMITE_TEMP     = 28;                  // Entre 26 e 32 °C
    ```
5. Conecte o hardware conforme o `diagram.json` e faça o upload do código.

## 🏗️ Arquitetura do Sistema

O sistema opera em 3 camadas conceituais:

1. **Percepção:** ESP32 + DHT22 (coleta de dados) + Atuadores (LED/Buzzer).
2. **Rede:** Conexão Wi-Fi local + Protocolo HTTP (Requisições POST RESTful).
3. **Aplicação:** Servidor ThingSpeak (armazenamento time-series, dashboard e alertas de status).

---

## 🔒 Segurança e Boas Práticas

- **Gestão de Segredos:** Este repositório não contém chaves de API, IDs de canal ou senhas de Wi-Fi reais. Em um ambiente de produção, essas credenciais devem ser injetadas via variáveis de ambiente ou um Secrets Manager, nunca *hardcoded*.
- **Criptografia:** O protocolo HTTP foi utilizado para fins didáticos e de simplicidade no plano gratuito. Para ambientes de produção, a migração para HTTPS (`WiFiClientSecure`) é fortemente recomendada para garantir a confidencialidade dos dados em trânsito.
- **Rate Limiting:** O firmware respeita rigorosamente o intervalo mínimo de 15–20 segundos entre envios, conforme as limitações do plano gratuito do ThingSpeak, evitando bloqueios de IP por *flooding*.

---

## 🎓 Contexto Acadêmico

- **Disciplina:** Internet das Coisas (IoT)
- **Instituição:** Faculdade Senac (Compet Superior)
- **Desafio:** Monitor do Depósito (Caminho A: HTTP no ThingSpeak)   