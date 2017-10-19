  
/* ########################################################
  SMART: Sistema de MonitorAmento de Ruído para smarTcities  
  Autor: Maurício Moreira Neto
  Data: 19/10/2017
 ########################################################## */

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

/*
  - Iniciando o monitor serial
  - Definindo os pinos dos LEDs
  - Definindo o pino analogico do sensor de som KY 038
*/
void setup()
{
  Serial.begin(9600);
  //Define pinos led como saida
  pinMode(pinoled_ver, OUTPUT);
  pinMode(pinoled_ama, OUTPUT);
  pinMode(pinoled_verm, OUTPUT);
  //Define pinos sensor como entrada
  pinMode(pino_analogico, INPUT);
}

/*
  - Mostrando os valores no monitor serial
  - Calculo do nivel de ruido (em dB)
  - Threshold do valor de som
*/
void loop()
{
  valor_A0 = analogRead(pino_analogico);
  tensao = valor_A0/1023.0*4.53;
  dB = 87.1*tensao - 75,4;
  
  // Mostrando nos valor no monitor serial 
  Serial.print("Saida A0: ");
  Serial.println(valor_A0);
  Serial.print("Ruído: ");
  Serial.println(dB);
  Serial.println("");
  //delay(1000);
  
  // Threshold dos valores de ruido
  if(valor_A0 < 0){
      Serial.println("Erro no sensor!");
    }
  if (valor_A0 > 0 && valor_A0 < 300){
      digitalWrite(pinoled_ver, HIGH);
      digitalWrite(pinoled_ama, LOW);
      digitalWrite(pinoled_verm, LOW);
    }
    if (valor_A0 > 300 && valor_A0 < 700){
      digitalWrite(pinoled_ver, LOW);
      digitalWrite(pinoled_ama, HIGH);
      digitalWrite(pinoled_verm, LOW);
    }
    if (valor_A0 > 701  ){
      digitalWrite(pinoled_ver, LOW);
      digitalWrite(pinoled_ama, LOW);
      digitalWrite(pinoled_verm, HIGH);
    }
}

