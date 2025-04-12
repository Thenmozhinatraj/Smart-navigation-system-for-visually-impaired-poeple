#include <SPI.h>
#include <LoRa.h>

#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  26

#define VIBRATION_PIN 12  // Vibration motor control pin (GPIO12)

void setup() {
  Serial.begin(115200);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("❌ LoRa init failed. Check wiring.");
    while (true);
  }

  pinMode(VIBRATION_PIN, OUTPUT);
  digitalWrite(VIBRATION_PIN, LOW);

  Serial.println("✅ LoRa Receiver ready...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();

    Serial.print("📩 Received: ");
    Serial.println(received);
    Serial.print("📶 RSSI: ");
    Serial.println(rssi);

    // Vibration Logic Based on RSSI
    if (rssi > -15) {
      Serial.println("🟢 Strong Signal – Vibrate Long");
      digitalWrite(VIBRATION_PIN, HIGH);
      delay(1000);
      digitalWrite(VIBRATION_PIN, LOW);
    }
    else if (rssi > -20) {
      Serial.println("🟡 Medium Signal – Vibrate Short");
      digitalWrite(VIBRATION_PIN, HIGH);
      delay(300);
      digitalWrite(VIBRATION_PIN, LOW);
    }
    else {
      Serial.println("🔴 Weak Signal – No Vibration");
      digitalWrite(VIBRATION_PIN, LOW);
    }

    delay(1000); // Prevent continuous triggers
  }
}
