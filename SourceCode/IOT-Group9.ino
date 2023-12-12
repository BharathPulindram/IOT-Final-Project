#include "heltec.h"
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define BLYNK_TEMPLATE_ID "TMPLI2oDQqir"
#define BLYNK_TEMPLATE_NAME "iot group 9"

#define BLYNK_AUTH_TOKEN "7xFpQtW7R_zGC9XQeKJ5x5wmBqqF53EK"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define DHT_SENSOR_PIN 21
#define DHT_SENSOR_TYPE DHT22
int airQuality = 33;
int gasQuality = 35;
int propane = 32;

const int irSensor1 = 2;
const int irSensor2 = 0;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Goldfish Evian";
char pass[] = "lazywater505";

BlynkTimer timer;
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

WiFiClientSecure client;
void sendSensor() {

  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature(true);
  int air_quality = analogRead(airQuality);
  int gas_quality = analogRead(gasQuality);
  int prs_quality = analogRead(propane);
  delay(200);

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, air_quality);
  Blynk.virtualWrite(V3, gas_quality);
  Blynk.virtualWrite(V4, prs_quality);
  delay(300);
}

int airQual = 0;
float currTempF = 0.0;
float currHumidity = 0.0;

void displayValuesOled() {
  int currHumidity = dht.readHumidity();
  int currTempF = dht.readTemperature(true);
  int airQual = analogRead(airQuality);
  String tempFarenheit = "Temperature : " + (String)currTempF + "*F";
  String humidtyDisplay = "Humidity : " + (String)currHumidity + "%";
  String airQuality = "AirQuality : " + (String)airQual;

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, tempFarenheit);
  Heltec.display->drawString(0, 12, humidtyDisplay);
  Heltec.display->drawString(0, 24, airQuality);
  Heltec.display->display();
}

void setup() {
  delay(2000);
  pinMode(27, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, sendSensor);
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  currTempF = dht.readTemperature(true);
  currHumidity = dht.readHumidity();
  airQual = analogRead(airQuality);
  displayValuesOled();
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFI");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(27, LOW);
    delay(2000);
    Serial.print("*");
    digitalWrite(27, HIGH);
    delay(2000);
  }
  digitalWrite(27, HIGH);
  Serial.print("OKAY");
}

void loop() {
  Blynk.run();
  timer.run();
  delay(2000);
  currTempF = dht.readTemperature(true);
  currHumidity = dht.readHumidity();
  airQual = analogRead(airQuality);

  // Check the person count
  inCount = digitalRead(irSensor1);
  outCount = digitalRead(irSensor2);
  int totalCount =0;
  if (!inCount == LOW)
  {
    totalCount++; // increment for every person entering
  }
  else if(!outCount == LOW)
  {
      totalCount--; // decrement for every person leaving
  }

  if(totalCount > 6) // if count > 6 - alert is thrown
  {
    Blynk.virtualWrite(V6, totalCount);
  }

}