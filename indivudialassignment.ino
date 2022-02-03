#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"  
//#include <Adafruit_Sensor.h>
#include <DHT.h>
 
char auth[] = "l0XcZAMpCnymzP1HGGIjSa9FCoF16EDn";    // You should get Auth Token in the Blynk App.
char ssid[] = "test";                           // Your WiFi credentials.
char pass[] = "12345678";
 
#define DHTTYPE DHT11     // DHT 11
#define DHTPIN D4
 
#define buzzer_alarm D0
#define pir_human D3
 
 
int alarm_status;
int pir_status = 0;
 
DHT dht(DHTPIN, DHTTYPE);
 
 
void setup()
{
  Serial.begin(115200);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
 
  pinMode(pir_human, INPUT);
  pinMode(buzzer_alarm, OUTPUT);
 
  digitalWrite(buzzer_alarm, LOW);
 
  delay(100);
}
 
 
void loop()
{
  Blynk.run();
  MQ135 gasSensor = MQ135(A0);
  float air_quality = gasSensor.getPPM();
 
  float t = dht.readTemperature();
  float h = dht.readHumidity();
 
  pir_status = digitalRead(pir_human);
  alarm_status = digitalRead(buzzer_alarm);
 
 
  if (pir_status == 1)
  {
    Serial.println("Person Detected");
  }
  else if (pir_status == 0)
  {
    Serial.println("No One in Room");
  }
 
 
  if (air_quality > 150)
  {
    digitalWrite(buzzer_alarm, HIGH);
//    digitalWrite(relay_fan, LOW);
    Serial.println("Buzzer Status: ON");
    Serial.println("Exhaust Fan: ON");
  }
  else
  {
    digitalWrite(buzzer_alarm, LOW);
//    digitalWrite(relay_fan, HIGH);
    Serial.println("Buzzer Status: OFF");
    Serial.println("Exhaust Fan: OFF");
  }
 
  Serial.print("Air Quality: ");
  Serial.print(air_quality);
  Serial.println(" PPM");
 
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
 
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
 
  Serial.println();
  Serial.println("****************************");
  Serial.println();
 
  Blynk.virtualWrite(V1, t);             // For Temperature
  Blynk.virtualWrite(V2, h);             // For Humidity
  Blynk.virtualWrite(V3, air_quality);   // For Gas
  Blynk.virtualWrite(V4, alarm_status);  // For Alarm & Exhaust Fan
  Blynk.virtualWrite(V5, pir_status);    // For Human Detection

  delay(1500);
}
