#include "config.h"

#define THRESHOLD 3
#define AVG_READS 20
#define RECORD_INTERVAL 50u
#define RESET_INTERVAL (AVG_READS * RECORD_INTERVAL)

int sensorReadings[AVG_READS], nonce = 0;
int prevAvg = 0, avgReading = 0, dispReading = 0;

unsigned long prevRecordTS = millis(), prevLoopTS = millis(), now;

void setup() {
  Serial.begin(9600);
  setupReadings();
  setupDisplay();
  setupESP8266Wifi();
  setupMQTT();
}

void loop() {
  unsigned long now = millis();

  if (storeReading(now)) {
    prevLoopTS = now;
    processReading(now);
    dispReading = maxReading();
    sendReadingToMQTT(dispReading);

    Serial.print("Changing display to ");
    Serial.println(dispReading);
  } else if ((unsigned long)(now - prevLoopTS) > RESET_INTERVAL && dispReading != 0) {
    // If the loop has gone too long without a threshhold reading
    prevLoopTS = now;
    setupReadings();
    dispReading = 0;

    Serial.println("Reseting display to zero.");
  }

  displayNumber(dispReading);
  mqttLoop();
}

void setupReadings() {
  for (int i = 0; i < AVG_READS; i++) { sensorReadings[i] = 0; }
}

bool storeReading(unsigned long ts) {
  // Only record new readings at/after predetermined measurement intervals
  if ((unsigned long)(ts - prevRecordTS) <= RECORD_INTERVAL) {
    return false;
  }

  prevRecordTS = ts;
  nonce = (nonce + 1) % AVG_READS;
  sensorReadings[nonce] = analogRead(A0);

  Serial.print("Read: ");
  Serial.println(sensorReadings[nonce]);

  // we have a good reading, let's pass that info up the stack
  if (sensorReadings[nonce] > THRESHOLD) {
    return true;
  }

  // remove the low threshhold reading and pass the fail up the stack
  Serial.println("Zeroing last read, below threshold.");
  sensorReadings[nonce] = 0;
  return false;
}

void processReading(unsigned long ts) {
  // We take a 20 reading rolling average to make sure we have a good read.
  int total = 0;
  for (int i = 0; i < AVG_READS; i++) {
    total += sensorReadings[i];
  }
  avgReading = (total / AVG_READS);

  if (avgReading != prevAvg) {
    Serial.print(AVG_READS);
    Serial.print(" sample rolling average: ");
    Serial.println(avgReading);
  }
}

int maxReading() {
  int maximum = 0;
  for (int i = 0; i < AVG_READS; i++) {
    if (sensorReadings[i] > maximum) {
      maximum = sensorReadings[i];
    }
  }
  return maximum;
}
