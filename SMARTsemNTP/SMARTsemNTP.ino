/** ==================================================================================
                      PROJETO SMART
    Projeto de monitoramento de ruído urbano em cidades inteligentes.
  
    @author: Maurício Moreira Neto
    @date: 10/11/2017
    Hardware utilizado: Arduino MEGA 2560 / 3xLEDs / Sensor Microfone KY038 /
    3xResistores / ESP8266 / Protoboard / Jumpers
    
   ==================================================================================== */
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <PubSubClient.h>

#define pinG 7
#define pinB 5
#define pinR 6


String topic = "SMART/NOISE";
int pinNoise = A0;
int dB = 0;
int errorMQTT = -2;

/**
  Variaveis para conexão do Arduio a rede WiFi
*/
IPAddress server(192, 168, 0, 135);
char ssid[] = "MauMauWiFiZis";           // your network SSID (name)
char pass[] = "mau19901953";             // your network password
int status = WL_IDLE_STATUS;             // the Wifi radio's status

WiFiEspClient espClient;
PubSubClient client(espClient);

/**
          Funcao de Setup
  Inicia as Seriais
  Inicia os pinos de LED e os pinos de entrada de dados 
  Inicia o módulo de comunicação WiFi e a rede
  Inicia o NTP
*/ 
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // comunicação
  pinMode(pinNoise, INPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinR, OUTPUT);
  InitWiFi();  
  client.setServer(server,1883);
}
/**
      Funcao Loop
  Verifica se houve a comunicação com a rede WiFi
  Caso não, o algoritmo fica tentando fazer a reconexão
  Pega o horário pela função checkOST()
  Capura o dado pela função getNoiseData()
*/
void loop() {
  status = WiFi.status();
  if (status != WL_CONNECTED){
      while(status != WL_CONNECTED){
          Serial.print("Tentando conexão para WPA SSID: ");
          Serial.println(ssid);
          // conectando com a rede
          status = WiFi.begin(ssid,pass);
          delay(500);
        }
        Serial.println("Conectado ao AP");
    }
    // tentando se reconectar
    if (!client.connected()){
        reconnect();
      }
      //checkOST();
      getNoiseData();
      delay(2000);
      client.loop();
}
/**
      Funcao getNoiseData()
  Envia os dados no formato JSON via MQTT para os subscribers
  Faz o calculo do Decibeis 
  Cria a lógica dos LEDs
*/
void getNoiseData(){

    Serial.println("Coletando os dados de ruído!");
    int noise = analogRead(pinNoise);
    double dB = 20 * log10(noise / 5);

    // checando se o sensor funciona
    if (isnan(noise)){
        Serial.println(" Falha na leitura do sensor de ruído KY038 ");
        return;
      }
      
      Serial.print(" Ruído:");
      Serial.print(dB);
      Serial.print(" dB ");
      
      // convertendo para string
      String noiseS = String(dB);

      // debugando mensagens
      Serial.print( "Enviando os dados de ruído : [" );
      Serial.print(noiseS);
      Serial.print( "] -> " );
      
      // JSON
      String payload = " { ";
      payload += "\"topico\":\""; payload += topic;
      payload += "\" , ";
      payload += "\"ruído\":"; payload += noiseS;
      payload += " } ";

      // Send payload
      char attributes[100];
      payload.toCharArray( attributes, 100 );
      client.publish( "SMART/NOISE", attributes );
      Serial.println( attributes );
      
      // Versão capturando o dado digital
      if (dB < 0)
      {
          Serial.println("Erro no sensor!");
          digitalWrite(pinG, 0);
          digitalWrite(pinB, 255);
          digitalWrite(pinR, 0);
      }
      if (dB >= 0 && dB <= 30)
      {
          digitalWrite(pinG, 255);
          digitalWrite(pinB, 0);
          digitalWrite(pinR, 0);
      }
      if (dB > 30)
      {
          digitalWrite(pinG, 0);
          digitalWrite(pinB, 0);
          digitalWrite(pinR, 255);
          delay(3000);
      }
}
/**
      Funcao initWiFi()
  Verifica a conexão com o módulo de comunicação WiFi
  Se conecta a rede WiFi setada nas variaveis globais
*/
void InitWiFi(){
    Serial.begin(9600);
    WiFi.init(&Serial1);
    if (WiFi.status() == WL_NO_SHIELD){
      Serial.println("modulo WiFi não conectado");

      while(true);
      }

      Serial.println("Conectando ao AP ...");
     
      while ( status != WL_CONNECTED) {
          Serial.print("Tentando conectar ao WPA SSID: ");
          Serial.println(ssid);
          
          status = WiFi.begin(ssid, pass);
          delay(500);
        }
       Serial.println("Conectando ao AP");
}
/**
      Funcao reconnect()
  Se conecta aao Broker via MQTT...
  Caso falhe, ele tentará reconexão em 5 segundos.
*/
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar com o Broker ...");
    if (client.connect("arduinoClient")) {
      Serial.println("Conectado!");
      String payload = " { ";
      payload += "\"Erro\":"; payload += errorMQTT;
      payload += " } ";
      char attributes2[100];
      payload.toCharArray( attributes2, 100 );
      client.publish( "SMART/NOISE", attributes2 );
      } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando em 5 segundos ");
      delay(5000);
    }
  }
}
