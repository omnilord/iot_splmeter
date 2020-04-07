/*
 *
 * communication module
 * The code here is for connecting to Wifi, connecting to
 * an MQTT broker, transmitting messages to said broker,
 * and performing any required upkeep of network connectivity.
 *
 */
#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <PubSubClient.h>

/* macros in ALL CAPS are defined in config.h */
#include "config.h"

SoftwareSerial esp8266(RX, TX);
WiFiEspClient wifi_client;
int wif_status = WL_IDLE_STATUS;
PubSubClient mqttClient(WiFi);

void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setupESP8266Wifi() {
  esp8266.begin(115200);
  WiFi.init(&esp8266);

  // check for acceptable setup of esp8266
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while (wifi_status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(AP);
    wifi_status = WiFi.begin(AP, PASS);
  }

  // SUCCESS!
  Serial.println("You're connected to the network");
  printWifiStatus();
}

void setupMQTT() {
  delay(1500);
  mqttClient.setServer(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
  mqtt_reconnect();
}

void mqtt_reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_LWT_TOPIC, 0, false, MQTT_LWT_MSG)) {
      Serial.println("connected");
      mqttClient.publish(MQTT_LWT_TOPIC, millis());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // 5 seconds
    }
  }
}

void mqttLoop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

void sendReadingToMQTT(int reading) {
  boolean rc = mqttClient.publish(MQTT_PUBLISH_TOPIC, "myMessage");
}
