#include <SPI.h>
#include <SD.h>

#define errorPin 8
#define SAMPLE_PERIOD_SEC 10000

// set up variables using the SD utility library functions:
File myFile;

// Adafruit SD shields and modules: pin 10
const int chipSelect = 10;

int sensorPins[3] = {0,1,2};
int sensorValues[3];
unsigned long previousTime = 0;


void setup() {
  pinMode(errorPin,OUTPUT);

  previousTime = millis();

  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1)
    {
      showError(errorPin,250);
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
  } else {
    while (1)
    {
      showError(errorPin,100);
    }
  }
}

void loop() {
  
  if (millis() - previousTime >= SAMPLE_PERIOD_SEC)
  {
    for (int i = 0; i < 3; i++)
    {
      sensorValues[i] = analogRead(sensorPins[i]);
    }

    for(int i = 0; i < 2; i++)
    {
      Serial.print(sensorValues[i]); Serial.print(" ");
    }
    Serial.println(sensorValues[2]);
    previousTime = millis();
  }

}



void showError(int Pin, int speed)
{
  digitalWrite(Pin,HIGH);
  delay(speed);
  digitalWrite(Pin,LOW);
  delay(speed);
}
