#include <SPI.h>
#include <LoRa.h>

#define SS      5
#define RST     14
#define DIO0    26

#define VIBRATION_PIN 12  // Your motor control pin

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(VIBRATION_PIN, OUTPUT);
  digitalWrite(VIBRATION_PIN, LOW);

  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check wiring.");
    while (true);
  }

  Serial.println("LoRa Receiver Ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    incoming.trim();
    Serial.println("Received: " + incoming);

    // Only vibrate if message starts with GREEN
    if (incoming.startsWith("GREEN")) {
      digitalWrite(VIBRATION_PIN, HIGH);
      delay(1000);  // Vibrate for 1 second
      digitalWrite(VIBRATION_PIN, LOW);
    }
  }
}
