/* 
 #####################################################################
  @title: SMART - Sistema de MonitorAmento de Ruído para smarTcities  
  @author: Maurício Moreira Neto
  @update: 23/10/2017
 #####################################################################
*/
/*
https://create.arduino.cc/projecthub/thingsboard/temperature-dashboard-using-arduino-uno-esp8266-and-mqtt-5e26eb
*/

#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>

// -----------------------------------------------------------------------------
IPAddress server(192, 168, 1, 3);
char ssid[] = "MauMauWiFiZis";           // your network SSID (name)
char pass[] = "mau19901953";           // your network password
int status = WL_IDLE_STATUS;   // the Wifi radio's status

//Definicao pinos leds
int pinoled_ver = 3;
int pinoled_ama = 4;
int pinoled_verm = 5;

//Variaveis para o calculo de dB
float tensao = 0;
int dB = 0;

//Definicao pinos sensor
int pino_analogico = A0;
int valor_A0 = 0;

// Initialize the Ethernet client object
WiFiEspClient espClient;
PubSubClient client(espClient);
// -----------------------------------------------------------------------------
void setup() {
  //Define pinos led como saida
  pinMode(pinoled_ver, OUTPUT);
  pinMode(pinoled_ama, OUTPUT);
  pinMode(pinoled_verm, OUTPUT);
  
  //Define pinos sensor como entrada
  pinMode(pino_analogico, INPUT);
  
  // initialize serial for debugging
  Serial.begin(9600);
  
  // initialize serial for ESP module
  Serial1.begin(9600);
  
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  //connect to MQTT server
  client.setServer(server, 1883);
  client.setCallback(callback);
}

// -----------------------------------------------------------------------------
//print any message received for subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  
  for (int i=0;i<length;i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.println("topico");
  Serial.println(topic);
  Serial.println(" | ");
  Serial.println(message);
}

// -----------------------------------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }

  String payload = (String)pino_analogico;
  payload += pino_analogico;
  char payloadChar[payload.length()];
  payload.toCharArray(payloadChar, payload.length() + 1);
  client.publish("topicao", payloadChar);
  client.loop();
}

//void microfone(int pino_analogico) {
//  valor_A0 = analogRead(pino_analogico);
//  tensao = valor_A0/1023.0*4.53;
//  dB = 87.1*tensao - 75,4;
//  
//  // Mostrando nos valor no monitor serial 
//  Serial.print("Saida A0: ");
//  Serial.println(valor_A0);
//  Serial.print("Ruído: ");
//  Serial.println(dB);
//  Serial.println("");
//  delay(1000);
//  //digitalWrite(pinoled_ver, HIGH);
//  
//  // Threshold dos valores de ruido
//    if(valor_A0 < 0){
//      Serial.println("Erro no sensor!");
//    }
//    if (valor_A0 > 0 && valor_A0 < 300){
//      digitalWrite(pinoled_ver, HIGH);
//      digitalWrite(pinoled_ama, LOW);
//      digitalWrite(pinoled_verm, LOW);
//    }
//    if (valor_A0 > 301 && valor_A0 < 700){
//      digitalWrite(pinoled_ver, LOW);
//      digitalWrite(pinoled_ama, HIGH);
//      digitalWrite(pinoled_verm, LOW);
//    }
//    if (valor_A0 > 701  ){
//      digitalWrite(pinoled_ver, LOW);
//      digitalWrite(pinoled_ama, LOW);
//      digitalWrite(pinoled_verm, HIGH);
//    }
//}  


// -----------------------------------------------------------------------------
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect, just a name to identify the client
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("topico", payloadChar);
      // ... and resubscribe
      //client.subscribe("presence");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
