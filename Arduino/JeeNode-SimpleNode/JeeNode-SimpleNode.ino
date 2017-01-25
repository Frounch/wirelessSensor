#include <JeeLib.h>
#include <ArduinoJson.h>
#include <Adafruit_BME280.h>

#define CHAMBRE 1
#define EXT 0

#if CHAMBRE
#define jeenodeId 3
#define dataId "slp"
#endif
#if EXT
#define jeenodeId 4
#define dataId "out"
#endif

#define jeenodeGroup 201

#define DEBUG 0
#if DEBUG
#define updatePeriod   10
#define SERIAL 1
#else
#define updatePeriod   600
#define SERIAL 0
#endif

MilliTimer sendTimer;
byte needToSend;
StaticJsonBuffer<65> jsonBuffer;
JsonObject& data = jsonBuffer.createObject();
char payload[65];
double temperature, pressure, humidity;
Adafruit_BME280 bme;
int seconds;

void setup() {
#if SERIAL
  Serial.begin(9600);
#endif
  
  bme.begin();
  seconds = 0;
  rf12_initialize(jeenodeId, RF12_868MHZ, jeenodeGroup);
  rf12_sleep(0);
  data["id"] = dataId;
}

void loop() {
   if (sendTimer.poll(1000))
  {
     seconds++;
     if(seconds >= updatePeriod)
     {
       seconds = 0;
   
       // Prepare jeenode rfm12b
        rf12_sleep(-1);
       needToSend = 1;
       if (rf12_recvDone() && rf12_crc == 0) {
        for (byte i = 0; i < rf12_len; ++i) {
          Serial.print((char) rf12_data[i]);
        }
        Serial.println();
       }
       
       // Read sensors
       data["t"] = bme.readTemperature();
       data["h"] = bme.readHumidity();
       data["p"] = bme.readPressure();
       if(rf12_lowbat())
        data["lowbat"] = 1;
       else
        data["lowbat"] = 0;

       // Export to buffer
       data.printTo(payload, sizeof payload);

      

#if SERIAL
       // print to serial
       Serial.print("(");
       Serial.print(data.measureLength());
       Serial.print(") ");
       Serial.println(payload);
       Serial.flush();
#endif      
     }
  }

  if (needToSend && rf12_canSend()) {
    needToSend = 0;
    rf12_sendStart(0, payload, data.measureLength());
    rf12_sendWait(2);
    rf12_sleep(0);
  }
}
