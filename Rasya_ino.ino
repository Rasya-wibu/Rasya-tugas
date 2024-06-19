#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

#define DHTPIN 21 // Pin tempat sensor DHT11 dihubungkan
#define DHTTYPE DHT11 // Jenis sensor DHT (DHT11 atau DHT22, bisa disesuaikan)

const char* ssid = "40i"; // SSID WiFi
const char* password = "12345679"; // Password WiFi

DHT dht(DHTPIN, DHTTYPE); // Inisialisasi objek DHT

void setup() {
Serial.begin(9600);
Serial.println("Hello, ESP32!");

dht.begin(); // Mulai sensor DHT

// Sambungkan ke WiFi
Serial.printf("Connecting to WiFi ", ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("WiFi connected");
}

void loop() {
// Baca nilai kelembapan dan suhu dari sensor DHT
float humidity = dht.readHumidity();
float temperature = dht.readTemperature();

if (isnan(humidity) || isnan(temperature)) {
Serial.println("Failed to read from DHT sensor!");
return;
}

// Buat timestamp berdasarkan waktu millis
unsigned long currentMillis = millis();
String timestampStr = String(currentMillis);

// Persiapkan data JSON untuk dikirim
String data = "{"
" "humidity": " + String(humidity, 2) + ","
" "temperature": " + String(temperature, 2) + ","
" "timestamp": " + timestampStr +
"}";

// Kirim data ke server web melalui HTTP POST
HTTPClient http;
http.begin("http://192.168.103.207:5000/api/sensor");
http.addHeader("Content-Type", "application/json");

int httpCode = http.POST(data);

if (httpCode > 0) { // Jika respons diterima dari server
if (httpCode == HTTP_CODE_OK) {
Serial.println("Data sent successfully");
} else {
Serial.println("Unexpected HTTP code: " + String(httpCode));
}
} else {
Serial.println("Error on sending POST: " + http.errorToString(httpCode));
}

http.end(); // Selesai dengan objek HTTPClient

delay(6000); // Tunggu 6 detik sebelum mengirim data lagi
}
