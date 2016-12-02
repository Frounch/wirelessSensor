/***************************************************************************
This is a library for the BME280 humidity, temperature & pressure sensor

Designed specifically to work with the Adafruit BME280 Breakout
----> http://www.adafruit.com/products/2650

These sensors use I2C or SPI to communicate, 2 or 4 pins are required
to interface.

Adafruit invests time and resources providing this open source code,
please support Adafruit andopen-source hardware by purchasing products
from Adafruit!

Written by Limor Fried & Kevin Townsend for Adafruit Industries.
BSD license, all text above must be included in any redistribution
***************************************************************************/

#include <Adafruit_BME280.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define serialPeriod 2000
#define screenPeriod 2000

Adafruit_BME280 bme; // I2C
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
double temperature,pressure,humidity;

int count = 0;
int max = serialPeriod / screenPeriod;

void setup() {
  // Demarre le BME280
  bme.begin();
  Serial.begin(9600);

  // Demarre l'ecran LCD
  lcd.begin(16, 2);
  lcd.print("Temp:         *C");
  lcd.setCursor(0,1);
  lcd.print("Humidity:     % ");

  root["sensor"] = "static";
}

void loop() {
  // Maj Temp
  temperature = bme.readTemperature();
  pressure = bme.readPressure();
  humidity = bme.readHumidity();

  lcd.setCursor(11,0);
  lcd.print((int)temperature);
  lcd.setCursor(11,1);
  lcd.print((int)humidity);

  if(count < max  )
  {
    count++;
  }
  else
  {
    count = 0;
    root["temp"] = temperature;  
    root["humidity"] = humidity ;
    root["pressure"] = pressure;
    root.printTo(Serial);
    Serial.println();
  }
  delay(screenPeriod);
}

