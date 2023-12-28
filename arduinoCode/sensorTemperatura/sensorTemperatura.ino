#include <OneWire.h>                
#include <DallasTemperature.h>

#define SAMPLING_PERIOD_MS 1000
#define SENDING_PERIOD_MS 300
OneWire ourWire(2);                //Se establece el pin 2  como bus OneWire
 
DallasTemperature sensors(&ourWire); //Se declara una variable u objeto para nuestro sensor
unsigned long previousMeasure = 0;
unsigned long previousSending = 0;
float temp = 0;

void setup() {
  delay(1000);
  Serial.begin(9600);
  sensors.begin();   //Se inicia el sensor
  sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
  temp = sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC
  previousMeasure = millis();
  previousSending = millis();
}
 
void loop() {
  if (millis() - previousMeasure >=  SAMPLING_PERIOD_MS)
  {
    sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
    temp = sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC

    //Serial.print("Temperatura= ");
    //Serial.println(temp);
    //Serial.println(" C");
    previousMeasure = millis();
  }

  if (millis() - previousSending >=  SENDING_PERIOD_MS)
  {
    Serial.println(temp); 
    previousSending = millis();
  }               
}