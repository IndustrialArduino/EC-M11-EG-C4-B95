#define RXD2 4
#define TXD2 2
#define PIN 5
#define RES 19

#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define MAXDO 19
#define MAXCS 5
#define MAXCLK 18
#define OUTPUT1 21

// Initialize the thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(PIN, OUTPUT);
  pinMode(RES, OUTPUT);
  pinMode(OUTPUT1, OUTPUT);

  digitalWrite(RES, HIGH);
  digitalWrite(PIN, HIGH);
  digitalWrite(OUTPUT1, HIGH);
  delay(1000);
  digitalWrite(OUTPUT1, LOW);
  delay(500);
  digitalWrite(OUTPUT1, HIGH);

  Serial.println("MAX31855 test");
  // Wait for MAX chip to stabilize
  delay(500);
  Serial.print("Initializing sensor...");
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }

  // Optional: Can configure fault checks as desired (default is ALL)
  // Multiple checks can be logically OR'd together.
  // thermocouple.setFaultChecks(MAX31855_FAULT_OPEN | MAX31855_FAULT_SHORT_VCC);

  Serial.println("DONE.");
}

void loop() {
  // Basic readout test, print the current temperature
  Serial.print("Internal Temp = ");
  Serial.println(thermocouple.readInternal());

  double c = thermocouple.readCelsius();
  if (isnan(c)) {
    Serial.println("Thermocouple fault(s) detected!");
    uint8_t e = thermocouple.readError();
    if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
    if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");
    if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");
  } else {
    Serial.print("C = ");
    Serial.println(c);
  }
  // Serial.print("F = ");
  // Serial.println(thermocouple.readFahrenheit());
  Serial.println("");

  Serial.print("Analog Read: ");
  Serial.print(analogRead(36));
  Serial.println("");

  // Read from port 1 (Serial2), send to port 0 (Serial)
  while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }

  // Read from port 0 (Serial), send to port 1 (Serial2)
  while (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }

  delay(1000);
}
