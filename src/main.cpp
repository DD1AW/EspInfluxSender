#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <InfluxDb.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define INFLUXDB_HOST "192.168.1.1"   //Enter IP of device running Influx Database
#define WIFI_SSID "SSID"              //Enter SSID of your WIFI Access Point
#define WIFI_PASS "PASSWORD"          //Enter Password of your WIFI Access Point
ESP8266WiFiMulti WiFiMulti;
Influxdb influx(INFLUXDB_HOST);

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4; 

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


void setup() {
  // Start the Serial Monitor
  Serial.begin(9600);

 WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WIFI");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  influx.setDb("esp8266_test");
  
  // Start the DS18B20 sensor
  sensors.begin();

  Serial.println("Setup Complete.");
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");

  InfluxData row("data");
  row.addTag("Device", "ESP8266");
  row.addTag("Sensor", "Temp");
  row.addTag("Unit", "Celsius");
  row.addValue("Temperature", temperatureC);
  influx.write(row);

  delay(5000);
}