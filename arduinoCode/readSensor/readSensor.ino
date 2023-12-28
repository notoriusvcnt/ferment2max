#define ANALOG_PIN 0
#define SAMPLING_PERIOD_MS 25
int analogValue;
float meanValue;
unsigned long previousTime;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  if (millis() - previousTime >= SAMPLING_PERIOD_MS)
  {
    
    analogValue = analogRead(ANALOG_PIN);
    meanValue = movingAverage(analogValue);
    //Serial.print(0); Serial.print(" "); Serial.print(1023); Serial.print(" ");
    //Serial.print(analogValue); Serial.print(" "); Serial.println(meanValue);
    writeToSerial10Bits(analogValue);
    previousTime = millis();
  }


}
float movingAverage(int value)
{
  const int kPoints = 8;
  static int buffer[kPoints];
  static int index;

  if (index >= kPoints)
  {
    index = 0;
  } 
  buffer[index] = value;
  index++;

  int sum = 0;

  for (int i = 0; i < kPoints; i++)
  {
    sum += buffer[i];
  }

  return sum/kPoints;  
}

void writeToSerial10Bits(int value)
{
  int A_5bits = value >> 5; //5 bits
  int B_5bits = value % 32; //5 bits

  Serial.write(255);
  Serial.write(A_5bits);
  Serial.write(B_5bits);

}
