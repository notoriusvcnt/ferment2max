#include <SPI.h>
#include <SD.h>
#include <OneWire.h>                
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>




#define errorPin 8
#define SAMPLE_PERIOD_SEC 1000

OneWire ourWire(2);                //Se establece el pin 2  como bus OneWire
 
DallasTemperature sensors(&ourWire); //Se declara una variable u objeto para nuestro sensor
unsigned long previousMeasure = 0;
unsigned long previousSending = 0;
float temp = 0;

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //

/*
 The microSD module circuit:
    SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - pin 10 on Arduino Uno.
*/

// set up variables using the SD utility library functions:
File myFile;

// Adafruit SD shields and modules: pin 10
const int chipSelect = 10;

int sensorPins[3] = {0,1,2};
float sensorValues[4];
unsigned long previousTime = 0;


void setup() {
  pinMode(errorPin,OUTPUT);

  previousTime = millis();

  Serial.begin(9600);
  //inicializamos el sensor de temperatura
  sensors.begin();   //Se inicia el sensor
  sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
  temp = sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC

  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("SER-VIDXS");

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1)
    {
      showError(errorPin,250);
      lcd.print("error con la tarjeta.");
    }
  }

  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("DATALOG.txt", FILE_WRITE);

  if (myFile)
  {
    digitalWrite(errorPin,HIGH);
    Serial.println("File opened succesfully.");
    Serial.println("MQ-3,MQ-4,PRESION,TEMPERATURA");
    myFile.println("MQ-3,MQ-4,PRESION,TEMPERATURA");
  } else {
    Serial.println("error opening file.");
    while(1)
    {
      showError(errorPin,100);
    }
  }
}

void loop() {
  if (myFile)
  {
    if (millis() - previousTime >= SAMPLE_PERIOD_SEC)
    {
      //lectura de datos
      for (int i = 0; i < 3; i++)
      {
        sensorValues[i] = analogRead(sensorPins[i]);
      }

      //lectura de sensor de temperatura
      sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
      sensorValues[3] = sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC


      //escritura de datos a monitor serial y a tarjeta SD
      for(int i = 0; i < 3; i++)
      {
        Serial.print(sensorValues[i]); Serial.print(",");
        myFile.print(sensorValues[i]); myFile.print(",");
        lcd.setCursor(5*i,1);
        lcd.print(int(sensorValues[i]));
      }
      Serial.println(sensorValues[3]);
      myFile.println(sensorValues[3]);
      lcd.setCursor(10,0);
      lcd.print(sensorValues[3]);
      myFile.flush();
      
      //actualizamos variable temporal
      previousTime = millis();
    }
  } else {
    Serial.println("error opening file.");
    while(1)
    {
      showError(errorPin,100);
    }
  }

}

void showError(int Pin, int speed)
{
  digitalWrite(Pin,HIGH);
  delay(speed);
  digitalWrite(Pin,LOW);
  delay(speed);
}
