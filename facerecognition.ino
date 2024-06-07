#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Ganti dengan informasi koneksi WiFi Anda
const char *ssid = "ASUS";
const char *password = "asdfghjkl";

// Ganti dengan informasi koneksi MQTT server Anda
const char *mqtt_server = "172.20.10.3";
const char *mqtt_user = "43321101";
const char *mqtt_password = "43321101";
const int mqtt_port = 1883;

// Ganti dengan informasi pin LED Anda
const int led1Pin = D1;
const int led2Pin = D2;
const int led3Pin = D3;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Inisialisasi pin LED sebagai OUTPUT
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  // Mulai koneksi WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Mulai koneksi ke MQTT server
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT server...");
    if (client.connect("NodeMCU", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT server");
      client.subscribe("afif/pasien/1");
      client.subscribe("afif/pasien/2");
      client.subscribe("afif/pasien/3");
    } else {
      Serial.println("Failed to connect to MQTT server, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  client.loop();
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  // Konversi payload menjadi string
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Payload: ");
  Serial.println(message);

  // Handle pesan untuk LED 1
  if (strcmp(topic, "afif/pasien/1") == 0 && message.equals("meminta bantuan")) {
    turnOnAndOff(led1Pin);
  }

  // Handle pesan untuk LED 2
  else if (strcmp(topic, "afif/pasien/2") == 0 && message.equals("meminta bantuan")) {
    turnOnAndOff(led2Pin);
  }

  // Handle pesan untuk LED 3
  else if (strcmp(topic, "afif/pasien/3") == 0 && message.equals("meminta bantuan")) {
    turnOnAndOff(led3Pin);
  }
}

void turnOnAndOff(int pin) {
  digitalWrite(pin, HIGH); // LED menyala
  delay(750);               // Tunggu 750 ms
  digitalWrite(pin, LOW);  // LED mati
}
