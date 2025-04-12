#include <SPI.h>
#include <LoRa.h>

#define SS      5
#define RST     14
#define DIO0    26

#define RED_LED     2
#define YELLOW_LED  4
#define GREEN_LED   16

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check wiring.");
    while (true);
  }

  Serial.println("LoRa Transmitter with Timelapse");
  LoRa.setTxPower(10); // Safe level
  LoRa.enableCrc();
}

void sendSignal(String color, int secondsLeft) {
  LoRa.beginPacket();
  LoRa.print(color + "_" + String(secondsLeft));  // e.g., RED_10
  LoRa.endPacket();
  Serial.println("Sent: " + color + "_" + String(secondsLeft));
}

void blinkAndTransmit(int ledPin, String color) {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(ledPin, HIGH);

  for (int i = 10; i > 0; i--) {
    sendSignal(color, i);
    delay(1000);  // wait 1 second
  }

  digitalWrite(ledPin, LOW);
}

void loop() {
  blinkAndTransmit(RED_LED, "RED");
  blinkAndTransmit(YELLOW_LED, "YELLOW");
  blinkAndTransmit(GREEN_LED, "GREEN");
}
