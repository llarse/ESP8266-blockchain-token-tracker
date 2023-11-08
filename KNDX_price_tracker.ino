#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Network credentials
const char ssid[] = ""; 
const char password[] = "";

// API endpoint
const char priceCheckEndpoint[] = "https://api.coingecko.com/api/v3/simple/token_price/ethereum?contract_addresses=0x7ca5af5ba3472af6049f63c1abc324475d44efc1&vs_currencies=usd&include_market_cap=true"; 

// OLED display address
#define OLED_ADDR 0x3C
#define RESET_PIN   -1


// Display object
Adafruit_SSD1306 display(128, 64, &Wire, RESET_PIN);

WiFiClientSecure client;

void connectWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}

bool fetchKNDXData(String &jsonData) {
  HTTPClient http;
  client.setInsecure();
  http.begin(client, priceCheckEndpoint);
  http.addHeader("Accept", "application/json");
  int httpCode = http.GET();

  if (httpCode == 200) {
    jsonData = http.getString();
    http.end();
    return true;
  } else {
    http.end();
    return false;
  }
}

void displayInfo(float price, float marketCap) {
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print(F("$KNDX"));
  display.display();
  delay(3000);

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 20);
  display.println(F("Price"));
  display.setTextSize(2);
  display.print("$");
  display.print(price, 6);
  display.display();
  delay(3000);

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 20);
  display.println(F("Mkt Cap"));
  display.setTextSize(2);
  display.print("$");
  display.print((int)marketCap);
  display.display();
  delay(3000);
}

void setup() {
  Serial.begin(115200);
  connectWifi();
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;); // Don't proceed, loop forever
  }
  // display.setRotation(2); // Rotate the display 180 degrees. Specific to my setup.
  display.display();
  delay(1000);
}

void loop() {
  static unsigned long lastFetch = 0;
  static float lastPrice = 0.0;
  static float lastMarketCap = 0.0;

  // Check every 5 mins to never hit api limits
  if (millis() - lastFetch > (5*60000) || lastFetch == 0) {
    String jsonData;
    if (fetchKNDXData(jsonData)) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, jsonData);

      if (!error) {
        lastPrice = doc["0x7ca5af5ba3472af6049f63c1abc324475d44efc1"]["usd"].as<float>();
        lastMarketCap = doc["0x7ca5af5ba3472af6049f63c1abc324475d44efc1"]["usd_market_cap"].as<float>();
      }
    }
    lastFetch = millis();
  }
  displayInfo(lastPrice, lastMarketCap);
}