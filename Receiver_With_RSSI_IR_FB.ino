#include <SPI.h>
#include <LoRa.h>

#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  26

#define VIBRATION_PIN 12    // Vibration motor control pin (GPIO12)
#define IR_SENSOR_PIN 34    // IR sensor input pin (GPIO34)

void setup() {
  Serial.begin(115200);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("❌ LoRa init failed. Check wiring.");
    while (true);
  }

  pinMode(VIBRATION_PIN, OUTPUT);
  digitalWrite(VIBRATION_PIN, LOW);

  pinMode(IR_SENSOR_PIN, INPUT);
  Serial.println("✅ LoRa Receiver ready...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    received.trim();  // Remove trailing newline/spaces

    int rssi = LoRa.packetRssi();
    int irState = digitalRead(IR_SENSOR_PIN);

    Serial.print("📩 Received: ");
    Serial.println(received);
    Serial.print("📶 RSSI: ");
    Serial.println(rssi);

    // Print traffic signal proximity
    if (rssi <= -20) {
      Serial.println("🚦 Traffic Signal Nearby");

      if (received.startsWith("RED") && irState == HIGH) {
        Serial.println("🚶‍ Zebra Crossing Detected");
      }
    }

    // Vibrate motor only when GREEN signal received (even GREEN_1 etc.)
    if (received.startsWith("GREEN")) {
      Serial.println("🟢 GREEN Signal Received – Vibrating Motor");
      digitalWrite(VIBRATION_PIN, HIGH);
      delay(1000);
      digitalWrite(VIBRATION_PIN, LOW);
    } else {
      digitalWrite(VIBRATION_PIN, LOW);
    }

    delay(1000);
  }
}
