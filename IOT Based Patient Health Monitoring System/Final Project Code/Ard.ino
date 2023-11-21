#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "DHT.h"
DHT dht;

#define USE_ARDUINO_INTERRUPTS  true
#include <PulseSensorPlayground.h>

SoftwareSerial esp(10, 11);
LiquidCrystal  lcd(7, 6, 5, 4, 3, 2);


int temp = 25, buzzer = 8, myBPM=0, Threshold = 550;

const int PulseWire  = A0;
PulseSensorPlayground pulseSensor;
unsigned  long previousMillis = 0;
const long interval = 5000;

void setup()
{
  dht.setup(9);
  lcd.begin(16, 2);
  Serial.begin(9600);
  esp.begin(115200);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();

  pinMode(buzzer,  OUTPUT);
  digitalWrite(buzzer, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("  IoT Patient");
  lcd.setCursor(0, 1);
  lcd.print(" Monitor System");
  delay(1500);
  digitalWrite(buzzer, LOW);
  lcd.clear();
}

void  loop()
{
  delay(dht.getMinimumSamplingPeriod());  /* Delay of amount equal to sampling period */
  temp = 25;
  Serial.print("Temp:");
  Serial.println(temp);
  lcd.setCursor(0,  0);
  lcd.print("BODY:");
  lcd.print(temp);
  lcd.print("  *C");
  myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat())
  {
    beep();
    lcd.setCursor(0, 1);
    lcd.print("HEART:");
    lcd.print(myBPM);
    lcd.setCursor(9, 1);
    lcd.print(" BPM");
    Serial.println(myBPM);
    delay(20);
  }
  upload();
}

void beep()
{
  digitalWrite(buzzer, HIGH);
  delay(150);
  digitalWrite(buzzer, LOW);
}

void upload()
{
  unsigned long currentMillis  = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis  = currentMillis;
    esp.print('*');
    esp.print(myBPM);
    esp.print(temp);
    esp.println('#');

    Serial.print('*');
    Serial.print(myBPM);
    Serial.print(temp);
    Serial.println('#');
  }
}
