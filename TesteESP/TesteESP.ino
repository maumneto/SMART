 
#define DEBUG true
 
void setup()
{
  Serial.begin(9600);
  // Configure na linha abaixo a velocidade inicial do
  // modulo ESP8266 normalmente funciona a 115200
  Serial1.begin(9600);//mudar baudrate
  sendData("AT+RST\r\n", 2000, DEBUG);
  delay(1000);
  Serial.println("Versao de firmware");
  delay(3000);
  sendData("AT\r\n", 2000, DEBUG); // rst
  sendData("AT+GMR\r\n", 2000, DEBUG); // rst
  // Configure na linha abaixo a velocidade desejada para a
  // comunicação do modulo ESP8266 (9600, 19200, 38400, etc)
  sendData("AT+CIOBAUD=9600\r\n", 2000, DEBUG);
  Serial.println("** Final **");
}
 
void loop() {
//  String response="";
//  long int time = millis();
//  long int timeout = 2000;
//  while ( (time + timeout) > millis())
//  {
//    while (Serial.available())
//    {
//      char c = Serial.read(); // ler próximo dado.
//      response += c;
//    }
//  }
//  if (DEBUG)
//  {
//    sendData(response,timeout,DEBUG);
//  }
//  
  }
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read(); // ler próximo dado.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
