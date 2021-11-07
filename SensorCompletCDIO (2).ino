#include <Wire.h>      // Librería para usar el BUS I2C 

#include <Adafruit_ADS1X15.h> // Librería del ADS1115 

  

//---> Incluir el constructor del ads1115 

Adafruit_ADS1115 ads1115; 

  

//--->  Incluir las variables de calibración del sensor de salinidad 

const int valorSin = 16300;  

const int valorSal = 22200;   

int power_pin = 5;

//--->  Incluir las variables de calibración del sensor de humedad 

const int AirValue = 30218; 

const int WaterValue = 17850; 

// ---> Incluimos las variables del calculo de la temperatura.

float b = 0.79;

float m = 0.035;

//Contador para deepsleep

const int sleepTimeS = 30;

int cont=0;

void setup() { 

  

  Serial.begin(9600); 

  Serial.println("Inicializando..."); 

  ads1115.begin(); //Initializa el ads1115 

   

   // Configurar power_pin como pin de salida (Para sensor de salinidad) 

   pinMode(power_pin, OUTPUT); 

  

  Serial.println("Ajustando la ganancia..."); 

  ads1115.setGain(GAIN_ONE); 

   

  // Configurar power_pin como pin de salida (Para sensor de salinidad) 

   pinMode(power_pin, OUTPUT); 

  

} 

  

int funciónSalinidad (){ 

  

   //--->  Variables del programa 

  int16_t salinidad; 

  int16_t adc1; 

  

  //---> Poner power_pin a nivel alto 

  digitalWrite(power_pin, HIGH); 

  

  //--->  Esperar 100ms 

delay(100); 

  

  //--->  Muestrear la tensión del sensor de salinidad 

  salinidad = analogRead( power_pin ); 

  //--->  Poner power_pin a nivel bajo 

  digitalWrite(power_pin,LOW); 

  

adc1 = ads1115.readADC_SingleEnded(1); 

salinidad = 100*valorSin/(valorSin-valorSal)-adc1*100/(valorSin-valorSal); 

  

if (salinidad>=100){ 

    salinidad=100; 

  } 

  if (salinidad<=0){  

    salinidad=0; 

  } 

return salinidad; 

} 

  

int16_t funciónHumedad (){ 

   

  int16_t adc0; 

int16_t humedad; 

adc0 = ads1115.readADC_SingleEnded(0); 

humedad = 100*AirValue/(AirValue-WaterValue)-adc0*100/(AirValue-WaterValue); 

  

  if (humedad>=100){ 

    humedad=100; 

  } 

  if (humedad<=0){  

    humedad=0; 

  } 

return humedad; 

} 

float funciontemperatura(){
  int16_t adc2;
  float voltios, temperatura;
 adc2=ads1115.readADC_SingleEnded(2);
 voltios = 4.096*adc2/(pow(2,15)-1); 
 temperatura = (voltios - b)/m;
  //Serial.println("ADC2: ");
  //Serial.println(adc2);
  //Serial.println("Grados(ºC): ");
  //Serial.println(temperatura);
  return temperatura;
} 

void sleep(){
  
  if (cont<20){
    cont++;
    delay(1000);
  }
  else{
    Serial.println("ESP8266 in sleep mode");
  }   
  ESP.deepSleep(sleepTimeS * 1000000);
  }
 
void loop() { 

int salinidad, humedad;
float temperatura; 

int16_t adc0; 

adc0 = ads1115.readADC_SingleEnded(0); 

int adc1; 

adc1 = ads1115.readADC_SingleEnded(1); 

int adc2; 

adc2 = ads1115.readADC_SingleEnded(2);

 //int sleep;
   

humedad=funciónHumedad (); 

salinidad=funciónSalinidad (); 

temperatura=funciontemperatura();


; 

Serial.println("ESP8266 in normal mode");


Serial.print("------------------"); 

Serial.print('\n'); 

Serial.print("AIN1: "); 

Serial.println(adc1); 

  Serial.print("Salinidad (%): "); 

  Serial.print(salinidad); 

  Serial.println("%"); 

  

Serial.print("AIN0: "); 

Serial.println(adc0); 

Serial.print("Humedad (%): "); 

Serial.print(humedad); 

Serial.println("%"); 


Serial.println("ADC2: ");

Serial.println(adc2);

Serial.println("Grados(ºC): ");

Serial.println(temperatura);

Serial.print("------------------"); 

sleep();
  
  delay(5000); 

}
