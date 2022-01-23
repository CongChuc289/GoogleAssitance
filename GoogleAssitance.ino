#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID       "ChucDao"             // Your SSID
#define WLAN_PASS       "11111111"        // Your password
#define AIO_SERVER      "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "ChucDao"            // Username
#define AIO_KEY         "aio_EWLw23ukozuy9tlxyS3Vd99CPI4r"   // Auth Key
#define LED1 D1
#define LED2 D2
#define button1 D5
#define button2 D6
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe Device1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Device1");
Adafruit_MQTT_Subscribe Device2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Device2");
void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  connectwifi();
  mqtt.subscribe(&Device1);
  mqtt.subscribe(&Device2);
}
void connectwifi()
{
  WiFi.disconnect();
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connect wifi");
}
void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &Device1) {
      if (!strcmp((char*) Device1.lastread, "ON1")) {
        digitalWrite(LED1, HIGH);
        Serial.println("Bat Den 1");
      }
      else if (!strcmp((char*) Device1.lastread, "OFF1")) {
        digitalWrite(LED1, LOW);
        Serial.println("Tat Den 1");
      }
    }
    if (subscription == &Device2) {
      if (!strcmp((char*) Device2.lastread, "ON2")) {
        digitalWrite(LED2, LOW);
        Serial.println("Bat Den 2");
      }
      else if (!strcmp((char*) Device2.lastread, "OFF2")) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Tat Den 2");
      }
    }
  }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
