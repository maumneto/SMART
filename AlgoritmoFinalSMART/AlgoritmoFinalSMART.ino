#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>

#define pinG 5
#define pinY 6
#define pinR 7

String topic = "mamada/fodona";
int pinNoise = 3;
//float  tensao = 0;
int dB = 0;


IPAddress server(192, 168, 0, 69);
char ssid[] = "MauMauWiFiZis";           // your network SSID (name)
char pass[] = "mau19901953";             // your network password
int status = WL_IDLE_STATUS;             // the Wifi radio's status

WiFiEspClient espClient;
PubSubClient client(espClient);

// Setup
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(pinNoise, INPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinY, OUTPUT);
  pinMode(pinR, OUTPUT);
  InitWiFi();
  client.setServer(server,1883);
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
      getNoiseData();
      delay(3000);
      client.loop();
}
// getNoiseData
void getNoiseData(){
    Serial.println("Coletando os dados de ruído!");
   // float noise = pinNoise;
    int noise = digitalRead(pinNoise);
    double dB = 20 * log10(noise / 5);
    //tensao = noise/1023.0*4.53;
    //dB = 87.1*tensao - 75,4; 
    
    // checando se o sensor funciona
    if (isnan(noise)){
        Serial.println(" Falha na leitura do sensor de ruído KY038 ");
        return;
      }
      
      Serial.print(" Ruído:");
      Serial.print(noise);
      Serial.print(" dB ");
      
      // convertendo para string
      String noiseS = String(noise);

      // debugando mensagens
      Serial.print( "Enviando os dados de ruído : [" );
      Serial.print(noiseS);
      Serial.print( "] -> " );
      
      // JSON
      String payload = " { ";
      payload += "\"topico\":"; payload += topic;
      payload += " | ";
      payload += "\"ruido\":"; payload += noiseS;
      payload += " dB ";
      payload += " } ";

      // Send payload
      char attributes[100];
      payload.toCharArray( attributes, 100 );
      client.publish( "mamada/fodona", attributes );
      Serial.println( attributes );

      if(dB < 0){
        Serial.println("Erro no sensor!");
      }
      if (dB > 0 && dB < 20){
        digitalWrite(pinG, HIGH);
        digitalWrite(pinY, LOW);
        digitalWrite(pinR, LOW);
      }
      if (dB > 21 && dB < 40){
        digitalWrite(pinG, LOW);
        digitalWrite(pinY, HIGH);
        digitalWrite(pinR, LOW);
      }
      if (dB > 41  ){
        digitalWrite(pinG, LOW);
        digitalWrite(pinY, LOW);
        digitalWrite(pinR, HIGH);
      }
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








