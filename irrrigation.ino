
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definitions
#define DHTPIN D4             // ESP8266 GPIO2
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN A0  // Analog input
#define RELAY_PIN D6        // Relay control (GPIO13)
#define DHT_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
#define DRY_THRESHOLD 700     // Adjust based on calibration

// Initialize
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(115200);
  dht.begin();

  lcd.begin();
  lcd.clear();
  lcd.backlight();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  int soilRaw = analogRead(SOIL_MOISTURE_PIN);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("SoilRaw:");
  lcd.print(soilRaw);

  if (soilRaw > DRY_THRESHOLD) {
    digitalWrite(RELAY_PIN, LOW);  // Turn pump ON
    lcd.print("POn");
  } else {
    digitalWrite(RELAY_PIN, HIGH);   // Turn pump OFF
    lcd.print("POff");
  }

  Serial.printf("T=%.1f C, H=%.0f%%, Soil=%d\n", temperature, humidity, soilRaw);

  delay(1000);
}
