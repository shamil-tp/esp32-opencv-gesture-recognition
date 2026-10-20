#include <WiFi.h>

// Wi-Fi Credentials
const char* ssid = "THOTTUNGAL4G";
const char* password = "shamil927";

// Onboard Flash LED Pin for ESP32-CAM (AI-Thinker)
#define LED_PIN 4

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED Off initially

  Serial.begin(115200);
  delay(1000);

  Serial.println("\n--- ESP32-CAM Wi-Fi Test ---");
  Serial.printf("Connecting to %s\n", ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Fast blink while attempting to connect
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) { // Timeout after ~15 seconds
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH); // Solid ON for successful connection
    Serial.println("\nConnected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("\nFailed to connect to Wi-Fi!");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Solid ON with a short pulse every 5 seconds to signal connection
    digitalWrite(LED_PIN, HIGH);
    Serial.printf("Connected | IP: %s | RSSI: %d dBm\n", 
                  WiFi.localIP().toString().c_str(), 
                  WiFi.RSSI());
    delay(4800);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  } else {
    // Slow blink if disconnected
    Serial.println("Wi-Fi Disconnected!");
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
    }
  }
}