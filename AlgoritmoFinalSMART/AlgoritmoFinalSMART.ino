#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>

#define pinG 5
#define pinY 6
#define pinR 7

#define pinNoise A0

IPAddress server(192, 168, 1, 3);
char ssid[] = "MauMauWiFiZis";           // your network SSID (name)
char pass[] = "mau19901953";             // your network password
int status = WL_IDLE_STATUS;             // the Wifi radio's status

WiFiEspClient espClient;
PubSubClient client(espClient);
unsigned long lastSend;
// Setup
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  InitWiFi();
  client.setServer(server,1883);
  lastSend = 0;
}
// Loop
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
    if (millis() - lastSend > 1000){
      getNoiseData();
      lastSend = millis();
      }
      client.loop();
}
// getNoiseData
void getNoiseData(){
    Serial.println("Coletando os dados de ruído!");

    float noise = pinNoise;
    // checando se o sensor funciona
    if (isnan(noise)){
        Serial.println("Falha na leitura do sensor de ruído KY038");
        return;
      }
      Serial.print("Ruído:");
      Serial.print(noise);
      Serial.print("dB");
      
      // convertendo para string
      String noiseS = String(noise);

        // debugando mensagens
      Serial.print( "Enviando os dados de ruído : [" );
      Serial.print(noiseS);
      Serial.print( "] -> " );
      // JSON
      String payload = "{";
      payload += "\"ruido\":"; payload += noiseS;
      payload += "}";

      // Send payload
      char attributes[100];
      payload.toCharArray( attributes, 100 );
      client.publish( "mamada/fodona", attributes );
      Serial.println( attributes );
}
// initWiFi  
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
// reconnect
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar com o MQTT ...");
    if (client.connect("arduinoClient")) {
      Serial.println("Conectado!");
      } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando em 5 segundos ");
      delay(5000);
    }
  }
}








