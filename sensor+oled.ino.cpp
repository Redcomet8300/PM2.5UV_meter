#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DFRobot_GYML8511.h>
#include <PMS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8

#define BME280_ADDRESS 0x76
#define GYML8511_PIN A0

Adafruit_BME280 bme;
DFRobot_GYML8511 gyml8511;
PMS pms(Serial1);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB); // Initialize display with black tab

  if (!bme.begin(BME280_ADDRESS)) {
    Serial.println("(BME280) Could not find a valid sensor, check wiring!");
    while (1);
  }

  gyml8511.begin();

  Serial1.begin(9600);
  if (!pms.readData()) {
    Serial.println("(PMS) sensor error, check wiring!");
    while (1);
  }
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  int uvIndex = gyml8511.getUVIndex();

  if (pms.readData()) {
    float pm10 = pms.getPM10();
    float pm25 = pms.getPM25();
    float pm100 = pms.getPM100();

    // Clear previous data
    tft.fillScreen(ST77XX_BLACK);

    // Print data to TFT display
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("Temperature: ");
    tft.println(temperature);
    tft.print("Humidity: ");
    tft.println(humidity);
    tft.print("Pressure: ");
    tft.println(pressure);
    tft.print("UV Index: ");
    tft.println(uvIndex);
    tft.print("PM1.0: ");
    tft.println(pm10);
    tft.print("PM2.5: ");
    tft.println(pm25);
    tft.print("PM10: ");
    tft.println(pm100);
  } else {
    Serial.println("(PMS) Failed to read sensor data!");
  }

  delay(5000); // 5 seconds delay
}
